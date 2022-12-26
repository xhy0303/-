#!/usr/bin/env python
#
# Copyright 2013 Yue Li
# Copyright 2014 Lizhao You
#
import math,random,time,struct
import socket,sys,threading
import Image, numpy, argparse
import collections

import mac_frame_handler
from mac_params import string_to_hex_list
from mac_params import ENDNODE_FRAME_FORMAT, MAC_HEADER_LEN, CRC_LEN
from mac_params import TOTAL_ARQ_NSEQ as NSEQ
from mac_params import TOTAL_MAC_NSEQ as NSEQ_MAC

global WSIZE
WSIZE = 0

global fwriter
fwriter = None

# /////////////////////////////////////////////////////////////////////////////
#                            Buffer Controller for MAC
# /////////////////////////////////////////////////////////////////////////////
# 1) Manage tx buffer from APP and to PHY;
# 2) Manage rx buffer from PHY and to APP;
# 3) Choose ARQ mode: NULL, E2E-SW, E2E-GBN, E2E-SR;

PRINT_BUFFER_FLAG = 0

class BufferController:
  def __init__(self,content,node,framebytes,windowsize=8,mode=1,dack=False,infile=None,prt_msgq=None):
    self.c = content
    if type(self.c) == 'str':
      self.c = map(ord,self.c)
    self.frame = mac_frame_handler.EndnodeFrame(framebytes,node)
    self.FORMAT = self.frame.f
    self.pktsize = self.frame.pktsize
    self.headersize = self.frame.headersize
    self.npkt = float(len(self.c))/self.pktsize
    self.npkt = int(math.ceil(self.npkt))

    self.node = node
    self.arq_mode = mode
    self.NULL = 129

    global fwriter
    #if self.node == 'A':
    #  fwriter=open('ARQ_TXLOG_A.dat', 'w')
    #if self.node == 'B':
    #  fwriter=open('ARQ_TXLOG_B.dat', 'w')

    # MAC layer's buffer, implemented using linked list
    # should not use [[]]*NSEQ, Google "append to nested list"
    self.tx_buffer = map(lambda x: [], range(NSEQ))
    self.tx_oqueue = [[]]*NSEQ_MAC  # tx's output queue to PHY layer, implemented using list
    self.rx_buffer = [[]]*NSEQ      # rx's input queue from PHY layer, implemented using list
    self.rx_oqueue = []             # rx's output queue to APP layer

    self.f = infile
    self.prt_msgq = prt_msgq

    if self.arq_mode == 1:
      self.arq = NULL_ARQ(node)
    elif self.arq_mode == 2:
      self.arq = N_SW_ARQ(node)
    elif self.arq_mode == 3:
      self.arq = N_GBN_ARQ(node,windowsize,dack,self.f)
    elif self.arq_mode == 4:
      self.arq = N_SR_ARQ(node,windowsize,self.f,sack_flag=False)
    elif self.arq_mode == 5:
      self.arq = N_SR_ARQ(node,windowsize,self.f,sack_flag=True)
    elif self.arq_mode == 6:
      self.arq = RTT_ARQ(node)

    global WSIZE
    WSIZE = windowsize 

    self.index = 0  # current index of the tx buffer

    self.lock = threading.Lock()

    # update tx queue during initialization
    for i in range(self.npkt):
      if type(self.c[0]) == str:
        buff = self.c[self.pktsize*i:self.pktsize*(i+1)]
      elif type(self.c[0]) == int:
        buff = map(chr,self.c[self.pktsize*i:self.pktsize*(i+1)])

      if len(buff) != self.pktsize:
        if type(buff) == list:
          buff = buff + [chr(self.NULL)] * (self.pktsize-len(buff))
        if type(buff) == str:
          buff = buff + struct.pack('!B',self.NULL) * (self.pktsize-len(buff))
      self.tx_buffer[self.index].append(buff)
      self.index = (self.index+1)%NSEQ

    self.recstring = ""

  def bufferUpdate(self,content):
    # to make sure it is an valid update
    if len(content) == 0:  
      return
      
    self.lock.acquire()
    while len(content) > 0:
      buff = content[:self.pktsize]
      if len(buff) < self.pktsize:
        if type(buff) == list:
          buff = buff + [chr(self.NULL)] * (self.pktsize-len(buff))
        if type(buff) == str:
          buff = buff + struct.pack('!B',self.NULL) * (self.pktsize-len(buff))

      self.tx_buffer[self.index].append(buff)

      # FIXME: the bufferUpdate function conflicts with the implementation of wrap transmissions
      # Note that wrap tx is implemented using pop and insert, and the position is related to self.npkt
      # A better strategy is to realize warp tx in upper layer, and control the speed through TCP
      self.npkt += 1
      content = content[self.pktsize:]
      print "=============== self.npkt =",self.npkt, " ", map(ord,buff[0:10])

    self.lock.release()

    self.index = (self.index+1)%NSEQ

  def deliverRxFrames(self,packet):
    # deliver rx frames to APP layer
    self.recstring = self.recstring + packet
    pass

  def parseSACKfield(self,sack):
    (l,) = struct.unpack('!B',sack[self.frame.sack_st_pos])
    r = []
    for i in range(l):
      (s,) = struct.unpack('!B',sack[self.frame.sack_st_pos+i+1])
      r.append(s)
    return r

  def updateRxQueue(self,rxframe,beacon_seq=0):
    # rx a frame from PHY, update RxQueue and RxARQ
    (rec_seq,) = struct.unpack('!B',rxframe[self.FORMAT.RECV_SEQ_POS])
    rec_packet = rxframe[self.headersize:self.headersize+self.pktsize]
    (rec_ack,) = struct.unpack('!B',rxframe[self.FORMAT.RECV_ACK_POS])

    if self.arq_mode == 5:  # SR ARQ with selective ACK
      rec_sack = self.parseSACKfield(rxframe[:self.headersize])
    else:
      rec_sack = None

    if self.node == "A" and PRINT_BUFFER_FLAG:
      print "[RX] Node A RX ", len(rec_packet), string_to_ord_list(rec_packet[:5])
    elif self.node == "B" and PRINT_BUFFER_FLAG:
      print "\t\t\t\t\t[RX] Node B RX ", len(rec_packet), string_to_ord_list(rec_packet[:5])

    # for null ARQ
    if self.arq_mode==1:
      self.deliverRxFrames(rec_packet)
      return rec_packet

    seq = self.arq.pre_recv_ack

    # if rx an ack larger than 1 at first, we should also clean the tx buffer
    if seq is None and rec_ack > 0:
      seq = 0

    if seq is not None:
      if self.node == "A" and PRINT_BUFFER_FLAG:
        print "[RX] Node A SEQ", seq, rec_ack, len(self.tx_buffer[seq])
      if self.node == "B" and PRINT_BUFFER_FLAG:
        print "\t\t\t\t\t[RX] Node B SEQ", seq, rec_ack, len(self.tx_buffer[seq])

      # remove the already recived packets
      self.lock.acquire()
      while seq != rec_ack:
        if len(self.tx_buffer[seq]) > 0:
          #self.tx_buffer[seq].pop(0)

          # implement warp transmission
          self.tx_buffer[(seq+self.npkt)%NSEQ].append(self.tx_buffer[seq].pop(0))
          #print "Insert seq", seq, self.npkt, (seq+self.npkt)%NSEQ
        seq = (seq+1)%NSEQ
      self.lock.release()

    if self.prt_msgq is not None:
      ostr = "[RX] UpdateRxQueue: b#=%d rec_seq=%d rec_ack=%d " % (beacon_seq, rec_seq, rec_ack)
      self.prt_msgq.put(ostr)

    if self.arq_mode == 4 or self.arq_mode == 5:  # SR ARQ
      r = self.arq.update(rec_seq,rec_ack,rec_sack)
      if r[0] == 1:  # in-order pkt
        sseq = r[1]
        eseq = r[2]
        self.rx_buffer[sseq] = rec_packet
        rec_msg = ""
        x = map(ord,rec_packet)
        #print "[ARQ_RX] ", sseq, eseq, rec_seq, beacon_seq, x[0], x[5], x[9]
        while sseq != eseq:
          rec_msg = rec_msg + self.rx_buffer[sseq]
          sseq = (sseq+1)%NSEQ
        #print self.rx_buffer[eseq], eseq
        rec_msg = rec_msg + self.rx_buffer[eseq]
        self.deliverRxFrames(rec_msg)          
        return rec_msg
      elif r[0] == 2:  # out-of-order pkt
        self.rx_buffer[rec_seq] = rec_packet
        return None
      else:
        return None
    else:  # SW/GBN ARQ
      r = self.arq.update(rec_seq,rec_ack)
      if r:
        self.deliverRxFrames(rec_packet)
        return rec_packet
      else:
        return None

  def extractTxFrame(self,beacon_seq=0,burst_seq=None):
    # extract a frame for transmission, require support from TxARQ, update TxQueue
    # Input (embed input information to frame)
    #   @beacon_seq: cur beacon seq num
    #   @burst_seq:  cur burst seq num
    if self.arq_mode == 4 or self.arq_mode == 5:
      self_seq, self_ack, self_sack = self.arq.retrieve()
    elif self.arq_mode == 6:
      self_sack = None
      self_seq, self_ack = self.arq.retrieve(beacon_seq)
    else:
      self_sack = None
      self_seq, self_ack = self.arq.retrieve()

    if self.prt_msgq is not None:    
      if burst_seq is None:
        ostr = "[TX] InsertTxQueue: b#=%d self_seq=%d self_ack=%d " % (beacon_seq,self_seq,self_ack)
      else:
        ostr = "[TX] InsertTxQueue: beacon#=%d burst#=%d self_seq=%d self_ack=%d " % (beacon_seq,burst_seq,self_seq,self_ack)
      self.prt_msgq.put(ostr)

    # extract tx_pkt, depending re-tx mode or tx mode
    tx_frame_list = self.tx_buffer[self_seq]
    if len(tx_frame_list) == 0:  # if no pkts from APP layer, generate a new pkt
      tx_pkt = [chr(self.NULL)] * self.pktsize
    else:                        # if not found in output queue, get it from APP layer
      tx_pkt = tx_frame_list[0]

    if self.node == "A" and PRINT_BUFFER_FLAG:
      print "[TX] Node A PRE_RECV_ACK:", self.arq.pre_recv_ack 
      print "[TX] Node A TX_SEQ:", self_seq, string_to_ord_list(tx_pkt)
    if self.node == "B" and PRINT_BUFFER_FLAG:
      print "\t\t\t\t\t[TX] Node B PRE_RECV_ACK:", self.arq.pre_recv_ack 
      print "\t\t\t\t\t[TX] Node B TX_SEQ:", self_seq, string_to_ord_list(tx_pkt)

    # update self_seq and tx_frame
    tx_frame_data = self.frame.generate_frame_data(beacon_seq,self_seq,self_ack,tx_pkt,burst_seq,self_sack)

    if fwriter:
      x = map(ord,tx_pkt[:10])
      s = "[ARQ_TX] %d %d %d %d %d \n" % (self_seq, burst_seq, x[0], x[5], x[9])
      fwriter.write(s)
    
    if burst_seq is not None:  # for burst mode
      self.tx_oqueue[burst_seq] = tx_frame_data
    else:                      # for beacon mode
      self.tx_oqueue[beacon_seq] = tx_frame_data

    # clean up tx data for NULL_ARQ
    if self.arq_mode == 1:
      self.lock.acquire()

      self.tx_buffer[self_seq].pop(0)
      self.lock.release()
      
    return tx_frame_data

  def getTxFrame(self,txseq):
    # get a transmitted frame on a particular seq
    assert(txseq < NSEQ_MAC)
    return self.tx_oqueue[txseq]

  def getRxBufferSize(self):
    return len(self.recstring)*1.0 / self.pktsize

# ////////////////////////////////////////////////////// #
#                    ARQ Protocols                       #
# ////////////////////////////////////////////////////// #

def IsSeqSmaller(a_seq,b_seq):
  if a_seq == None or b_seq == None:
    return False
  if abs(a_seq-b_seq) > WSIZE: # we are on the seq boundary
    if a_seq > b_seq:  # e.g., a_seq=127 b_seq=1
      return True
    else:
      return False
  else:
    return a_seq < b_seq

def GetNextSeqNum(seq,recv_ack):
  # If recv_ack is larger than seq, use recv_ack
  # Since we use warp seq, we should compare them carefully
  # Depend on whether seq/recv_ack is small (less than 8)

  #print seq, recv_ack
  seqAr = (seq+1)%NSEQ
  seqAd = (seq+1)/NSEQ

  if recv_ack is None:
    return seqAr

  if IsSeqSmaller(seqAr,recv_ack):
    return recv_ack
  else:
    return seqAr

  # TODO: remove the following codes
  # TODO: refractor following programs with IsSeqSmaller function
  #if recv_ack <= WSIZE:
  #  if seqAr <= WSIZE:
  #    return max(seqAr, recv_ack)
  #  else:  # no matter seqAd==0 or seqAd==1
  #    return seqAr
  #elif seqAd == 0 and recv_ack > WSIZE:
  #  return max(seqAr,recv_ack)
  #elif seqAd == 1 and recv_ack > WSIZE:
  #  return seqAr

class RTT_ARQ:
  def __init__(self,node,infile=None):
    self.pre_recv_ack = 0  # should transmit first packet

    self.seq = 0 # cur tx seq no
    self.ack = 0 # cur tx ack no

    self.TX_STATE = ''
    self.RX_STATE = ''

    self.n = node
    self.f = infile
    self.lock = threading.Lock()
    self.prev_beacon=1

  def retrieve(self,beacon_nu):
    self.lock.acquire()  
    r = (self.seq, self.ack)
    if self.prev_beacon<beacon_nu:
     print "!!!!!!!!!!!!!!!!!!what happened!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
     self.seq=0
     self.prev_beacon=self.prev_beacon+1
     r = (self.seq, self.ack)
     self.seq=1
    else:
     self.seq = (self.seq+1)%NSEQ
    
    self.lock.release()

    if self.n=="A":
      ostr = "Node A sending pkt: %d \nNode A sending ack: %d" % (r[0], r[1])
      if self.f is not None:
        self.f.write(ostr)
    elif self.n=="B":
      ostr = "\t\t\t\t\t\t\t\tNode B sending pkt: %d \t\t\t\t\t\t\t\tNode B sending ack: %d\n" % (r[0], r[1])
      if self.f is not None:
        self.f.write(ostr)

    return r

  def update(self,rec_seq,rec_ack):
    self.lock.acquire()
    if self.n=="A":
      ostr = "Node A receive pkt: %d \nNode A receive ack: %d" % (rec_seq, rec_ack)
      if self.f is not None:
        self.f.write(ostr)
    if self.n=="B":
      ostr = "\t\t\t\t\t\t\t\tNode B receive pkt: %d \t\t\t\t\t\t\t\tNode B receive ack: %d\n" % (rec_seq, rec_ack)

    #isNewPkt = False
    #if rec_seq == self.ack:           
    #  self.ack = (rec_seq+1)%NSEQ
     # isNewPkt = True 
      
         
   # elif rec_seq > self.ack:
    #  self.ack = (rec_seq+1)%NSEQ 
      #isNewPkt = True
    isNewPkt = False
    self.ack=rec_seq%NSEQ
      
    # assumption: you will never receive an old ACK (FIFO Queue)
   

    self.lock.release()
    return isNewPkt

class NULL_ARQ:
  def __init__(self,node,infile=None):
    self.pre_recv_ack = 0
    self.seq = 0
    self.ack = 0
    self.n = node
    self.f = infile
    self.lock = threading.Lock()

  def retrieve(self):
    self.lock.acquire()
    r = (self.seq,self.ack)

    if self.n=="A":
      ostr = "Node A sending pkt: %d \nNode A sending ack: %d" % (self.seq, self.ack)
      if self.f is not None:
        self.f.write(ostr)
    elif self.n=="B":
      ostr = "\t\t\t\t\t\t\t\tNode B sending pkt: %d \n\t\t\t\t\t\t\t\tNode B sending ack: %d" % (self.seq, self.ack)
      if self.f is not None:
        self.f.write(ostr)

    self.seq = (self.seq+1)%NSEQ
    self.lock.release()
    return r

  def update(self,rec_seq,rec_ack):
    if self.n=="A":
      ostr = "Node A receive pkt: %d \nNode A receive ack: %d" % (rec_seq, rec_ack)
      if self.f is not None:
        self.f.write(ostr)
    elif self.n=="B":
      ostr = "\t\t\t\t\t\t\t\tNode B receive pkt: %d\n\t\t\t\t\t\t\t\tNode B receive ack: %d" % (rec_seq, rec_ack)
      if self.f is not None:
        self.f.write(ostr)
    return True

class N_SW_ARQ:#end node, stop and wait ARQ
  def __init__(self,node,infile=None):
    self.pre_recv_ack = 0  # should transmit first packet

    self.seq = 0 # cur tx seq no
    self.ack = 0 # cur tx ack no

    self.TX_STATE = ''
    self.RX_STATE = ''

    self.n = node
    self.f = infile
    self.lock = threading.Lock()

  def retrieve(self):
    self.lock.acquire()
    self.seq = self.pre_recv_ack
    r = (self.seq, self.ack)
    self.lock.release()

    if self.n=="A":
      ostr = "Node A sending pkt: %d \nNode A sending ack: %d" % (r[0], r[1])
      if self.f is not None:
        self.f.write(ostr)
    elif self.n=="B":
      ostr = "\t\t\t\t\t\t\t\tNode B sending pkt: %d \n\t\t\t\t\t\t\t\tNode B sending ack: %d" % (r[0], r[1])
      if self.f is not None:
        self.f.write(ostr)

    return r

  def update(self,rec_seq,rec_ack):
    self.lock.acquire()
    if self.n=="A":
      ostr = "Node A receive pkt: %d \nNode A receive ack: %d" % (rec_seq, rec_ack)
      if self.f is not None:
        self.f.write(ostr)
    if self.n=="B":
      ostr = "\t\t\t\t\t\t\t\tNode B receive pkt: %d \n\t\t\t\t\t\t\t\tNode B receive ack: %d" % (rec_seq, rec_ack)

    isNewPkt = False
    if rec_seq == self.ack:           # get correct pkt
      self.ack = (self.ack+1)%NSEQ    # require next pkt
      isNewPkt = True                 # indication

    # assumption: you will never receive an old ACK (FIFO Queue)
    self.pre_recv_ack = rec_ack

    self.lock.release()
    return isNewPkt

class N_GBN_ARQ:
  """
    End to End, End node Go back N ARQ
    * acumulative ack
    * duplicate ACK
    * include TX ARQ and RX ARQ
  """
  def __init__(self,node,windowsize=8,dack=True,infile=None):
    self.pre_recv_ack = None  # previous received ack seq
    self.rx_npkt = 0          # number of packets received so far

    self.windowsize = windowsize
    self.seq = 0  # cur tx seq no
    self.ack = 0  # cur tx ack no

    self.TX_STATE = ''
    self.RX_DUACK = False

    self.USE_DACK = dack
    self.f = infile

    self.n = node
    self.lock = threading.Lock()

  def retrieve(self):
    self.lock.acquire()

    if self.TX_STATE == '':  # at the beginning
      self.TX_STATE = 'NEXT'
    elif self.TX_STATE == 'NEXT':
      if self.pre_recv_ack == None:
        self.seq = (self.seq+1)%NSEQ
        self.TX_STATE = 'NEXT'
      else:
        self.seq = GetNextSeqNum(self.seq,self.pre_recv_ack)  
        if (self.seq-self.pre_recv_ack+NSEQ)%NSEQ == self.windowsize-1:
          self.TX_STATE = 'WINDOW'
    elif self.TX_STATE == 'WINDOW': # Go Back to the beginning
      self.seq = self.pre_recv_ack
      self.TX_STATE = 'NEXT'

    # the ack seq has already been defined
    r = (self.seq, self.ack)
    self.lock.release()

    if self.n=="A":
      ostr1 = "[TX] Node A send_seq: %3d   send_ack: %3d \n" % (r[0], r[1])
      if self.f is not None:
        self.f.write(ostr1)
    elif self.n=="B":
      ostr1 = "\t\t\t\t\t\t\t\t[TX] Node B send_seq: %3d   send_ack: %3d \n" % (r[0], r[1])
      if self.f is not None:
        self.f.write(ostr1)

    return r

  def update(self,rec_seq,rec_ack):
    self.lock.acquire()
    self.rx_npkt = self.rx_npkt + 1
    if self.n == "A":
      ostr1 = "[RX] Node A recv_seq: %3d   recv_ack: %3d\n" % (rec_seq, rec_ack)
      ostr2 = "[RX] Node A self_ack: %3d   self_seq: %3d\n" % (self.ack, self.seq)
      if self.f is not None:
        self.f.write(ostr1)
        self.f.write(ostr2)

    if self.n == "B":
      ostr1 = "\t\t\t\t\t\t\t\t[RX] Node B recv_seq: %3d   recv_ack: %3d\n" % (rec_seq, rec_ack)
      ostr2 = "\t\t\t\t\t\t\t\t[RX] Node B self_ack: %3d   self_seq: %3d\n" % (self.ack, self.seq)
      if self.f is not None:
        self.f.write(ostr1)
        self.f.write(ostr2)

    # RX path: check the status of rec_seq
    isNewPkt = False
    if rec_seq == self.ack: # get correct pkt
      self.ack = (self.ack+1)%NSEQ
      isNewPkt = True

    # TX path: check the status of rec_ack
    if rec_ack == 0 and self.rx_npkt == 1:
      # if first time recv an ack and the ack no is zero
      # it is no sense, and we should ignore it
      pass
    else:
      if IsSeqSmaller(rec_ack,self.pre_recv_ack):
        print "[N_GBN_ARQ] WARNING: you receive an old ACK | pre_recv_ack=%d rec_ack=%d" % (self.pre_recv_ack, rec_ack)
        pass
      elif self.USE_DACK and not self.RX_DUACK and self.pre_recv_ack == rec_ack:
        self.RX_DUACK = True  # if we are in dup_ack state, ignore other dup acks
        self.TX_STATE = 'WINDOW'
      else:
        self.pre_recv_ack = rec_ack
        if self.pre_recv_ack != rec_ack:
          self.RX_DUACK = False

    self.lock.release()
    return isNewPkt   

  def getNextSeqNum2(self,seq,recv_ack):
    # TODO: not tested or remove it?
    if recv_ack == None:
      return (seq+1)%NSEQ

    predicate_seq = seq+1
    if predicate_seq == NSEQ:
      if recv_ack - predicate_seq%NSEQ < self.windowsize:
        return recv_ack
      else:
        return (seq+1)%NSEQ
    else:
      if recv_ack > predicate_seq:
        return recv_ack
      else:
        if recv_ack+NSEQ - predicate_seq < self.windowsize:
          return recv_ack
        else:
          return (seq+1)%NSEQ

class N_SR_ARQ:
  """
    End to End, End node Selective Repeat ARQ
    * acumulative ack
    * include TX ARQ and RX ARQ
    * [todo] selective ack
  """
  def __init__(self,node,windowsize=8,infile=None,sack_flag=True):
    self.rx_npkt =  0    # number of packets received so far
    self.pre_recv_ack = None  # previous rx ack seq#

    self.windowsize = windowsize
    self.seq = 0      # tx branch: cur tx seq#
    self.ack = 0      # tx branch: cur ack seq#

    self.nakque = []  # tx branch: nak seq# queue
    self.rxque  = []  # rx branch: rx  seq# queue

    self.TX_STATE = ''
    self.use_sack = sack_flag
    self.sack_flag = [0]*NSEQ

    self.f = infile
    self.n = node
    self.lock = threading.Lock()

  # Tx branch
  def retrieve(self):
    self.lock.acquire()

    if self.TX_STATE == '':  # at the beginning
      self.seq = 0
      self.nakque.append(self.seq)
      self.TX_STATE = 'NEXT'
    elif self.TX_STATE == 'NEXT' or self.TX_STATE == 'WINDOW_NEXT':
      # the accu ack may change (due to reception) after we update seq last time
      self.seq = GetNextSeqNum(self.seq,self.pre_recv_ack)
      
      #if self.n == "A":
      #  ostr = "[TX][NEXT] Node A: %d \n" % (self.seq)
      #  if self.f is not None:
      #    self.f.write(ostr)
      #if self.n == "B":
      #  ostr = "\t\t\t\t\t\t\t\t[TX][NEXT] Node B: %d \n" % (self.seq)
      #  if self.f is not None:
      #    self.f.write(ostr)

      if self.nakque.count(self.seq) == 0:
        self.nakque.append(self.seq)

      # reset the state (goto beginning) if full
      if len(self.nakque) == self.windowsize and self.seq == self.nakque[self.windowsize-1]:
        self.TX_STATE = 'WINDOW'
    elif self.TX_STATE == 'WINDOW':
      if self.n == "A" and self.pre_recv_ack is not None:
        ostr = "[TX][WINDOW] Node A: %d %d \n" % (self.seq, self.pre_recv_ack)
        if self.f is not None:
          self.f.write(ostr)
      if self.n == "B" and self.pre_recv_ack is not None:
        ostr = "\t\t\t\t\t\t\t\t[TX][WINDOW] Node B: %d %d \n" % (self.seq, self.pre_recv_ack)
        if self.f is not None:
          self.f.write(ostr)

      self.seq = self.pre_recv_ack if self.pre_recv_ack is not None else 0
      self.TX_STATE = 'NEXT'

    # the ack seq has already been updated in update()
    sack = self.genSACK() if self.use_sack else None
    r = (self.seq, self.ack, sack)
    self.lock.release()

    if self.n=="A":
      ostr = "[TX] Node A send_seq: %3d   send_ack: %3d \n" % (r[0], r[1])
      if self.f is not None:
        self.f.write(ostr)
    elif self.n=="B":
      ostr = "\t\t\t\t\t\t\t\t[TX] Node B send_seq: %3d   send_ack: %3d \n" % (r[0], r[1])
      if self.f is not None:
        self.f.write(ostr)

    return r

  def genSACK(self):
    # Check self.rxque
    s = self.ack
    e = self.rxque[-1] if len(self.rxque)>0 else None
    r = []
    if e is not None:
      s = (s+1)%NSEQ
      while True:
        l = (s-1+NSEQ)%NSEQ
        if self.sack_flag[l] == 0 and self.sack_flag[s] == 1:
          r.append(s)
        elif self.sack_flag[l] == 1 and self.sack_flag[s] == 0:
          r.append(l)
        
        if s == e and self.sack_flag[s] == 1:
          r.append(e)

        s = (s+1)%NSEQ
        if s == (e+1)%NSEQ:
          break

    #print "GenSACK: ack=", self.ack, " rxque=", self.rxque, " ret=", r
    assert(len(r)%2 == 0)
    from mac_params import MAC_HEADER_LEN, SACK_ST_POS
    rr = MAC_HEADER_LEN - SACK_ST_POS
    if len(r) > rr:
      if rr%2 == 1:
        rr = rr-1
      return r[:rr]
    return r

  def update(self,rec_seq,rec_ack,rec_sack=None):
    """
    @ret: [mode,rec_seq,end_seq]

    mode: 0 (duplicate pkt)
          1 (in-order pkt)
          2 (out-of-order pkt)

    end_seq: upper layer can deliver [rec_seq:end_seq]
             only useful when mode == 1
    """
    self.lock.acquire()
    self.rx_npkt = self.rx_npkt + 1
    if self.n == "A":
      ostr1 = "[RX] Node A recv_seq: %3d   recv_ack: %3d\n" % (rec_seq, rec_ack)
      ostr2 = "[RX] Node A self_ack: %3d   self_seq: %3d\n" % (self.ack, self.seq)
      if self.f is not None:
        self.f.write(ostr1)
        self.f.write(ostr2)

    if self.n == "B":
      ostr1 = "\t\t\t\t\t\t\t\t[RX] Node B recv_seq: %3d   recv_ack: %3d\n" % (rec_seq, rec_ack)
      ostr2 = "\t\t\t\t\t\t\t\t[RX] Node B self_ack: %3d   self_seq: %3d\n" % (self.ack, self.seq)
      if self.f is not None:
        self.f.write(ostr1)
        self.f.write(ostr2)

    ################################################
    # RX branch: check the status of rec_seq
    # If in-order, incrase ack seq; else, store it
    ################################################
    end_seq = 0
    if IsSeqSmaller(rec_seq,self.ack):
      rx_mode = 0
      pass
    elif rec_seq == self.ack: # get in-order pkt
      rx_mode = 1

      # see if we have received advanced pkt
      seq = (self.ack+1)%NSEQ
      while self.rxque.count(seq) > 0:
        self.rxque.remove(seq)
        if self.use_sack:
          self.sack_flag[seq] = 0
        seq = (seq+1)%NSEQ
      self.ack = seq 
      end_seq = (seq-1+NSEQ)%NSEQ
    else:  # get out-of-order pkt, store it
      c = self.rxque.count(rec_seq)
      if c > 0:
        rx_mode = 0  # receive duplicate pkt
      else:
        rx_mode = 2
        self.rxque.append(rec_seq)
        if self.use_sack:
          self.sack_flag[rec_seq] = 1
          #print "OnOutRx", self.sack_flag

    if self.n == "A":
      ostr =  "[RX] Node A [%d %d %d] self_ack=%d \n" % (rx_mode, rec_seq, end_seq, self.ack)
      if self.f is not None:
        self.f.write(ostr)
        #print self.rxque
    if self.n == "B":
      ostr = "\t\t\t\t\t\t\t\t[RX] Node B [%d %d %d] self_ack=%d \n" % (rx_mode, rec_seq, end_seq, self.ack)
      if self.f is not None:
        self.f.write(ostr)
        #print "\t\t\t\t\t\t\t\t", self.rxque

    ################################################
    # TX branch: check the status of rec_ack
    ################################################
    if rec_ack == 0 and self.rx_npkt == 1:
      # if first time recv an ack and the ack no is zero
      # it is no sense, and we should ignore it
      pass
    else:
      if IsSeqSmaller(rec_ack,self.pre_recv_ack):
        print "[N_SR_ARQ] WARNING: you receive an old ACK | pre_recv_ack=%d rec_ack=%d" % (self.pre_recv_ack, rec_ack)
        sys.exit()
      else:
        fnak_seq = self.nakque[0] if len(self.nakque)>0 else None
        if IsSeqSmaller(rec_ack,fnak_seq):
          print "[N_SR_ARQ] WARNING: you receive an unexpected ACK | nak_seq=%d rec_ack=%d" % (fnak_seq, rec_ack)
          sys.exit()
        #elif rec_ack == fnak_seq:
        #  self.nakque.pop(0)
        else:
          self.pre_recv_ack = rec_ack
          # nakque must be not empty
          # rx an advanced ack, clean up nak queue
          while IsSeqSmaller(fnak_seq,rec_ack):
            self.nakque.pop(0)
            # reset the TX state - allow more transmissions
            if self.TX_STATE == 'WINDOW':
              self.TX_STATE = 'NEXT'
            if len(self.nakque) == 0:
              break
            fnak_seq = self.nakque[0]
            #print "[N_SR_ARQ] nak_seq=%d rec_ack=%d" % (fnak_seq, rec_ack)

    self.lock.release()

    return [rx_mode,rec_seq,end_seq]  

# ////////////////////////////////////////////////////// #
#                    MAIN FUNCTION                       #
# ////////////////////////////////////////////////////// #
import os, glob
def ReadImages(dirPath):
  imageData = ""
  imgfile = []
  os.chdir(dirPath)
  types = ('*.jpg','*.bmp')
  for f in types:
    imgfile.extend(glob.glob(f))
  for i in imgfile:
    print "image file: ",i
    imgloc = i
    #open the img
    img = Image.open(imgloc)
    mode = img.mode
    if mode == 'RGB':
      greyLevel = 3
    elif mode == 'L':
      greyLevel = 1
    img_data = numpy.array(list(img.getdata()), numpy.uint8)
    img_data.shape = len(img_data)*greyLevel,1
    #print "img_data_before send 1: ",len(img_data)
    #img_data consists of 680*976 pixels with RGB 3 values of each pixel, it is like [[r1,g1,b1],[r2,g2,b2].....[r663680],[g663680],[b663680]], list(img.getdata()) returns an operatable list of the img data

    #the following 4 parameters measure the size of the image
    width,height = img.size
    width1 = width / 128
    width2 = width % 128
    height1 = height / 128
    height2 = height % 128

    m = [width1,width2,height1,height2,greyLevel]#1 indicates grey-level
    #print "the picinfo vector is ",m
    #here length means the number of values, not bytes, img_data is a nested array
    for i in range(len(img_data)):
      m.append(img_data[i][0])
    buff_char = map(chr,m)
    string = "".join(buff_char)
    imageData = imageData+string

  return imageData

def ReadImage(imagePath):
  imgloc = imagePath
  #open the img
  img = Image.open(imgloc)
  mode = img.mode
  if mode == 'RGB':
    greyLevel = 3
  elif mode == 'L':
    greyLevel = 1
  img_data = numpy.array(list(img.getdata()), numpy.uint8)
  img_data.shape = len(img_data)*greyLevel,1

  width,height = img.size
  width1 = width / 128
  width2 = width % 128
  height1 = height / 128
  height2 = height % 128

  m = [width1,width2,height1,height2,greyLevel]#1 indicates grey-level

  for i in range(len(img_data)):
    m.append(img_data[i][0])
  return m

def string_to_ord_list(s):
  return map(lambda x: ord(x), s)

if __name__ == '__main__':
  from optparse import OptionParser
  parser = OptionParser()
  parser.add_option("", "--play", type="int", default=1, help="Mode: txt or GUI")
  parser.add_option("-m", "--arq-mode", type="int", default=4, help="ARQ mode")
  parser.add_option("-t", "--times", type="int", default=100, help="time slots")
  parser.add_option("-w", "--windowsize", type="int", default=8, help="window size")  
  parser.add_option("-d", "--downlink", type="float", default=1, help="downlink PRR")
  parser.add_option("-u", "--uplink", type="float", default=1, help="uplink PRR")
  (options, args) = parser.parse_args()

  play = options.play
  assert(play == 1 or play == 2)

  mode = options.arq_mode
  run_times = options.times
  uplink = options.uplink
  downlink = options.downlink
  windowsize = options.windowsize

  framesize = 1535 #757 #404  #74
  pktsize = framesize - 20 - 4

  SEQ_NUM = 256

  if play == 1:
    data_raw1 = []
    data_raw2 = []
    for i in range(run_times):
      x = [i] * pktsize
      for j in range(pktsize):
        data_raw1.append((i+j)%SEQ_NUM)
        data_raw2.append((i+2*j)%SEQ_NUM)
  elif play == 2:
    data_raw1 = ReadImage('img_data/INC.bmp') #_logo_m.jpg')
    data_raw2 = ReadImage('img_data/CUHK.bmp')#logo_m.bmp')

  seed_num = random.randint(0, sys.maxint) # 9000541137569755104
  print "LEN: ", seed_num, len(data_raw1), len(data_raw2)
  max_pic_len = max(len(data_raw1), len(data_raw2))

  import os, sys
  f = None
  #f = sys.stdout
  #f = open('SR_ARQ.dat','w')
  node11 = BufferController(data_raw1,'A',framesize,windowsize,mode,True,f)
  node21 = BufferController(data_raw2,'B',framesize,windowsize,mode,True,f)

  random.seed(seed_num)
  for i in range(run_times):

    if play == 2 and len(node11.recstring) >= max_pic_len \
                 and len(node21.recstring) >= max_pic_len:
      pass #break

    beacon_seq = i%NSEQ
    f11 = node11.extractTxFrame(1,beacon_seq)
    f21 = node21.extractTxFrame(1,beacon_seq)

    print i, ": ", string_to_hex_list(f11[:30]), string_to_hex_list(f11[-4:])
    print i, ": ", string_to_hex_list(f21[:30]), string_to_hex_list(f21[-4:])

    if PRINT_BUFFER_FLAG:
      print "INFO: ", i, len(node11.recstring), len(node21.recstring), "f21=", string_to_ord_list(f21[-10:]), "f11=", string_to_ord_list(f11[-10:])

    if random.randint(1,100) <= uplink*100:
      fxor = ''
      for xx in range(len(f11)):
        fxor += chr(ord(f11[xx])^ord(f21[xx]))
      print i, ": ", len(fxor),string_to_hex_list(fxor[0:130]), string_to_hex_list(fxor[-14:])

      from gnuradio import digital
      x = digital.crc.gen_and_append_crc32(fxor[:-4])
      print "STRING HEX LIST", len(x), string_to_hex_list(x[-4:])

      rx_pkt = mac_frame_handler.HandleXorFrame(fxor,512)
      print i, ": ", string_to_hex_list(rx_pkt[-14:])
      from gnuradio import digital
      (ok, payload) = digital.crc.check_crc32(rx_pkt)
      print ok

      if random.randint(1,100) <= downlink*100:
        # for node A
        #self_seq, = struct.unpack('!B',fxor[7])
        #self_pkt = node11.getTxFrame(self_seq)
        self_pkt = node11.getTxFrame(beacon_seq)
        rxframe = ''
        for xx in range(len(fxor)):
          rxframe += chr(ord(fxor[xx])^ord(self_pkt[xx]))
        #print "Node A: ", (rxframe == f21)
        #print "f21: ", len(f21), string_to_ord_list(f21)

        rxpkt = node11.updateRxQueue(rxframe)
        #print string_to_ord_list(rxpkt)        

        if play == 1 or play == 2:
          rs = node11.recstring
          clen = min(len(rs),len(data_raw2))
          rs = rs[:clen]
          d = data_raw2[:clen]

          flag = (string_to_ord_list(rs[-pktsize:]) == d[-pktsize:])
          if not flag and mode != 1:
            print "===================================================================="
            print "ERROR", i, len(rs), len(data_raw2), "f21=", string_to_ord_list(f21[-10:]), "f11=", string_to_ord_list(f11[-10:])
            print "Node B R:", string_to_ord_list(rs[-(2*pktsize):])
            print "Node B T:", d[-(2*pktsize):]
            break

      if random.randint(1,100) <= downlink*100:  
        # for node B
        #self_seq, = struct.unpack('!B',fxor[9])
        #self_pkt = node21.getTxFrame(self_seq)
        self_pkt = node21.getTxFrame(beacon_seq)
        rxframe = ''
        for xx in range(len(fxor)):
          rxframe += chr(ord(fxor[xx])^ord(self_pkt[xx]))
        #print "Node B: ", (rxframe == f11)

        rxpkt = node21.updateRxQueue(rxframe)
        if play == 1 or play == 2:
          rs = node21.recstring
          clen = min(len(rs),len(data_raw1))
          rs = rs[:clen]
          d = data_raw1[:clen]

          flag = (string_to_ord_list(rs[-pktsize:]) == d[-pktsize:])
          if not flag and mode != 1:
            print "===================================================================="
            print "ERROR", i, len(rs), len(data_raw1), "f21=", string_to_ord_list(f21[-10:]), "f11=", string_to_ord_list(f11[-10:])
            print "Node A R:", string_to_ord_list(rs[-(2*pktsize):])
            print "Node A T:", d[-(2*pktsize):]
            break

  if f is not None and f is not sys.stdout:
    f.close()

  print len(node11.recstring)/pktsize, len(node21.recstring)/pktsize

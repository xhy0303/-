
class SM:
    start_state=None
    def transition_fn(self,s,x):
        pass
    def output_fn(self,s):
        pass
    def transduce(self,input_seq):
        for i in range(len(input_seq)):
            tmp=self.transition_fn(self.start_state,input_seq[i])
            self.start_state=tmp
        print(self.output_fn(self.start_state))

class Accumulator(SM):
    start_state = 0
    def transition_fn(self, s, x):
        return s + x
    def output_fn(self, s):
        return s
class Binary_Addition(SM):
    start_state=(0,0)
    output=[]
    def transition_fn(self, s, x):
        sign=s[0]+x[0]+x[1]
        if sign==2:
            self.output.append(0)
            return (1,0)
        elif sign==1:
            self.output.append(1)
            return (0,1)
        else:
            self.output.append(0)
            return (0,0)
    def output_fn(self, s):
        return self.output
class Reverser(SM):
    start_state = 0
    seq_cache=[]
    output=[]
    sign=0
    def transition_fn(self, s, x):
        if x!='end' and self.sign==0:
            self.seq_cache.append(x)
            self.output.append(None)
            return s+1
        elif s > 0:
            if x=='end' and self.sign==0:
                self.seq_cache.reverse()
            self.sign=1
            self.output.append(self.seq_cache[0])
            self.seq_cache=self.seq_cache[1:]
            return s-1
        else:
            self.output.append(None)
            return s
    def output_fn(self, s):
        return self.output



sample1=Accumulator()
sample1.transduce([-1,2,3,-2,5,6])
sample2=Binary_Addition()
sample2.transduce([(1,1),(1,0),(0,0)])
sample3=Reverser()
sample3.transduce(['foo',' ','bar']+['end']+list(range(5)))
#sample3.transduce(['foo', ' ', 'bar'] + ['end'] + ['end']*3 +list(range(2)))
#sample3.transduce(list('the') + ['end'] + list(range(3)))
#sample3.transduce([] + ['end'] + list(range(5)))
#sample3.transduce(list('nehznehS evol I') + ['end'] +list(range(15)))



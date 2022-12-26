import time
start = time.time()
### your codes
n=0
while(n<10000000):
    '''a=[1,2,3]
    a+=[n]
    n+=1'''
    a=[1,2,3]
    a.append(n)
    n+=1
print(time.time()-start)
from itertools import combinations
import numpy as np
def sum0(list):
    num=len(list)
    for i in range(len(list)):
        for n in combinations(list,num):
            tmp=np.array(n)
            if tmp.sum()==0:
                return True
        num-=1
    return False

print(sum0([-3,11, 21, 5, 10, 11, 2, 1]))
print(sum0([2, 3, 4, 5, 6, 7, 8, 9]))
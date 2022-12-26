from functools import reduce
from math import gcd
def lcm(*num):
    print(reduce(lambda a,b: int(a*b/gcd(a,b)),num,1))
lcm(3,5)
lcm(41, 106, 12) # 26076
lcm(1, 2, 6, 24, 120, 720) # 720
lcm(3) # 3
lcm() # 如果没有向函数提供数字，可以返回值 1。
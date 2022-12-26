from functools import reduce
def add(a,b):
    return a+b
def mul(a,b):
    return a*b
print(reduce(add,[23, 49, 6, 32],-61))
print(reduce(add,['foo', 'bar','baz','quz']))
print(reduce(mul,[2,4,6]))
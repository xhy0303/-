'''def f(n):
    n=5
    print(id(n))
m = 2
f(m)
print(id(m))
print(m)'''

'''def f(L):
    L[0] = 42
    print(L(0))
L = [1,2,3]
f(L)
print(L[0])'''

'''def f():
    n = 5
    #return 1
n=4
n=f()
print(n)'''

'''def f(L):
    L2 = L
    L = [1,2]
    L[0] = 5
    print(L)
L = [2,3]
print(L2)'''

D={"what":22, "are":11, "you":14, "doing":5, "next":9, "Saturday?":4}
sum = 0
for x in D.items():
    sum =sum + x[1]
print(sum)
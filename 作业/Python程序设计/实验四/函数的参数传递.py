def sum(a, b, c):
    print("a=%d, b=%d, c=%d"%(a,b,c))
    print(a+b+c)


sum(*(1,),b=2, c=3)#c
sum(1, *(2, 3))#f
sum(1, *(2, 3))#g

sum(*(1, 2, 3))#a
sum(1, *(2, 3))#b
sum(*(1,),b=2, c=3)#d
sum(*(1, 2),c=3)#e
sum(c=1, *(2, 3))#h
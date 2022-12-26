N=int(input("Please input n: "))
for i in range(N):
    tmp=N-i
    sign=0
    for n in range(2,tmp):
        if tmp%n==0:
            sign=1
            break
    if sign==1:
        sum=1
        for n in range(1,tmp+1):
            sum*=n
        print("%d!: %d"%(tmp,sum))
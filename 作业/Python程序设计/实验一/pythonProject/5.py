x=int(input("Please input N: "))
sign=0
for i in range(pow(10,x-1),pow(10,x)):
    sum=0
    tmp=i
    while tmp>0:
        sum+=pow(tmp%10,x)
        tmp//=10
    if sum==i:
        print(str(sum)+" ",end='')
        sign=1
if sign==0:
    print("[]")
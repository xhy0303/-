a=list(input())
b=list(input())
if len(a)!=len(b):
    print(-1)
else:
    sum=0
    for i in range(len(a)):
        sum+=(int(a[i])*int(b[i]))
    print(sum)
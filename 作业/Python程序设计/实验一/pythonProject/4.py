x = int(input())
sum:str=""
sign=0
if x<0:
    x*=-1
    sign=1
while x > 0:
    tmp1=int(x/7)
    tmp2=int(x%7)
    sum=str(tmp2)+sum
    x=tmp1
if sign==1:
    sum="-"+sum
print(sum)
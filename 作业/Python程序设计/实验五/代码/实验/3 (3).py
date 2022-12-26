x=int(input("Enter an integer: "))
a=0
b=0
c=0
print("Is %d divisible by 5 and 7? "%(x),end='')
if x%5==0 and x%7==0:
    print("True")
    a=1
else:
    print("False")

print("Is %d divisible by 5 or 7? "%(x),end='')
if x%5==0 or x%7==0:
    print("True")
    b=1
else:
    print("False")

print("Is %d divisible by 5 or 7, but not both? " % (x), end='')
if b==1 and a!=1:
    print("True")
else:
    print("False")
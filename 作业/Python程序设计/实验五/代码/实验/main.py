import math
import Triangle

def emirp(N):
    f=open('FirstN_Emirp_Numbers.txt','w')
    count = 1
    for i in range(1,N+1):
        tmp=str(i)
        sign=0
        for n in range(int(len(tmp)/2)):
            if tmp[n]!=tmp[len(tmp)-n-1]:
                sign=1
                break
        flag=0
        if sign==1:#不是回文数
            for n in range(2,int(math.sqrt(i))+1):
                if i%n==0:#不是质数就跳出
                    flag=1
                    break
            if flag==1:
                continue
            else:
                mid=i
                rev=0
                while mid>0:
                    rev=(rev*10)+(mid%10)
                    mid//=10
                for n in range(2,int(math.sqrt(rev))+1):
                    if rev%n==0:
                        flag=1
                        break
                if flag==1:
                    continue
                else:
                    if count%10!=0:
                        #print(i,end='')
                        f.write(str(i)+" ")
                        count+=1
                    else:
                        #print(i)
                        f.write(str(i)+'\n')
                        count+=1

def checkSumOfSquareNumbers(c):
    for i in range(1,c):
        for n in range(i+1,c):
            if pow(i,2)+pow(n,2)==pow(c,2):
                print("True, %d^2+%d^2=%d^2"%(i,n,c))
                return
    print("False")

def twoSumClosest(nums,target):
    min=target-int(nums[0])-int(nums[1])
    for i in range(len(nums)):
        for n in range(i+1,len(nums)):
            if min>target-int(nums[i])-int(nums[n]):
                min=target-int(nums[i])-int(nums[n])
    print(min)

def owlRow(num):
    empty = '      '
    lineA = ' {o,o}'
    lineB = ' /)_) '
    lineC = ' " "  '
    arr=[empty,lineA,lineB,lineC]
    for i in range(len(arr)):
        for n in range(num):
            print(arr[i],end='')
        print()


if __name__=="__main__":
    owlRow(10)































    '''N=int(input("Please input N: "))
    emirp(N)'''

    '''c = int(input("Please input c: "))
    checkSumOfSquareNumbers(c)'''

    '''arr = input("Please input nums: ")
    nums = [int(n) for n in arr.split()]
    target = int(input("Please input target: "))
    twoSumClosest(nums, target)'''
    #owlRow(5)

    '''arr = input("Please input sides: ")
    sides = [int(n) for n in arr.split()]
    if Triangle.isValid(sides[0],sides[1],sides[2]):
        Triangle.area(sides[0],sides[1],sides[2])
        Triangle.perimeter(sides[0],sides[1],sides[2])'''

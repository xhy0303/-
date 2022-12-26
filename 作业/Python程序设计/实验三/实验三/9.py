def check(lst):
    for i in range(int(len(lst)/2)):
        if lst[i]!=lst[len(lst)-1-i]:
            return False
    return True


def is_almost_symmetric(lst):
    for i in range(len(lst)):
        for n in range(i+1,len(lst)):
            lst[i],lst[n]=lst[n],lst[i]
            if check(lst):
                return True
    return False

arr=input("Please input a list: ")
list=[int(n) for n in arr.split()]
print(is_almost_symmetric(list))
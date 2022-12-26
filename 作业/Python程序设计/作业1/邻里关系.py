import string
def offByOne(str1,str2):
    if(len(str1)==len(str2)):
        count = 0
        for i in range(len(str1)):
            if count == 0 and str1[i] != str2[i]:
                count += 1
            elif count == 1 and str1[i] != str2[i]:
                return False
        if count == 1:
            return True
        else:
            return False
    else:
        return False

def offBySwap(str1,str2):
    if str1==str2 or len(str1)!=len(str2):
        return False
    else:
        count=0
        for i in range(len(str1)-1):
            tmp1=str1[0:i]+str1[i+1]+str1[i]+str1[i+1:]
            tmp2=str2[0:i]+str2[i]+str2[i+1]+str2[i+1:]
            if tmp1==tmp2:
                count+=1
        if count==1:
            return True
        else:
            return False

def offByExtra(str1,str2):
    if(len(str1)>len(str2)):
        for i in range(len(str1)):
            tmp=str1[0:i]+str1[i+1:]
            if tmp==str2:
                return True
    else:
        for i in range(len(str2)):
            tmp = str2[0:i] + str2[i + 1:]
            if tmp == str1:
                return True
    return False

def ListOfNeighbors(str,L):
    result=[]
    for l in range(len(L)):
        if L[l]!=str and (offByExtra( L[l],str) or offByOne( L[l],str) or offBySwap( L[l],str)):
            result.append( L[l])
    return result

s = []
with open('EnglishWords.txt') as src:
    for word in src.read().split():
        s.append(word)
print(ListOfNeighbors('news',s))
import string
with open('EnglishWords.txt') as src:
    tmp=[]
    for word in src.read().split('\n'):
        tmp.append(word)
    words=tuple(tmp)
count1={}
count2={}
for word in words:
    count1[word] = 0
    count2[word] = 0
with open('war.txt') as data1:
    str=data1.read();
    for ch in str:
        if ch in string.punctuation:
            str=str.replace(ch,' ')
    for da in str.split():
        if da in count1.keys():
            count1[da]+=1

with open('treasure.txt') as data2:
    str=data2.read();
    for ch in str:
        if ch in string.punctuation:
            str.replace(ch,' ')
    for da in str.split():
        if da in count2.keys():
            count2[da] += 1
with open('hyde.txt') as data3:
    str=data3.read();
    for ch in str:
        if ch in string.punctuation:
            str.replace(ch,' ')
    for da in str.split():
        if da in count2.keys():
            count2[da] += 1


count1_ordered=sorted(count1.items(),key=lambda x:x[1],reverse=True)
count2_ordered=sorted(count2.items(),key=lambda x:x[1],reverse=True)

print("Leo's favorite words are ")
for i in range(20):
    print(count1_ordered[i],end=' ')
print()
print("Bob's favorite words are ")
for i in range(20):
     print(count2_ordered[i],end=' ')
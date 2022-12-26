
'''def merge(list1,list2):
    list=list1+list2
    list.sort()
    return list'''

def merge(list1,list2):
    list=[]
    while(len(list1)!=0 and len(list2)!=0):
        if list1[0]<list2[0]:
            list.append(list1[0])
            list1.remove(list1[0])
        else:
            list.append(list2[0])
            list2.remove(list2[0])
    if len(list1)!=0:
        list+=list1
    else:
        list+=list2
    return list

list1=[2,34]
list2=[0,12,435]
list=merge(list1,list2)
print(list)
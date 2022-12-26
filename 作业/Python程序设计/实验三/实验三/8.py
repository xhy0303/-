arr=input()
children=[int(n) for n in arr.split()]
arr=input()
size=[int(n) for n in arr.split()]
children.sort()
size.sort()
count=0
for i in range(len(size)):
    for n in range(len(children)):
        if size[i]>=children[n]:
            count+=1
            children.remove(children[n])
            break

print(count)

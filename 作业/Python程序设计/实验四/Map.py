def reverse(str):
    return str[::-1]
def process1(x):
    return (x,x*x,x*x*x)
def process2(num):
    return num[0]*num[1]

l1 = ['12', '-2', '0']
print(list(map(int,l1)))
l2=['hello', 'world']
print(list(map(len,l2)))
print(list(map(reverse,l2)))

print(list(map(process1,range(2,6))))
print((list(map(process2,zip(range(2,5),range(3,9,2))))))
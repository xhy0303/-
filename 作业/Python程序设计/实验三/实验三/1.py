s=[0]*3
print(s)
s[0]+=1
print(s)

s=['']*3
print(s)
s[0]+='a'
print(s)

s=[[]]*3
print(s)
s[0]+=[1]
print(s)

print(id(s[0]))
print(id(s[1]))
print(id(s[2]))

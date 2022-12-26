def func1(n):
    return int(n)>0
def func2(str):
    return str=='world'
def func3(str):
    return str!='method'
def func4(n):
    if n%3==0 or n%5==0:
        return True
    else:
        return False

print(list(filter(func1,['12', '-1', '0'])))
print(list(filter(func2,['hello', 'world'])))
print(list(filter(func3,['technology', 'method', 'technique'])))
print(list(filter(func4,range(20))))
def process(src, dst, num):
    if num==0:
        if src==dst:
            return True
    elif num>0:
        for i in range(len(src)):
            tmp = src[:i] + src[i + 1:]
            sign=process(tmp, dst, num - 1)
            if sign==True:
                return sign

src='abc'
dst='bac'
num=int(len(src)-len(dst))
print(process(src,dst,num))
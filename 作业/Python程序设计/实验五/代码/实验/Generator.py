def generate_triangles():
    i=1
    sum=0
    while True:
        sum+=i
        yield sum
        i+=1

def generate_triangles_under(n):
    g=generate_triangles()
    tmp=g.__next__()
    while tmp<n:
        print(tmp)
        tmp=g.__next__()


generate_triangles_under(100)
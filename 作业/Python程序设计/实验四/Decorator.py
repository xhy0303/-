from time import time
def timed(fn): #fn stands for function
 # 你的代码
    def count():
        start=time()
        fn()
        print(time()-start)
    return count


@timed
def test_plus():
    result = []
    for i in range(100000):
        result = result + [i]

@timed
def test_append():
    result = []
    for i in range(100000):
        result.append(i)

test_plus()
test_append()
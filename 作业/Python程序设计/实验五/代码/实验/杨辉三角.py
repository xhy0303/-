def print_yanghui_triangle(num):
    arr=[]#储存本行的杨辉三角
    up=[]#储存上一行杨辉三角
    for i in range(1,num+1):
        arr=list(range(i))#将arr初始化为以当前行数为长度的空列表
        for n in range(1,i+1):
            if n==1 or n==i:#每一行第一个与最后一个元素设置为1
                arr[n-1]=1
            elif i!=1:#其余各元素为上一行相同位置与其前一个位置元素之和
                arr[n-1]=up[n-1-1]+up[n-1]
        line=''#用line字符串储存每一行杨辉三角用于输出
        for n in range(1,i+1):
            line += str(arr[n - 1])#将每一个元素添加到line中
            if n!=i:#最后一个元素之前的每一个元素之后都添加空格
                line+=' '
        print('{:^100}'.format(line))#用format函数控制line以100个字符长度居中输出
        up=arr#输出之后将本行三角值储存到上一行中


print_yanghui_triangle(10)
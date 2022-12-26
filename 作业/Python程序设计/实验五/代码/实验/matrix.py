def matrix_dim(list):
    return [len(list),len(list[0])]

def mult_M_v(M,v):
    list=[]
    for i in range(len(M)):
        sum=0
        for n in range(len(v)):
            sum+=(v[n][0]*M[i][n])
        list.append([sum])
    return list

def transpose(M):
    list=[]
    for i in range(len(M[0])):
        tmp=[]
        for n in range(len(M)):
            tmp.append(M[n][i])
        list.append(tmp)
    return list

def largest_col_sum(M):
    max=0
    for i in range(len(M[0])):
        tmp=0
        for n in range(len(M)):
            tmp+=M[n][i]
        if i==0 or tmp>max:
            max=tmp
    return max

def switch_columns(M, i, j):
    for n in range(len(M)):
        M[n][i],M[n][j]=M[n][j],M[n][i]
    return M

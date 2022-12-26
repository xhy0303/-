import matrix

M=[[5,6,7],[0,-3,5]]
print(matrix.matrix_dim(M))
print(matrix.mult_M_v(M,[[1],[2],[3]]))
print((matrix.transpose(M)))
print(matrix.largest_col_sum(M))
print(matrix.switch_columns(M,1,2))
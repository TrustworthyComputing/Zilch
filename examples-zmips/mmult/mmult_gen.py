N = 4

B_str = [ [ 'move $r'+str(N*N+ i*N+(j+1))+', '+str(i*N+(j+1)) for j in range(N) ] for i in range(N) ] 

A_str = [ [ '$r'+str(i*N+(j+1)) for j in range(N) ] for i in range(N) ] 
for i in range(N):
	for j in range(N):
		print('move ' + A_str[i][j] + ', ' + A_str[i][j][2:])
print
	
B_str = [ [ '$r'+str(N*N+ i*N+(j+1)) for j in range(N) ] for i in range(N) ] 
for i in range(N):
	for j in range(N):
		print('move ' + B_str[i][j] + ', ' + A_str[i][j][2:])
print

A = [ [ i*N+(j+1) for j in range(N) ] for i in range(N) ] 
B = [ [ i*N+(j+1) for j in range(N) ] for i in range(N) ] 
# for r in A:
# 	print(r)
# print
# for r in B:
# 	print(r)
# print

R_str = [ [ 'mult $r'+str(N*N*2+1)+', ' for j in range(N) ] for i in range(N) ] 
R = [ [ 0 for j in range(N) ] for i in range(N) ] 

for i in range(len(A_str)):
	for j in range(len(B_str[0])):
		
		R_str[i][j] += str(A_str[i][0] + ', ' + B_str[0][j] + '\n')
		R[i][j] += A[i][0] * B[0][j]
		
		for k in range(1, len(B_str) - 1):
			R_str[i][j] += 'mult $r0, '+ str(A_str[i][k] + ', ' + B_str[k][j] + '\n')
			R_str[i][j] += 'add $r'+str(N*N*2+1)+', $r'+str(N*N*2+1)+', $r0\n'
			R[i][j] += A[i][k] * B[k][j]
		
		R_str[i][j] += 'mult $r0, '+ str(A_str[i][len(B_str)-1] + ', ' + B_str[len(B_str)-1][j] + '\n')
		R_str[i][j] += 'add $r'+str(N*N*2+1)+', $r'+str(N*N*2+1)+', $r0\n'
		R_str[i][j] += 'print $r'+str(N*N*2+1)+'\n'
		R[i][j] += A[i][len(B)-1] * B[len(B)-1][j]

for row in R_str:
	for r in row:
		print(r)
print('answer $r'+str(N*N*2+1))

print
for r in R:
	print(r)

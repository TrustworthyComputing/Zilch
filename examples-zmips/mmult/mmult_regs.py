import sys

N = 0
if len(sys.argv) > 1:
	N = int(sys.argv[1])
else:
	print('Expected dimension size')
	sys.exit(1)

PRINT = False

filename = 'mmult_'+str(N)+'x'+str(N)+'_noram' 
f = open(filename + '.zmips', 'w')

A = [ [ (i*N+(j+1))%10 + 1 for j in range(N) ] for i in range(N) ] 
A_str = [ [ '$r'+str(i*N+(j+1)) for j in range(N) ] for i in range(N) ] 
for i in range(N):
	for j in range(N):
		f.write('move ' + A_str[i][j] + ', ' + str(A[i][j]) + '\n')
		print('move ' + A_str[i][j] + ', ' + str(A[i][j]))
print

R = [ [ 0 for j in range(N) ] for i in range(N) ] 
R_str = [ [ 'mult $r'+str(N*N+1)+', ' for j in range(N) ] for i in range(N) ] 

for i in range(len(A_str)):
	for j in range(len(A_str[0])):
		R_str[i][j] += str(A_str[i][0] + ', ' + A_str[0][j] + '\n')
		R[i][j] += A[i][0] * A[0][j]
		for k in range(1, len(A_str) - 1):
			R_str[i][j] += 'mult $r0, '+ str(A_str[i][k] + ', ' + A_str[k][j] + '\n')
			R_str[i][j] += 'add $r'+str(N*N+1)+', $r'+str(N*N+1)+', $r0\n'
			R[i][j] += A[i][k] * A[k][j]
		R_str[i][j] += 'mult $r0, '+ str(A_str[i][len(A_str)-1] + ', ' + A_str[len(A_str)-1][j] + '\n')
		R_str[i][j] += 'add $r'+str(N*N+1)+', $r'+str(N*N+1)+', $r0\n'
		if not PRINT:
			R_str[i][j] += '# '
		R_str[i][j] += 'print $r'+str(N*N+1)+'\n'
		R[i][j] += A[i][len(A)-1] * A[len(A)-1][j]

for row in R_str:
	for r in row:
		print(r)
		f.write(r)
print('answer $r'+str(N*N+1))
f.write('answer $r'+str(N*N+1))
f.close()

print
for r in R:
	print(r)

from __future__ import print_function
import sys

N = 0
if len(sys.argv) > 1:
	N = int(sys.argv[1])
else:
	print('Expected dimension size')
	sys.exit(1)
	
def mmult(tape):
	for i in range(N):
		for j in range(N):
			acc = 0
			for k in range(N):
				acc += tape[i*N + k] * tape[(k+N)*N + j]
			print(acc, end=' ')
		print()
	print()


def mmult_zmips(tape):
	prog = 'move $r10, ' + str(N) + '\n'
	prog += '__L0__\n'
	prog += '\tmove $r1, 0\n'
	prog += '\t__L1__\n'
	prog += '\t\tmove $r2, 0\n'
	prog += '\t\tmove $r3, 0\n'
	prog += '\t\tmove $r8, 0\n'
	prog += '\t\t__L2__\n'
	prog += '\t\t\tmult $r3, $r0, $r10\n'
	prog += '\t\t\tadd $r3, $r3, $r2\n'
	prog += '\t\t\tsecseek $r4, $r3\n'
	prog += '\t\t\tadd $r3, $r2, $r10\n'
	prog += '\t\t\tmult $r3, $r3, $r10\n'
	prog += '\t\t\tadd $r3, $r3, $r1\n'
	prog += '\t\t\tsecseek $r5, $r3\n'
	prog += '\t\t\tmult $r4, $r4, $r5\n'
	prog += '\t\t\tadd $r8, $r8, $r4\n'
	prog += '\t\t\tadd $r2, $r2, 1\n'
	prog += '\t\tblt $r2, $r10, __L2__\n'
	prog += '\t\tprint $r8\n'
	prog += '\t\tadd $r1, $r1, 1\n'
	prog += '\tblt $r1, $r10, __L1__\n'
	prog += '\tprintln $0\n'
	prog += '\tadd $r0, $r0, 1\n'
	prog += 'blt $r0, $r10, __L0__\n'
	prog += 'answer $r1\n'
	
	filename = 'mmult_'+str(N)+'x'+str(N)+'_tape' 
	f = open(filename + '.zmips', 'w')
	f.write(prog)
	f.close()
	f = open(filename + '.auxtape', 'w')
	for i in tape:
		f.write(str(i)+'\n')
	f.close()



# initialize with two identical arrays
tape = [ i+1 for i in range(N*N) ] * 2
print(tape, end='\n\n')

mmult(tape)

mmult_zmips(tape)



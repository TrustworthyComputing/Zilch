#include <stdio.h>

#define N 3

void multiply(int mat1[N*N], int mat2[N*N], int res[N*N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			res[i*N+j] = 0;
			for (int k = 0; k < N; k++) {
                int t = mat1[i*N+k] * mat2[k*N+j];
                res[i*N+j] += t;
            }
		}
	}
}

int main(void) { 
	int mat1[N*N] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int mat2[N*N] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int res[N*N];

	multiply(mat1, mat2, res);

	printf("Result matrix is \n"); 
	for (int i = 0; i < N; i++) { 
		for (int j = 0; j < N; j++) {
            printf("%d ", res[i*N+j]);
        }
		printf("\n"); 
	} 

	return 0; 
} 


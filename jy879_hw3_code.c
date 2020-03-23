/*HW4 Jingwen Ye jy879
	openMP inverts amatrix via Gaussian elimination
	compile with: gcc jy879_hw3_code.c -o  hw3 -fopenmp -lm
	run with: ./hw3
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>           /* clock_gettime lib */
#include <omp.h>
#include <math.h>

#define BILLION 1000000000L
#define NUM_THREADS 1 //1,2,4,8,16
#define N 4096 //1024, 2048,4096

void swaprow_p(double **A, double **I, int r);
void swaprow_s(double **A, double **I, int r);
void update_s(double **A, double **I, int i, int j);
void backsub(double **A, double **I, double **X, int i, int j);
void matrix(double **A, double **I, double **X);

int maxr = 0;
double **A;
double **I;
double **A_i;
double **I_i;
double **X;
double **R;
static omp_lock_t lock;

int main() {
	// parameter for timing
	struct timespec start, end;
	uint64_t diff = 0;
	int i, j;
	// array initialized
	A = (double **)malloc(N * sizeof(double *));
	I = (double **)malloc(N * sizeof(double *));
	A_i = (double **)malloc(N * sizeof(double *));
	I_i = (double **)malloc(N * sizeof(double *));
	X = (double **)malloc(N * sizeof(double *));
	R = (double **)malloc(N * sizeof(double *));
	for (i = 0; i < N; i++) {
		A[i] = (double *)malloc(N * sizeof(double));
		I[i] = (double *)malloc(N * sizeof(double));
		A_i[i] = (double *)malloc(N * sizeof(double));
		I_i[i] = (double *)malloc(N * sizeof(double));
		X[i] = (double *)malloc(N * sizeof(double));
		R[i] = (double *)malloc(N * sizeof(double));
	}
	//fulfill A, I, X with random Number
	srand48(1);
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			A[i][j] = drand48();
			I[i][j] = 0.0;
			A_i[i][j] = A[i][j];
			I_i[i][j] = I[i][j];
			X[i][j] = 0.0;
			R[i][j] = 0.0;
		}
	}
	for (i = 0; i < N; i++) {
		I[i][i] = 1;
		I_i[i][i] = 1;
	}

	clock_gettime(CLOCK_MONOTONIC, &start);//timer start
	for (i = 0; i < N - 1; i++) {
		swaprow_p(A, I, i);
		#pragma omp parallel shared(A,I,i) private(j)  num_threads(NUM_THREADS)
			#pragma omp for schedule(dynamic, 64) nowait
			for (j = i+1; j < N; j++) {
				update_s(A, I, i, j);
			}

	}

	for (i = N - 1; i >= 0; i--) {
		#pragma omp parallel shared(A,I,i,X) private(j)  num_threads(NUM_THREADS)
			#pragma omp for schedule(dynamic, 64) nowait		
		for(j = 0; j < N; j++){
			X[i][j] = I[i][j]/A[i][i];
		}	
		for (j = i - 1; j >= 0; j--) {
			backsub(A, I, X, i, j);
		}
	}

/*	for (i =0 ; i < N; i++){
		printf("| ");
		for (j = 0; j < N; j++){
			printf("%5.2e ",X[i][j]);		
		}	
		printf("\n");
	}
*/
	clock_gettime(CLOCK_MONOTONIC, &end);//timer end
	diff = diff + BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("Time elapsed to find all pivots in entire matrix SEQ is %15llu ns\n", (long long unsigned)diff);

//	matrix(A, I, X);

	free(A);
	free(I);
	free(X);
	free(R);

	return 0;
}

void swaprow_p(double **A, double **I, int r) {
	int i = 0;
	int r_max = r;
	double temp;
	double temp1;
	omp_init_lock(&lock);
	#pragma omp parallel shared(A,r_max,r,maxr) private(i)  num_threads(NUM_THREADS)
		#pragma omp for schedule(dynamic, 64) nowait
		for (i = r; i < N; i++){
			if (abs(A[i][r]) > abs(A[r_max][r])){
				r_max = i;
			}
		}

		omp_set_lock(&lock);
		if (abs(A[r_max][r]) > abs(A[maxr][r])) {
			maxr = r_max;
		}
		omp_destroy_lock(&lock);
	// do swap
	if (r != maxr) {
		for (i=0; i < N; i++){
			temp = A[r][i];
			A[r][i] = A[maxr][i];
			A[maxr][i] = temp;
	
			temp1 = I[r][i];
			I[r][i] = I[maxr][i];
			I[maxr][i] = temp1;
		}
	}
	return;
}

void swaprow_s(double **A, double **I, int r) {
	int i = 0;
	int r_max = r;
	double temp;
	double temp1;
	for (i = r; i < N; i++){
		if (abs(A[i][r]) > abs(A[r_max][r])){
			r_max = i;
		}
	}
	if (r != r_max) {
		for (i=0; i < N; i++){
			temp = A[r][i];
			A[r][i] = A[r_max][i];
			A[r_max][i] = temp;
	
			temp1 = I[r][i];
			I[r][i] = I[r_max][i];
			I[r_max][i] = temp1;
		}
	}	
	return;
}

void update_s(double **A, double **I, int i, int j) {
	double p;
	p = A[j][i] / A[i][i];
	int k;
	for (k = i; k < N; k++) {
		A[j][k] = A[j][k] - p * A[i][k];
	}
	int y;
	for (y = 0; y < N; y++) {
		I[j][y] = I[j][y] - p * I[i][y];
	}
}

void backsub(double **A, double **I, double **X, int i, int j) {
	int k;
	for (k = 0; k < N; k++) {
		I[j][k] = I[j][k] -A[j][i] * X[i][k];
	}
}

void matrix(double **A, double **I, double **X){
	int i,j,k;
	double nR = 0.0;
	double nA = 0.0;
	double nX = 0.0;
	double err = 0.0;

	for (i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			for (k = 0; k < N; k++){
				R[i][j] += A_i[i][k] * X[k][j];
			}
		R[i][j] = R[i][j]-I_i[i][j];
		nR = nR + R[i][j]*R[i][j];
		nA = nA + A_i[i][j]*A_i[i][j];
		nX = nX + X[i][j]*X[i][j];
		}
	}

	err = pow(nR,0.5)/(pow(nA,0.5)*pow(nX,0.5));
	printf("err is %e:\n", err);
}


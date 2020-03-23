/*HW1 Jingwen Ye jy879
	
	Problem 2 row by col
	
	compile with: gcc jy879_mm_rbyc.c -o rbyc
	run with: ./rbyc
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>           /* clock_gettime lib */

#define BILLION 1000000000L

int main(){

	int i,j,dim_n,k;
	// get value from user for dim_n
	printf("Give value dim_n = ?\n");
	scanf("%d",&dim_n);
	int temp = dim_n;
	int N = temp;

    //allocate memory for matrix a,b and sum c
    float **a = (float **)malloc(N*sizeof(float *));
    float **b = (float **)malloc(N*sizeof(float *));
    float **c = (float **)malloc(N*sizeof(float *));
	for (i=0; i<N; i++){
		a[i] = (float *)malloc(N*sizeof(float));
		b[i] = (float *)malloc(N*sizeof(float));
		c[i] = (float *)malloc(N*sizeof(float));			
	}

    //fulfill array with random Number
    srand48(1);
    for (i = 0;i < N;i++){
		for (j = 0; j < N ;j++){
            a[i][j] = drand48();
            b[i][j] = drand48();
			c[i][j] = 0;
			} 
    	}
	

    // timer setting
	uint64_t diff;
	struct timespec start, end;

	printf("test for dim_n=%d\n", N);
	
    // approach row by column
	clock_gettime(CLOCK_MONOTONIC,&start);//timer start

	for (i = 0; i<N; i++){
		for (j=0; j<N; j++){
			for (k=0; k<N; k++){
				c[i][j] = c[i][j] + a[i][k]*b[k][j];
				}
			}
		}

	clock_gettime(CLOCK_MONOTONIC,&end);//timer end
	
	diff = diff + BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("row by col time = %llu\n",(long long unsigned int) diff);
	free(a);
	free(b);
	free(c);
		
	return 0;
}
    

    

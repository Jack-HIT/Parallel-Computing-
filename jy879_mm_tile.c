/*HW1 Jingwen Ye jy879
	
	Problem 2 tile
	
	compile with: gcc jy879_mm_tile.c -o tile

	run with: ./tile
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>           /* clock_gettime lib */

#define BILLION 1000000000L

int main(){

	int i,j,dim_n,tile_size,k,ii,jj,kk;
	// get value from user for dim_n
	printf("Give value dim_n = ?\n");
	scanf("%d",&dim_n);
	int temp = dim_n;
	int N = temp;
	
	// get value from user for tile_size
	printf("Give value tile_size = ?\n");
	scanf("%d",&tile_size);
	temp = tile_size;
	int B = temp;//subblocl size is B
    
    int p = N/B;//p*p blocks

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
	
    // approach tile
	clock_gettime(CLOCK_MONOTONIC,&start);//timer start
	
	for (ii = 0; ii<p; ii++){
		for (jj=0; jj<p; jj++){
			for (kk=0; kk<p; kk++){
				for (i = 0; i<B; i++){
					for (j=0; j<B; j++){
						for (k=0; k<B; k++){
							c[i][j] = c[i][j] + a[i][k]*b[k][j];
							}
						}			
					}
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
    

    

/*Jingwen Ye (jy879)

hw1 Problem 1

compile with: gcc jy879_P1.c -o jy879_P1
run with: ./jy879_P1
*/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>           /* clock_gettime lib */
#include <math.h>           /* for 2^26 and 2^10 */

#define MAX_LENGTH pow(2,26)
#define MIN_SIZE pow(2,10)
#define BILLION 1000000000L
#define K 10

int main(){
    float* A;   //build array
    A = (float *)malloc(sizeof(float) * MAX_LENGTH);

    struct timespec start, end;
    float diff;
    int n = 1;
    int s = 1;  //stride
    int i = 0;
    float load;
    int k = 0;
    FILE *fp;
    fp = fopen("P1result.txt", "w+");

    for (i = 1;i <= MAX_LENGTH;i++){
	A[i] = i;
	} 
    for (n = MIN_SIZE; n <= MAX_LENGTH; n = n*2){ //double length
        for (s = 1;s <= (n/2);s = s*2){ //double stride
            clock_gettime(CLOCK_MONOTONIC, &start); //timer begin
            for (k = 0;k < (s*K);k++){
                for (i = 0;i < n-1;i+=s){
                    load = A[i];
                }
            }
            clock_gettime(CLOCK_MONOTONIC, &end); //timer stop
            diff = (BILLION * ((float)end.tv_sec - (float)start.tv_sec) + (float)end.tv_nsec - (float)start.tv_nsec)/(n*K);
            printf("n = %d, s = %d, time = %f ns\n",n, s, diff);
	    fprintf(fp, "%d, %d, %2.4lf\n", n, s, diff);
	}
    }
    free(A);
    fclose(fp);
    printf("Finish\n");
    return 0;
}

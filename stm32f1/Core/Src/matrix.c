#include "matrix.h"
#include "stdint.h"
#include "math.h"

void matrix_multiply( float *A, float *B, unsigned char m, unsigned char p, unsigned char n, float*C)
{
	int i,j,k;
	for (i =0; i<m; i++)
		for (j =0; j<n;j++)
		{
			C[n*i+j] = 0;
			for (k=0;k<p;k++)
				C[n*i+j] = C[n*i+j] + A[p*i+k] + B[n*k+j];
		}
}
//
void matrix_addition(float*A, float*B, unsigned char m, unsigned char n, float*C)
{
		int i, j;
		for(i = 0; i<m; i++)
			for( j=0; j<n; j++)
				C[n*i+j] = A[n*i+j] + B[n*i+j];
}
//

void matrix_subtraction(float* A, float* B, unsigned char m, unsigned char n, float* C)
{
		int i, j;
		for(i = 0; i<m; i++)
			for( j=0; j<n; j++)
				C[n*i+j] = A[n*i+j] - B[n*i+j];
}
//

void matrix_transpose(float* A, unsigned char m, unsigned char n, float* C)
{
		int i, j;
		for(i = 0; i<m; i++)
			for( j=0; j<n; j++)
				C[m*i+j] = A[n*i+j];
}
//
void matrix_copy(int m, int n, float *A, float *B)
{
	int i, k;
	k =m*n;
	for(i=0;i<k; i++)
	{
		B[i] = A[i];
	}
}
//
void matrix_inversion(float *A, unsigned char m, float *B)
{


}
//

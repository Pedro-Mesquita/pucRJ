#include <time.h>

#ifndef MATRIX_LIB
#define MATRIX_LIB

typedef struct {
	unsigned long int rows;
	unsigned long int cols;
	float *values;
} matrix;

int scalar_matrix_mult(float scalar_value, matrix *m, matrix *r);
int matrix_matrix_mult(matrix *m1, matrix *m2, matrix *r);
int load_matrix(matrix *matrix, char *filename);
int store_matrix(matrix *matrix, char *filename);
void usage(const char *name);
int check_linear_errors(matrix *source, matrix *destination, float scalar_value);
int check_mult_errors(matrix *matA, matrix *matB, matrix *matC);

#define timedifference_msec(start,stop) (((double)((stop) - (start))) / CLOCKS_PER_SEC * 1000.0)

#endif
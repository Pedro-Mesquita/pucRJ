/** 
 * @file matrix_lib_test.c
 * @brief Test program for the matrix library
 * @author Alexandre Meslin
 * @date 2026-08-26
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <math.h>

#include "matrix_lib.h"

/**
 * @brief main function of the program
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 if the program executed successfully, 1 otherwise
 * 
 * This program performs matrix operations, including scalar multiplication and matrix multiplication. 
 * It reads matrices from files, performs the specified operations, and writes the results to output files. 
 * The program also checks for errors in the results.
 */
int main(int argc, char *argv[]) {
    // dados da matriz
    float scalar_value;
    long dim1_rows;
    long dim1_cols;
    long dim2_rows;
    long dim2_cols;
    char *matrix1_filename;
    char *matrix2_filename;
    char *result1_filename;
    char *result2_filename;

    // as matrizes
    matrix matrix1;
    matrix matrix2;
    matrix result1;
    matrix result2;

    // time
    clock_t start;
    clock_t stop;

    // opções
    char short_options[] = "s:r:c:C:m:M:o:O:";
    struct option long_options[] = {
        {"scalar", 1, NULL, 's'},
        {"rows1", 1, NULL, 'r'},
        {"cols1", 1, NULL, 'c'},
        {"cols2", 1, NULL, 'C'},
        {"matrix1", 1, NULL, 'm'},
        {"matrix2", 1, NULL, 'M'},
        {"output1", 1, NULL, 'o'},
        {"output2", 1, NULL, 'O'},
        {NULL, 0, NULL, 0}
    };
    char ch;

    // Check the number of arguments
    if(argc != 17) {
        usage(argv[0]);
        exit(1);
    }

    // Parse command line arguments
    while((ch=getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch(ch) {
            case 's':
                scalar_value = strtof(optarg, NULL);
                break;
            case 'r':
                dim1_rows = strtol(optarg, NULL, 10);
                break;
            case 'c':
                dim2_rows = dim1_cols = strtol(optarg, NULL, 10);
                break;
            case 'C':
                dim2_cols = strtol(optarg, NULL, 10);
                break;
            case 'm':
                matrix1_filename = (char *) malloc(strlen(optarg)+1);
                strcpy(matrix1_filename, optarg);
                break;
            case 'M':
                matrix2_filename = (char *) malloc(strlen(optarg)+1);
                strcpy(matrix2_filename, optarg);
                break;
            case 'o':
                result1_filename = (char *) malloc(strlen(optarg)+1);
                strcpy(result1_filename, optarg);
                break;
            case 'O':
                result2_filename = (char *) malloc(strlen(optarg)+1);
                strcpy(result2_filename, optarg);
                break;
            default:
                printf("[%d] %s: unrecognized option '%c'\n", __LINE__, argv[0], ch);
                usage(argv[0]);
                exit(2);
        }
    }
    if(!matrix1_filename || !matrix2_filename || !result1_filename || !result2_filename) {
        printf("[%d] Memory allocation error\n", __LINE__);
        exit(3);
    }
    if(!scalar_value || !dim1_cols || !dim1_rows || !dim2_cols || !dim2_rows) {
        fprintf(stderr, "[%d] %s: erro na conversao do argumento: errno = %d\n", __LINE__, argv[0], errno);

        /* If a conversion error occurred, display a message and exit */
        if (errno == EINVAL) {
            fprintf(stderr, "[%d] Conversion error occurred: %d\n", __LINE__, errno);
            return 1;
        }
  
        /* If the value provided was out of range, display a warning message */
        if (errno == ERANGE) {
            fprintf(stderr, "The value provided was out of rangei: %d\n", errno);
            return 1;
        }
        exit(4);
    }
    if(dim1_cols%8 || dim1_rows%8 || dim2_cols%8 || dim2_rows%8) {
        fprintf(stderr, "[%d] Some dimension(s) of one of the matrices is not a multiple of 8\n", __LINE__);
        return 12;
    }
    printf("[%d] Starting...\n", __LINE__);

    // allocate memory arrays for the 4 matrices
    float *m1 = (float *) aligned_alloc(32, sizeof(float)*dim1_rows*dim1_cols);
    float *m2 = (float *) aligned_alloc(32, sizeof(float)*dim2_rows*dim2_cols);
    float *r1 = (float *) aligned_alloc(32, sizeof(float)*dim1_rows*dim1_cols);
    float *r2 = (float *) aligned_alloc(32, sizeof(float)*dim1_rows*dim2_cols);

    if(!m1 || !m2 || !r1 || !r2) {
        fprintf(stderr, "[%d] %s: array allocation problem.\n", __LINE__, argv[0]);
        exit(5);
    }

    // Initialize the 4 matrices
    // 1st matrix
    printf("[%d] Reading first matrix\n", __LINE__);
    matrix1.rows = dim1_rows;
    matrix1.cols = dim1_cols;
    matrix1.values = m1;
    if (load_matrix(&matrix1, matrix1_filename)) {
        fprintf(stderr, "[%d] %s: matrix 1 initialization problem.\n", __LINE__, argv[0]);
        exit(6);
    }

    // 2nd matrix
    printf("[%d] Reading second matrix\n", __LINE__);
    matrix2.rows = dim2_rows;
    matrix2.cols = dim2_cols;
    matrix2.values = m2;
    if (load_matrix(&matrix2, matrix2_filename)) {
        fprintf(stderr, "[%d] %s: matrix 2 initialization problem.\n", __LINE__, argv[0]);
        exit(7);
    }

    // 1st result
    result1.rows = dim1_rows;
    result1.cols = dim1_cols;
    result1.values = r1;

    // 2nd result
    result2.rows = dim1_rows;
    result2.cols = dim2_cols;
    result2.values = r2;

    /* 
     * Scalar product of matrix A 
     */
    printf("[%d] Executing scalar_matrix_mult(%f, matrixA)...\n", __LINE__, scalar_value);
    printf("[%d] Multiplying scalar %f by matrix %ldx%ld resulting matrix %ldx%ld\n", __LINE__, scalar_value, matrix1.rows, matrix1.cols, result1.rows, result1.cols);
    start = clock();
    if (scalar_matrix_mult(scalar_value, &matrix1, &result1)) {
        fprintf(stderr, "[%d] %s: scalar_matrix_mult problem.\n", __LINE__, argv[0]);
        return 8;
    }
    stop = clock();
    printf("[%d] Scalar product: %f ms\n", __LINE__, timedifference_msec(start, stop));

      /* Write first result */
    printf("[%d] Writing first result: %s...\n", __LINE__, result1_filename);
    if(store_matrix(&result1, result1_filename)) {
	    fprintf(stderr, "%s: failed to write first result to file.", argv[0]);
	    return 9;
    }

    /* Check for errors */
    printf("[%d] Checking matrix result for errors...\n", __LINE__);
    check_linear_errors(&matrix1, &result1, scalar_value);

    /* 
     * Calculate the product between matrix A and matrix B 
     */
    printf("[%d] Executing matrix_matrix_mult(matrixA, mattrixB, matrixC)...\n", __LINE__);
    printf("[%d] Multiplying matrix %ldx%ld by matrix %ldx%ld resulting matrix %ldx%ld\n", __LINE__, matrix1.rows, matrix1.cols, matrix2.rows, matrix2.cols, result2.rows, result2.cols);
    start = clock();
    if (matrix_matrix_mult(&matrix1, &matrix2, &result2)) {
        fprintf(stderr, "[%d] %s: matrix_matrix_mult problem.", __LINE__, argv[0]);
        return 10;
    }
    stop = clock();
    printf("[%d] *** Matrix product: %f ms\n", __LINE__, timedifference_msec(start, stop));

    /* Write second result */
    printf("[%d] Writing second result: %s...\n", __LINE__, result2_filename);
    if(store_matrix(&result2, result2_filename)) {
  	    fprintf(stderr, "%s: failed to write second result to file.", argv[0]);
	    return 11;
    }

    /* Check foor errors */
    printf("[%d] Checking matrix result for errors...\n", __LINE__);
    check_mult_errors(&matrix1, &matrix2, &result2);
    
    return 0;
}

/**
 * Shows how to use this program
 * 
 * @param name pointer to this program filename string
 */
void usage(const char *name) {
    printf("Usage: %s [OPTION]...\n", name);
    printf("Multiply matrix by scalar and by matrix\n");
    printf("-s --scalar     scalar value that will multiply the first matrix\n");
    printf("-r --rows       number of rows in the first matrix\n");
    printf("-c --cols1      number of columns in the first matrix or the number of rows in the second matrix\n");
    printf("-C --cols2      number of columns in the second matrix\n");
    printf("-m --matrix1    float file that will be used to load the first matrix\n");
    printf("-M --matrix2    float file that will be used to load the second matrix\n");
    printf("-o --output1    float file where the first result will be stored\n");
    printf("-O --output2    float file where the second result will be stored\n");
}

/**
 * Read matrix from secondary memory
 * 
 * @param matrix pointer to the matrix structure
 * @param filename filename string
 * @return 0 in case of success or the error code
 */
int load_matrix(matrix *matrix, char *filename) {
    unsigned long n;
    FILE *fd;
  
    /* Check the numbers of the elements of the matrix */
    n = matrix->rows * matrix->cols;
  
    /* Check the integrity of the matrix */
    if(n == 0 || matrix->values == NULL) return 1;
  
    /* Try to open file of floats */
    if((fd = fopen(filename, "rb")) == NULL) {
      fprintf(stderr, "[%d] Unable to open file '%s'\n", __LINE__, filename);
      return 2;
    }
  
    if(fread(matrix->values, sizeof(float), n, fd) != n) {
        fprintf(stderr, "[%d] Error reading from file '%s': short read (less than 8 floats)\n", __LINE__, filename);
        return 3;
    }
  
    if(fd != NULL) fclose(fd);
  
    return 0;
}

/**
 * @brief store the matrix at filename file
 * 
 * @param matrix matrix to be stored
 * @param filename file filename
 * @return 0 in case of success or the error code
 */
int store_matrix(matrix *matrix, char *filename) {
    unsigned long int n = 0;
    FILE *fd = NULL;
  
    /* Check the numbers of the elements of the matrix */
    n = matrix->rows * matrix->cols;
  
    /* Check the integrity of the matrix */
    if(n == 0 || matrix->values == NULL) return 1;
  
    /* Try to open file of floats */
    if((fd = fopen(filename, "wb")) == NULL) {
      fprintf(stderr, "[%d] Unable to open file %s\n", __LINE__, filename);
      return 2;
    }
  
    if(fwrite(matrix->values, sizeof(float), n, fd) != n) {
        fprintf(stderr, "[%d] Error writing to file %s: short write\n", __LINE__, filename);
        return 3;
    }
  
    if(fd != NULL) fclose(fd);
  
    return 0;
}

/**
 * @brief checks for linear multiplication error
 * 
 * @param source source matrix
 * @param destination destination matrix
 * @param scalar_value value scalar to multiply source matrix
 * @return 0 in case of success or the error code
 */
int check_linear_errors(matrix *source, matrix *destination, float scalar_value) {
    for(int line=0; line<source->rows; line++) {
        for(int row=0; row<source->cols; row++) {
            int pos = line*source->cols+row;
            if(fabs((scalar_value*source->values[pos]-destination->values[pos])/destination->values[pos]) > 0.0001) {
                fprintf(stderr, "[%d] Linear error at [%d, %d] - %f x %f\n", __LINE__, line, row, source->values[pos], destination->values[pos]);
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief checks for matrix multiplication error
 * 
 * @param matA 1st matrix
 * @param matB 2nd matrix
 * @param matC matrix resulted
 * @return 0 in case of success or the error code
 */
int check_mult_errors(matrix *matA, matrix *matB, matrix *matC) {
    // Loop para calcular cada elemento da matriz resultante
    for (int i = 0; i < matC->rows; i++) {
        for (int j = 0; j < matC->cols; j++) {
            float sum = 0.0;
            for (int k = 0; k < matA->cols; k++) {
                sum += matA->values[i * matA->cols + k] * matB->values[k * matB->cols + j];
            }
            if(fabs((matC->values[i * matC->cols + j]-sum)/sum) > 0.0001) {
                fprintf(stderr, "Multiplication error at [%d, %d] - %f x %f\n", i, j, matC->values[i * matC->cols + j], sum);
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief print a matrix
 * 
 * @param matrix matrix to be printed
 * @return 0 in case of success or the error code
 */
int print_matrix(matrix *matrix) {
    unsigned long int i;
    unsigned long int n;
    unsigned long int nxt_newLine;
  
    /* Check the numbers of the elements of the matrix */
    n = matrix->rows * matrix->cols;
  
    /* Check the integrity of the matrix */
    if(n==0 || matrix->values==NULL) return 0;
  
    /* Initialize new line controol */
    nxt_newLine = matrix->cols - 1;
  
    /* Print matrix elements */
    for(i=0; i<n; i++) {
        printf("%5.1f ", matrix->values[i]);
        if(i == nxt_newLine) {
            printf("\n");
            nxt_newLine += matrix->cols;
        }
        if(i == 255) {
            printf("Ooops...256 printing limit found...skipping printing...\n");
            break;
        }
    }
    printf("\n");
    return 0;
}

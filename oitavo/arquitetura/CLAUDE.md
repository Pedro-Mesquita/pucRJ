# Project Context

This repository contains **Trabalho 1** for the course **Introdução à Arquitetura de Computadores** at PUC-Rio.

The goal of the assignment is to optimize matrix operations written in C using **Intel Intrinsics**, specifically **AVX/FMA instructions**, and compare their performance against non-vectorized implementations studied in class.

The project runs on **Linux** and must be compiled using **GCC**.

The final implementation must preserve the interface and behavior expected by the professor-provided test program.

---

# Matrix Representation

Matrices use the following structure:

```c
typedef struct {
    unsigned long int rows;
    unsigned long int cols;
    float *values;
} matrix;
```

Important details:

* `rows` is a multiple of 8.
* `cols` is a multiple of 8.
* `values` stores the matrix in **row-major order**.
* A matrix element at row `i` and column `j` is accessed as:

```c
matrix->values[i * matrix->cols + j]
```

Because dimensions are multiples of 8, AVX instructions operating on 8 single-precision floats at once can be used without requiring special handling for remaining elements.

---

# Project Structure

Use the following organization:

```text
trabalho1/
├── matrix_lib.c
├── matrix_lib.h
├── matrix_lib_test.c
│
├── benchmarks/
│   ├── scalar/
│   │   ├── scalar_column.c
│   │   ├── scalar_row.c
│   │   ├── scalar_pointer.c
│   │   └── scalar_avx.c
│   │
│   └── matrix/
│       ├── matrix_conventional.c
│       ├── matrix_row_partial.c
│       └── matrix_avx_fma.c
│
├── data/
│   ├── inputs/
│   │   ├── 8x8/
│   │   ├── 16x16/
│   │   ├── 32x32/
│   │   ├── 64x64/
│   │   ├── 128x128/
│   │   ├── 256x256/
│   │   ├── 512x512/
│   │   ├── 1024x1024/
│   │   └── ...
│   │
│   └── outputs/
│
├── results/
│   ├── scalar_results.csv
│   ├── matrix_results.csv
│   └── graphs/
│
├── scripts/
│   ├── generate_matrices.py
│   ├── run_scalar_benchmarks.sh
│   ├── run_matrix_benchmarks.sh
│   └── generate_graphs.py
│
├── report/
│   └── relatorio.pdf
│
└── CLAUDE.md
```

This structure separates:

* the implementation that will be submitted;
* the historical/baseline implementations required for benchmarking;
* generated test data;
* benchmark results;
* graphs;
* automation scripts;
* the final report.

---

# Final Submission Implementation

The root file:

```text
matrix_lib.c
```

must always represent the **final optimized implementation intended for submission**.

It must contain:

```text
scalar_matrix_mult
→ Intel Intrinsics / AVX

matrix_matrix_mult
→ row-oriented partial sums + Intel Intrinsics / AVX/FMA
```

Do not permanently replace the root `matrix_lib.c` with a baseline implementation during benchmarking.

Baseline implementations belong under `benchmarks/`.

The professor requires only the following final deliverables:

```text
matrix_lib.c
report
```

Do not submit benchmark source files unless explicitly requested.

---

# Professor-Provided Files

The original project contains:

```text
matrix_lib.c
matrix_lib.h
matrix_lib_test.c
```

## Files that MUST NOT be modified

Do not modify:

```text
matrix_lib.h
matrix_lib_test.c
```

These files are provided by the professor and must remain compatible with the original implementation.

## File that may be modified

The final implementation must be contained in:

```text
matrix_lib.c
```

Only the implementations of:

```c
int scalar_matrix_mult(
    float scalar_value,
    struct matrix *matrix,
    struct matrix *result
);
```

and:

```c
int matrix_matrix_mult(
    struct matrix *matrixA,
    struct matrix *matrixB,
    struct matrix *matrixC
);
```

should be changed in the professor-provided library.

Auxiliary files under `benchmarks/`, `scripts/`, `data/`, and `results/` exist only to support development and performance analysis.

---

# Compilation

The final project must compile on Linux using:

```bash
gcc -Wall -std=c11 -mfma -o matrix_lib_test matrix_lib_test.c matrix_lib.c
```

Do not introduce dependencies that require changing the professor's compilation command.

The final program must compile without errors.

Warnings should also be avoided whenever possible.

---

# Part 1 — Scalar × Matrix

Implement:

```c
int scalar_matrix_mult(
    float scalar_value,
    struct matrix *matrix,
    struct matrix *result
);
```

The function calculates:

```text
result = scalar_value * matrix
```

for every element of the matrix.

The final version must use **Intel Intrinsics / AVX instructions**.

Since AVX registers are 256 bits wide and each `float` occupies 32 bits:

```text
256 / 32 = 8 floats
```

can be processed simultaneously.

The implementation should therefore process matrix elements in groups of 8 whenever appropriate.

---

# Scalar × Matrix Benchmark Implementations

Keep the required scalar multiplication implementations under:

```text
benchmarks/scalar/
```

The required versions are:

## 1. `scalar_column.c`

Non-vectorized implementation that indexes the matrix while traversing it **column by column**.

Conceptual access order:

```text
M[0][0]
M[1][0]
M[2][0]
...
M[0][1]
M[1][1]
...
```

Because the matrix is stored row-major, this version has poor spatial locality and should demonstrate the impact of cache behavior.

---

## 2. `scalar_row.c`

Non-vectorized implementation that indexes the matrix while traversing it **row by row**.

Conceptual order:

```text
M[0][0]
M[0][1]
M[0][2]
...
M[1][0]
M[1][1]
...
```

This implementation benefits from sequential memory access.

---

## 3. `scalar_pointer.c`

Non-vectorized implementation studied in class that traverses the linear `values` vector by incrementing a pointer or linear position.

The purpose is to avoid repeatedly calculating two-dimensional indexes.

---

## 4. `scalar_avx.c`

Optimized implementation using **Intel Intrinsics / AVX**.

It should process 8 consecutive `float` values whenever possible.

The root `matrix_lib.c` should use this strategy for its final `scalar_matrix_mult`.

---

# Scalar Benchmark Comparison

The report must compare:

```text
column traversal
vs
row traversal
vs
pointer increment
vs
AVX
```

It is NOT necessary to implement:

```text
column + AVX
row + AVX
```

as separate benchmark versions unless explicitly requested later.

---

# Part 2 — Matrix × Matrix

Implement:

```c
int matrix_matrix_mult(
    struct matrix *matrixA,
    struct matrix *matrixB,
    struct matrix *matrixC
);
```

The function calculates:

```text
C = A * B
```

The dimensions must satisfy:

```text
A.cols == B.rows
```

The resulting matrix dimensions are:

```text
A.rows × B.cols
```

The final implementation must use the optimized algorithm discussed in class together with **Intel Intrinsics / AVX/FMA**.

---

# Matrix × Matrix Benchmark Implementations

Keep the required matrix multiplication implementations under:

```text
benchmarks/matrix/
```

The required versions are:

## 1. `matrix_conventional.c`

Non-vectorized conventional matrix multiplication.

Conceptually:

```c
for (i = 0; i < A->rows; i++)
    for (j = 0; j < B->cols; j++)
        for (k = 0; k < A->cols; k++)
            C[i][j] += A[i][k] * B[k][j];
```

This multiplies a row from `A` by a column from `B`.

The access:

```text
B[k][j]
```

moves through a column of `B`.

Because `B` is stored row-major, this causes non-sequential memory access.

---

## 2. `matrix_row_partial.c`

Non-vectorized optimized version using **partial sums and row-oriented traversal**.

Conceptually:

```c
for (i = 0; i < A->rows; i++)
    for (k = 0; k < A->cols; k++)
        for (j = 0; j < B->cols; j++)
            C[i][j] += A[i][k] * B[k][j];
```

This reorganizes the loops so the inner loop traverses rows of `B` and `C` sequentially.

This version should demonstrate the performance impact of improving memory locality even without SIMD.

---

## 3. `matrix_avx_fma.c`

Optimized row-oriented implementation using **Intel Intrinsics / AVX/FMA**.

It should use the same partial-sum idea as `matrix_row_partial.c`, but vectorize the inner loop.

Conceptually:

```text
for each row i of A
    for each element k of row i
        broadcast A[i][k]
        process consecutive elements of B[k]
        update consecutive elements of C[i]
```

AVX should operate on groups of 8 `float` values.

FMA should be used when appropriate for operations equivalent to:

```text
C = A * B + C
```

The root `matrix_lib.c` should use this strategy for its final `matrix_matrix_mult`.

---

# Matrix Benchmark Comparison

The report must compare:

```text
conventional row × column
vs
row-oriented partial sums
vs
row-oriented partial sums + AVX/FMA
```

It is NOT necessary to implement a separate:

```text
conventional row × column + AVX
```

version unless explicitly requested later.

---

# Benchmark Input Data

Store generated binary matrix inputs under:

```text
data/inputs/
```

Possible test dimensions:

```text
8
16
32
64
128
256
512
1024
2048
...
```

All dimensions must remain multiples of 8.

For example:

```text
data/inputs/
├── 8x8/
├── 16x16/
├── 32x32/
├── 64x64/
├── 128x128/
├── 256x256/
├── 512x512/
└── 1024x1024/
```

Each directory may contain the binary files needed as matrices A and B.

Use the same input matrices when comparing implementations at a given size.

This is important for fair benchmarks and correctness validation.

---

# Matrix Sizes

Tests should range from very small matrices to matrices as large as the testing machine can reasonably execute.

Start with:

```text
8
16
32
64
128
256
512
1024
2048
```

Increase further when the machine allows it.

Scalar multiplication has linear complexity relative to the number of matrix elements and can probably be tested using substantially larger matrices.

Possible larger scalar benchmarks include:

```text
4096 × 4096
8192 × 8192
```

Matrix multiplication is significantly more expensive, so do not assume the same maximum size is practical.

Determine the largest reasonable matrix size through actual execution.

Do not fabricate results for sizes that were not tested.

---

# Output Data

Generated binary output matrices should be stored under:

```text
data/outputs/
```

Do not mix generated binary matrices with benchmark timing results.

The outputs are useful for validating that different implementations produce equivalent results.

---

# Benchmark Results

Store benchmark measurements under:

```text
results/
```

## Scalar results

Use:

```text
results/scalar_results.csv
```

Suggested format:

```csv
size,column,row,pointer,avx
8,...
16,...
32,...
64,...
128,...
256,...
```

Each timing value must come from an actual program execution.

---

## Matrix multiplication results

Use:

```text
results/matrix_results.csv
```

Suggested format:

```csv
size,conventional,row_partial,avx_fma
8,...
16,...
32,...
64,...
128,...
256,...
```

Do not invent or manually estimate benchmark times.

---

# Benchmark Repetitions

Execution times, especially for small matrices, can be noisy.

Whenever practical:

1. Run each implementation multiple times.
2. Record the measurements.
3. Use a consistent aggregation strategy, such as the mean or median.
4. Document the chosen strategy in the report.

Do not change the professor-provided timing mechanism inside `matrix_lib_test.c`.

If repeated executions are automated, they should be performed externally through scripts.

---

# Benchmark Scripts

Automation scripts should be stored under:

```text
scripts/
```

Suggested scripts:

## `generate_matrices.py`

Responsible for generating binary `float` input matrices for the required test sizes.

The generated files must use the binary format expected by `matrix_lib_test.c`.

---

## `run_scalar_benchmarks.sh`

Responsible for running the scalar multiplication benchmark implementations for all configured matrix sizes.

It should automate, where possible:

* compilation;
* execution;
* repeated measurements;
* collection of timing data.

It must not modify `matrix_lib_test.c`.

---

## `run_matrix_benchmarks.sh`

Responsible for benchmarking:

```text
matrix_conventional
matrix_row_partial
matrix_avx_fma
```

using equivalent inputs and compilation parameters.

---

## `generate_graphs.py`

Responsible for reading:

```text
results/scalar_results.csv
results/matrix_results.csv
```

and generating the figures required by the report.

Generated graphs should be stored under:

```text
results/graphs/
```

---

# Fair Benchmarking Rules

For performance comparisons:

1. Use the same machine for all implementations whenever possible.
2. Use the same compiler.
3. Use equivalent compilation options.
4. Use the same input matrices.
5. Use the same dimensions.
6. Use the same professor-provided timing code.
7. Do not modify `matrix_lib_test.c` between implementations.
8. Avoid background workloads that could heavily interfere with measurements.
9. Repeat measurements when necessary.
10. Document the hardware and environment used for the final measurements.

Do not make performance claims based only on theoretical expectations.

---

# Speedup

The report should calculate speedup using:

```text
speedup = baseline_execution_time / optimized_execution_time
```

Example:

```text
conventional implementation = 120 ms
AVX implementation          = 30 ms

speedup = 120 / 30 = 4
```

Therefore:

```text
speedup = 4×
```

Speedup should be evaluated across multiple matrix sizes.

Relevant comparisons may include:

```text
scalar column / scalar AVX
scalar row / scalar AVX
scalar pointer / scalar AVX

matrix conventional / matrix AVX-FMA
matrix row-partial / matrix AVX-FMA
```

Do not claim a speedup until the corresponding benchmark has actually been executed.

---

# Graphs

Store generated graphs under:

```text
results/graphs/
```

At minimum, the report should make the required implementation comparisons clearly visible.

A useful scalar graph should contain four series:

```text
column
row
pointer
AVX
```

A useful matrix multiplication graph should contain three series:

```text
conventional
row partial sums
AVX/FMA
```

Typical axes:

```text
X axis → matrix size
Y axis → execution time
```

Additional speedup graphs may also be created if they help explain the results.

Do not manipulate graph scales in a misleading way.

---

# Correctness

Performance optimization must NOT change numerical correctness.

Before benchmarking:

1. Verify scalar multiplication results.
2. Verify conventional matrix multiplication results.
3. Verify row-partial matrix multiplication results.
4. Verify AVX/FMA results.
5. Compare optimized outputs against a known-correct implementation.
6. Only analyze performance after correctness has been established.

Floating-point calculations may produce very small differences due to operation ordering and FMA behavior.

Do not use exact floating-point equality for independent implementations if operation ordering differs.

Use an appropriate tolerance when writing auxiliary validation tools.

Do not modify the professor-provided test program solely to accommodate an incorrect result.

---

# Timing

The professor-provided `matrix_lib_test.c` is responsible for measuring execution times.

Do NOT add timing logic inside:

```text
matrix_lib.c
```

All timing must remain in the `main` function of the test program, as required by the assignment.

Benchmark automation must execute the provided program and collect its reported timing externally.

---

# Error Handling

The assignment requires custom error codes to be documented in the report.

Validate relevant conditions such as:

* null matrix pointers;
* null `values` pointers;
* incompatible dimensions for matrix multiplication;
* invalid result dimensions, depending on the original library behavior;
* memory allocation failures, if allocation occurs inside the library.

Preserve any existing error-code conventions already defined in:

```text
matrix_lib.h
matrix_lib.c
```

Do not invent a new incompatible error-handling system before inspecting the original implementation.

Document all error codes used in the final report.

---

# Documentation

The assignment requires **Doxygen/Javadoc-style documentation**.

Public functions and important helpers should use comments similar to:

```c
/**
 * @brief Multiplies every element of a matrix by a scalar using AVX.
 *
 * @param scalar_value Scalar value.
 * @param matrix Input matrix.
 * @param result Output matrix.
 *
 * @return 0 on success or an appropriate error code.
 */
```

Document non-obvious AVX/FMA logic.

Avoid comments that merely repeat obvious C statements.

---

# Development Rules for Claude

When working on this repository:

1. Never modify `matrix_lib_test.c`.
2. Never modify `matrix_lib.h` unless I explicitly request it.
3. Keep the final assignment implementation in the root `matrix_lib.c`.
4. The root `matrix_lib.c` must contain the final AVX/FMA implementation.
5. Keep baseline benchmark implementations under `benchmarks/`.
6. Do not permanently overwrite `matrix_lib.c` with benchmark versions.
7. Preserve the function signatures required by the professor.
8. Preserve compatibility with the professor's GCC compilation command.
9. Use Intel Intrinsics for the final optimized arithmetic functions.
10. Prefer AVX/FMA and process 8 `float` elements per vector when applicable.
11. Take row-major memory layout and cache locality into account.
12. Do not add timing code to `matrix_lib.c`.
13. Do not modify timing logic in `matrix_lib_test.c`.
14. Do not fabricate benchmark results.
15. Do not fabricate speedups.
16. Benchmark data must come from actual executions.
17. Use equivalent inputs when comparing implementations.
18. Prioritize correctness before optimization.
19. Keep the code understandable enough to explain in a possible oral presentation.
20. Document important functions using Doxygen-style comments.
21. Do not over-engineer beyond the requirements of the assignment.
22. Keep generated inputs under `data/inputs/`.
23. Keep generated outputs under `data/outputs/`.
24. Keep timing results under `results/`.
25. Keep generated graphs under `results/graphs/`.
26. Keep benchmark automation under `scripts/`.
27. Do not treat auxiliary benchmark source files as final submission files.
28. When uncertain about a requirement, prefer the most conservative interpretation of the assignment.

---

# Required Performance Comparisons Summary

## Scalar × Matrix

Required implementations:

```text
1. column traversal, no SIMD
2. row traversal, no SIMD
3. pointer/vector increment, no SIMD
4. AVX
```

Files:

```text
benchmarks/scalar/
├── scalar_column.c
├── scalar_row.c
├── scalar_pointer.c
└── scalar_avx.c
```

---

## Matrix × Matrix

Required implementations:

```text
1. conventional row × column, no SIMD
2. row-oriented partial sums, no SIMD
3. row-oriented partial sums + AVX/FMA
```

Files:

```text
benchmarks/matrix/
├── matrix_conventional.c
├── matrix_row_partial.c
└── matrix_avx_fma.c
```

---

# Expected Final Deliverables

The professor requires only:

```text
matrix_lib.c
relatorio.pdf
```

Do NOT submit modified versions of:

```text
matrix_lib.h
matrix_lib_test.c
```

unless the professor explicitly changes the instructions.

The report should contain:

* description of the project;
* explanation of each relevant implementation;
* explanation of the optimization strategy;
* what worked;
* what did not work;
* limitations encountered;
* error codes used by the library;
* testing environment;
* processor information;
* compiler information;
* matrix sizes tested;
* benchmark methodology;
* execution times;
* graphs comparing implementations;
* calculated speedups;
* discussion of row-major memory access;
* discussion of cache locality;
* discussion of AVX;
* discussion of FMA;
* interpretation of measured results;
* final conclusions.

All conclusions about performance must be based on actual measurements.

# Project Context

This repository contains **Trabalho 1** for the course **Introdução à Arquitetura de Computadores** at PUC-Rio.

The goal of the assignment is to optimize matrix operations written in C using **Intel Intrinsics** (AVX; the assignment says "AVX/FMA" but the submitted code uses mul+add, see Decision Log), and compare their performance against non-vectorized implementations studied in class.

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
arquitetura/
├── matrix_lib.c              (entrega final: scalar AVX + matrix AVX mul+add)
├── matrix_lib.h              (professor, NAO modificar)
├── matrix_lib_test.c         (professor, NAO modificar)
├── .gitignore                (build/, data/inputs/* exceto 8x8, data/outputs/)
│
├── benchmarks/
│   ├── scalar/
│   │   ├── scalar_col.c        coluna por coluna, indexacao
│   │   ├── scalar_row.c        linha por linha, indexacao
│   │   ├── scalar_increment.c  ponteiros incrementais
│   │   └── scalar_avx.c        AVX, 8 floats por passo (= versao final)
│   │
│   └── matrix/
│       ├── matrix_conventional.c  linha x coluna (i -> j -> k)
│       ├── matrix_matrix.c        somas parciais em linha (i -> k -> j), sem SIMD
│       ├── matrix_avx.c           somas parciais + AVX mul+add (= versao final)
│       └── matrix_avx_fma.c       somas parciais + FMA (SO para comparacao no relatorio)
│
├── data/
│   ├── inputs/<R>x<C>/A.dat,B.dat   (quadradas NxN; nao quadradas RxC: A e RxC, B e CxR)
│   └── outputs/
│
├── results/
│   ├── iac2/                                   (OFICIAL: copia dos CSVs/logs medidos na IAC2)
│   ├── scalar_results.csv, scalar_raw.csv      (notebook, obsoleto; nao usar no relatorio)
│   ├── large_tests.csv, large_tests_matrix_bg.csv  (testes de escala no notebook, NAO sao benchmark)
│   └── graphs/
│
├── scripts/
│   ├── generate_matrices.py       gera A/B; aceita N ou RxC; escreve em blocos
│   ├── validate_results.py        compara saidas com numpy
│   ├── test_scalar_versions.sh    correcao + equivalencia das versoes escalares
│   ├── test_matrix_versions.sh    correcao + equivalencia das versoes de matriz
│   ├── run_large_tests.py         testes de correcao em escala (ate o limite da maquina)
│   ├── generate_matrices.c        gerador sem numpy (para a IAC2); mesmo formato
│   ├── bench_common.sh            funcoes comuns dos benchmarks (bash+awk, sem Python)
│   ├── run_scalar_benchmarks.sh   benchmark escalar: col row increment avx
│   ├── run_matrix_benchmarks.sh   benchmark de matrizes: conventional matrix avx [avx_fma]
│   └── (graficos: Pedro faz por conta propria; nao criar generate_graphs.py)
│
├── report/
│   └── relatorio.pdf              (a fazer)
│
└── CLAUDE.md
```

The file names above are the ones actually in the repository. Earlier drafts of
this document used `scalar_column.c`, `scalar_pointer.c`, `matrix_row_partial.c`
and `matrix_avx_fma.c` for the final AVX version; those names are obsolete.

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
→ Intel Intrinsics / AVX (same code as benchmarks/scalar/scalar_avx.c)

matrix_matrix_mult
→ row-oriented partial sums + Intel Intrinsics / AVX with
  _mm256_mul_ps + _mm256_add_ps (same code as benchmarks/matrix/matrix_avx.c)
```

**Decision (2026-09-19): the final `matrix_matrix_mult` does NOT use FMA.**
See "Decision Log" at the end of this file for the reasoning. Do not switch it
back to `_mm256_fmadd_ps` unless the professor explicitly confirms FMA is
expected.

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

## 1. `scalar_col.c`

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

## 3. `scalar_increment.c`

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

The final implementation must use the optimized algorithm discussed in class together with **Intel Intrinsics / AVX** (`_mm256_mul_ps` + `_mm256_add_ps`; no FMA — see Decision Log).

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

## 2. `matrix_matrix.c` (row-oriented partial sums)

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

## 3. `matrix_avx.c`

Optimized row-oriented implementation using **Intel Intrinsics / AVX**
(`_mm256_mul_ps` + `_mm256_add_ps`). This is the version that goes into the
final `matrix_lib.c`.

It uses the same partial-sum idea as `matrix_matrix.c`, but vectorizes the inner loop.

Conceptually:

```text
for each row i of A
    for each element k of row i
        broadcast A[i][k]
        process consecutive elements of B[k]
        update consecutive elements of C[i]
```

AVX operates on groups of 8 `float` values. The accumulation is done as a
separate multiply and add so that the result is bit-identical to the scalar
versions and to the reference inside `check_mult_errors()`.

The root `matrix_lib.c` uses this strategy for its final `matrix_matrix_mult`.

## 4. `matrix_avx_fma.c` (comparison only, not submitted)

Same as `matrix_avx.c` but with `_mm256_fmadd_ps`. Kept only to measure and
discuss FMA in the report (single rounding, ~12% faster at 2048 in `-O0`,
diverges from `check_mult_errors()` on near-zero elements). It is NOT the
submitted implementation.

---

# Matrix Benchmark Comparison

The report must compare:

```text
conventional row × column
vs
row-oriented partial sums
vs
row-oriented partial sums + AVX (mul+add)
```

Optionally add the FMA variant as a fourth series to support the FMA discussion.

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

Each directory contains `A.dat` and `B.dat`. For a non-square directory `RxC`,
`A` is R×C and `B` is C×R, so that `A × B` is well defined
(`matrix_lib_test -r R -c C -C R`). Non-square inputs exist for correctness
tests only; benchmarks use square matrices.

`generate_matrices.py` writes in row blocks (a 24000×24000 `uniform()` would
need a 4.6 GB float64 intermediate) and uses a fixed seed per size, so files
are reproducible byte-for-byte. Do not change the seed formula for square
sizes: the large inputs already on disk (up to 24000×24000, 11 GB) depend on it.

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

What is already known from actual runs on the test machine (i5-1335U, 15 GB
RAM, `-O0`), from `results/large_tests*.csv`:

* Scalar: all versions run and validate up to 10000×10000 in seconds.
  Inputs for 16384, 20000 and 24000 are generated (24000 needs ~7 GB in the
  test program) but were not run yet.
* Matrix product, row partial sums: 4096 → 149 s, 8192 → 1159 s (19 min).
  10000 was started and cancelled; extrapolation is ~35 min. Conventional
  will be much slower. The practical ceiling for the matrix benchmark is a
  time decision, not a memory one.

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
size,conventional,row_partial,avx
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
matrix_matrix       (row partial sums)
matrix_avx
matrix_avx_fma      (optional)
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

matrix conventional / matrix AVX
matrix row-partial / matrix AVX
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
AVX (mul+add)
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
4. Verify AVX results (and the FMA variant, if measured).
5. Compare optimized outputs against a known-correct implementation.
6. Only analyze performance after correctness has been established.

Floating-point calculations may produce very small differences due to operation ordering and FMA behavior.

Do not use exact floating-point equality for independent implementations if operation ordering differs.

Use an appropriate tolerance when writing auxiliary validation tools. For the
matrix product, the relative error must be measured against
`(|A| @ |B|)[i,j]` (sum of absolute terms), not against `|C[i,j]|`: with
inputs in [-1, 1] many `C[i,j]` are near zero by cancellation and the
relative error to `|C[i,j]|` explodes without any real error. Both
`validate_results.py` and `run_large_tests.py` already do this. The float64
numpy reference is the authoritative correctness criterion.

Known limitation of the professor's `check_mult_errors()`: it compares against
a sequential float32 mul+add sum with relative tolerance 1e-4 against
`|C[i,j]|`. Any implementation whose rounding differs from that reference
(e.g. FMA) triggers "Multiplication error" on near-zero elements for n ≥ 128
with random mixed-sign data, even though both are within float32 rounding of
the true value. The submitted mul+add versions are bit-identical to that
reference and pass it. Treat the professor's check as informational for
non-identical roundings; it is still the check the grader will see, which is
why the submitted code matches it exactly.

Bit-exactness results (verified with `cmp`): all four scalar versions produce
identical output; `matrix_conventional`, `matrix_matrix` and `matrix_avx`
produce identical output. Only `matrix_avx_fma` differs in bits.

For the scalar product the professor's check is O(n²) and fine at any size.
For the matrix product it is the naive O(n³) algorithm in `-O0` and takes
hours at 10000×10000; `run_large_tests.py` kills the process after
`result2` is written (line `[221]`) and validates with numpy instead. The
timing is printed before that point and is unaffected.

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

Error codes actually used (identical across all benchmark versions and the
final `matrix_lib.c`; every error also prints `[ERROR <line>] ...` to stderr):

```text
scalar_matrix_mult
  1  m->rows != r->rows
  2  m->cols != r->cols
  3  m or r is NULL
  4  m->values or r->values is NULL

matrix_matrix_mult
  1  m1->cols != m2->rows
  2  r->rows != m1->rows or r->cols != m2->cols
  3  m1, m2 or r is NULL
  4  values of m1, m2 or r is NULL
```

Checks are performed in the order 3, 4, 1, 2 (null pointers first). Keep new
code consistent with this table and document it in the report.

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

Document non-obvious AVX logic.

Avoid comments that merely repeat obvious C statements.

---

# Code Conventions (decided during development)

* Loop indices and dimension variables are `unsigned long int`, the same type
  as `rows`/`cols` in the struct. Do not use `int` for them (user decision).
* Parameter names follow `matrix_lib.h`: `(float scalar_value, matrix *m, matrix *r)`
  and `(matrix *m1, matrix *m2, matrix *r)`.
* AVX code uses `_mm256_loadu_ps` / `_mm256_storeu_ps` (no alignment
  requirement; no penalty on the 32-byte-aligned buffers from
  `aligned_alloc(32)` in the test program). Document this in a comment.
* AVX loops cover `n - n % 8` elements and are followed by a scalar remainder
  loop that never runs with the assignment's matrices; keep it, it is cheap
  and makes the function correct for any size.
* Every `.c` has a header comment with the error-code table and Doxygen
  comments on `scalar_matrix_mult` / `matrix_matrix_mult`. Comments are in
  Portuguese without accents (ASCII), like the rest of the code.
* Scalar benchmark files have a stub `matrix_matrix_mult` that returns 1 (the
  test program exits with code 10 after the scalar part — scripts rely on
  this). Matrix benchmark files have a stub `scalar_matrix_mult` that returns 0.
* Benchmarks and tests compile with `-Wall -std=c11 -mfma -O0 -I.`: `-O0` so
  that the measured difference comes from the written code, not from GCC
  auto-vectorization. Always the same flags for every version.

# Development Rules for Claude

When working on this repository:

1. Never modify `matrix_lib_test.c`.
2. Never modify `matrix_lib.h` unless I explicitly request it.
3. Keep the final assignment implementation in the root `matrix_lib.c`.
4. The root `matrix_lib.c` must contain the final AVX implementation (mul+add, no FMA).
5. Keep baseline benchmark implementations under `benchmarks/`.
6. Do not permanently overwrite `matrix_lib.c` with benchmark versions.
7. Preserve the function signatures required by the professor.
8. Preserve compatibility with the professor's GCC compilation command.
9. Use Intel Intrinsics for the final optimized arithmetic functions.
10. Prefer AVX and process 8 `float` elements per vector when applicable. Do not use FMA in the submitted code (see Decision Log).
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
29. Do not start long benchmark runs while functions are still being written; the agreed order is: implement everything → assemble `matrix_lib.c` → benchmark → graphs → report.
30. Long runs go in the background with output flushed (`python3 -u`), and must be cancellable; report only results that finished.
31. Generated inputs above 4096 are large (up to 2.3 GB per file). Delete large outputs after validating them (`run_large_tests.py` does this from 4096 up).

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
├── scalar_col.c
├── scalar_row.c
├── scalar_increment.c
└── scalar_avx.c
```

---

## Matrix × Matrix

Required implementations:

```text
1. conventional row × column, no SIMD
2. row-oriented partial sums, no SIMD
3. row-oriented partial sums + AVX (mul+add)   ← submitted
(4. row-oriented partial sums + FMA, optional, comparison only)
```

Files:

```text
benchmarks/matrix/
├── matrix_conventional.c
├── matrix_matrix.c          (row partial sums)
├── matrix_avx.c             (final AVX version, mul+add)
└── matrix_avx_fma.c         (FMA variant, comparison only)
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

---

# Decision Log

Decisions taken with the user during development, with the reasoning, so they
are not re-litigated.

## 2026-09-19 — File names

Files are named `scalar_col.c`, `scalar_row.c`, `scalar_increment.c`,
`scalar_avx.c`, `matrix_conventional.c`, `matrix_matrix.c`, `matrix_avx.c`,
`matrix_avx_fma.c` (user's choice). `run_scalar_benchmarks.sh` still has
`VERSOES=(column row pointer avx)` and must be updated to
`(col row increment avx)` when the benchmark phase starts.

## 2026-09-19 — `unsigned long int` for indices

First written as `unsigned long int`, changed to `int` on request, then
changed back on request ("siga os valores da struct"). Final: match the struct.

## 2026-09-19 — mul+add instead of FMA in the submitted `matrix_matrix_mult`

Facts established by measurement:

* `_mm256_fmadd_ps` is ~12% faster than `_mm256_mul_ps + _mm256_add_ps` at
  2048×2048 in `-O0` (4125–4261 ms vs 4641–4832 ms, 3 runs); equal within
  noise at 1024.
* Both are within float32 rounding of the float64 truth (verified on the
  flagged elements: FMA closer in 2 of 4 cases, farther in 2).
* FMA output differs in bits from the scalar versions; mul+add is
  bit-identical to them and to the reference in `check_mult_errors()`.
* With random inputs in [-1, 1], `check_mult_errors()` prints
  "Multiplication error" for the FMA version on ~1 near-zero element per
  matrix for n ≥ 128. Its own reference has relative error up to 9.5e-4 vs
  the true value there, above its 1e-4 tolerance; the criterion is simply
  too strict for cancelled elements.

What the assignment says: "instruções vetoriais (AVX/FMA) usando a biblioteca
Intel Intrinsics" and each function "deve usar as instruções vetoriais
disponibilizadas na biblioteca Intel Intrinsics"; compile with `-mfma`. It
does not mandate the FMA instruction; `-mfma` also implies `-mavx` in GCC and
may be there only to make 256-bit intrinsics compile.

Decision (user): submit mul+add. Zero risk of the grader seeing an error
message from the professor's own test program outweighs the ~12% and the
hint from the flag. Keep `matrix_avx_fma.c` as a measured variant and write
the FMA section of the report around this comparison ("what worked / what
did not"). If the professor confirms FMA is expected, the change is one line
in `matrix_lib.c` and `matrix_avx.c`.

## 2026-09-19 — Scale tests cancelled

`run_large_tests.py` was run for scalar (8 → 10000, all three non-AVX
versions, all exact) and matrix row-partial (8 → 8192 validated; 10000
cancelled by the user). Findings worth keeping for the report, not yet
benchmarked with repetitions:

* `scalar_col` at 8192 (3648 ms) was slower than at 10000 (1713 ms): a
  32 KiB stride (8192 × 4 B) maps every element of a column to the same cache
  set (power-of-two aliasing).
* `scalar_increment` beat `scalar_row` up to 2048 but lost at 4096/8192 in a
  single run; at those sizes the loop is memory-bound and one run is noisy.
  Let the median of repetitions decide.

Data from these runs is in `results/large_tests.csv` and
`results/large_tests_matrix_bg.csv`; it is not benchmark data.

## 2026-09-19 — Benchmark machine: IAC2, not the notebook

All report numbers come from `iac2.inf.puc-rio.br` (i5-12400, 6 P-cores,
L2 1.25 MB/core, L3 18 MB, 31 GB RAM, gcc 13.3, Ubuntu 24.04, idle when
measured). It has no numpy/pip/venv, so everything that runs there is bash +
C: `scripts/generate_matrices.c`, `bench_common.sh`, `run_*_benchmarks.sh`,
and the test scripts fall back to the professor's checks when `import numpy`
fails. Pedro types every remote command himself; Claude only gives the
commands. Results are copied back with rsync into `results/iac2/`.

Correctness on IAC2 (test_scalar_versions / test_matrix_versions, professor's
checks): all versions pass; all scalar versions bit-identical; conventional,
row-partial and AVX (mul+add) bit-identical.

### Scalar benchmark on IAC2 — DONE (results/iac2/scalar_results.csv)

Sizes 8 → 40000 (16 sizes, 5 reps each, 321 raw rows). Medians in ms at 40000:
col 27811, row 3946, increment 2805, avx 1792 → speedups vs AVX 15.5×, 2.2×,
1.57×. Findings for the report:

* Power-of-two aliasing in `col`: 8192 (1477 ms) is 2× slower than 10000
  (732 ms); 16384 (6304) vs 20000 (4043); 32768 (27078) ≈ 40000 (27811).
* Below 512 all four versions tie (L1/L2, fixed overhead dominates); 1024 has
  a noise inversion (row < avx) — mention, do not hide.
* AVX vs increment only ~1.6×: scalar product is memory-bound and `-O0`
  spills every `__m256` to the stack.

### Matrix benchmark on IAC2 — DONE (results/iac2/matrix_results.csv)

Repetitions: 5 for 8 → 2048; 3 for 4096; 8192: conventional 1 (96 min per
run), row-partial 3, avx 3; 16384: row-partial 1 (76 min) and avx 1 (31 min),
conventional not measured (~14 h). Logs: matrix_bench_1..4.log, 0 warnings.

Medians (ms): 2048 → 66037 / 9271 / 3713; 4096 → 647077 / 73946 / 29197;
8192 → 5764321 / 580371 / 233557; 16384 → — / 4541246 / 1878897
(conventional / row-partial / avx).

Speedups: conv/avx 17.8× (2048), 22.2× (4096), 24.7× (8192) — still growing
with N because the conventional keeps degrading (8.9× per doubling vs the
ideal 8×). row-partial/avx flat at ~2.5× from 1024 up (2.4× at 16384).
conv/row-partial 7.1× → 8.8× → 9.9×. Row-partial and AVX scale 7.8–8.0× per
doubling all the way to 16384 (ideal N³).

Findings for the report:

* Conventional jumps 21× from 512 to 1024 (N³ predicts 8×): B (4 MB) leaves
  L2, column stride 4 KiB misses every access; 10.4× from 1024 to 2048 (B =
  16 MB ≈ L3), 9.8× and 8.9× after that.
* Most of the gain is loop order, not SIMD: row-partial alone is 7–10× over
  conventional; AVX adds a further 2.5× (compute-bound, unlike the scalar
  product where AVX gave only 1.6×).
* Below 128 conventional ties or wins (all in L1, register accumulation); at
  8×8 AVX is slowest (setup cost).
* The ceiling for the matrix product is time, not memory (16384 conventional
  would take ~14 h); the scalar ceiling (40000) is memory (~19 GB touched of
  31 GB). State both in the report.

Benchmark phase is finished. Remaining: graphs (Pedro does them himself from
results/iac2/*.csv) and the report.

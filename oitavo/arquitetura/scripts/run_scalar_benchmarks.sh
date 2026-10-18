#!/usr/bin/env bash
#
# Benchmark de scalar_matrix_mult: coluna x linha x ponteiro x AVX.
#
# Uma unica execucao mede todos os tamanhos de TAMANHOS (bench_common.sh,
# 8 ate 4000 x 4000), REPS repeticoes cada; entradas que faltarem sao
# geradas. Os CSVs sao refeitos do zero a cada execucao.
#
# Saidas:
#   results/scalar_raw.csv      uma linha por execucao (todas as repeticoes)
#   results/scalar_results.csv  mediana por tamanho, uma coluna por versao
#
# Uso:
#   scripts/run_scalar_benchmarks.sh
#   REPS=3 scripts/run_scalar_benchmarks.sh    # muda as repeticoes (padrao 10)
#
# Nas versoes escalares matrix_matrix_mult e stub (retorna 1): o programa sai
# com codigo 10 logo depois de imprimir o tempo do escalar -- esperado.

set -uo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"
source scripts/bench_common.sh

[ $# -eq 0 ] || { echo "uso: $0   (os tamanhos sao fixos em scripts/bench_common.sh)" >&2; exit 1; }

VERSOES=(col row increment avx)
RAW=results/scalar_raw.csv
mkdir -p build data/outputs results

for n in "${TAMANHOS[@]}"; do garantir_entrada "$n"; done

echo "== compilando ($CFLAGS)"
for v in "${VERSOES[@]}"; do
    gcc $CFLAGS -o "build/test_$v" matrix_lib_test.c "benchmarks/scalar/scalar_$v.c" || exit 1
done

echo "funcao,versao,size,rep,tempo_ms" > "$RAW"
for n in "${TAMANHOS[@]}"; do
    echo "== ${n}x${n}  ($(date +%H:%M:%S))"
    for v in "${VERSOES[@]}"; do
        tempos=()
        for rep in $(seq 1 "$REPS"); do
            t=$(rodar_uma_vez "build/test_$v" "$n" "${v}_${n}" "Scalar product" 0)
            if [ -z "$t" ]; then echo "   $v rep $rep: SEM TEMPO (execucao falhou)" >&2; continue; fi
            echo "scalar_matrix_mult,$v,$n,$rep,$t" >> "$RAW"
            tempos+=("$t")
        done
        printf '   %-10s mediana %12s ms  (%s)\n' "$v" "$(printf '%s\n' "${tempos[@]}" | mediana)" "${tempos[*]}"
    done
done

agregar "$RAW" results/scalar_results.csv "${VERSOES[@]}"

#!/usr/bin/env bash
#
# Benchmark de matrix_matrix_mult: convencional x somas parciais x AVX
# (e, opcionalmente, a variante FMA, so para o relatorio).
#
# Uma unica execucao mede todos os tamanhos de TAMANHOS (bench_common.sh,
# 8 ate 4000 x 4000); entradas que faltarem sao geradas. Os CSVs sao
# refeitos do zero a cada execucao.
#
# Cada versao roda REPS vezes (padrao 10) em cada tamanho, como no escalar.
# Tempo total estimado na IAC2 a partir das medianas ja medidas: ~2 h 15,
# quase tudo em 4000 x 4000 (a convencional e ~85% disso).
# Rode em segundo plano, por exemplo:
#   nohup scripts/run_matrix_benchmarks.sh > results/matrix_bench.log 2>&1 &
#
# Saidas:
#   results/matrix_raw.csv      uma linha por execucao (todas as repeticoes)
#   results/matrix_results.csv  mediana por tamanho, uma coluna por versao
#
# Uso:
#   scripts/run_matrix_benchmarks.sh
#   VERSOES="conventional matrix avx avx_fma" scripts/run_matrix_benchmarks.sh
#   CHECK_ATE=512 ...   ate este tamanho o check_mult_errors() do professor roda
#                       inteiro; acima, o processo e encerrado assim que o check
#                       comeca (a corretude ja foi estabelecida pelos scripts de
#                       teste; o check convencional em -O0 levaria horas)

set -uo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"
source scripts/bench_common.sh

[ $# -eq 0 ] || { echo "uso: $0   (os tamanhos sao fixos em scripts/bench_common.sh)" >&2; exit 1; }

VERSOES=(${VERSOES:-conventional matrix avx})
CHECK_ATE="${CHECK_ATE:-512}"
RAW=results/matrix_raw.csv
mkdir -p build data/outputs results

for n in "${TAMANHOS[@]}"; do garantir_entrada "$n"; done

echo "== compilando ($CFLAGS)"
for v in "${VERSOES[@]}"; do
    gcc $CFLAGS -o "build/test_matrix_$v" matrix_lib_test.c "benchmarks/matrix/matrix_$v.c" || exit 1
done

echo "funcao,versao,size,rep,tempo_ms" > "$RAW"
for n in "${TAMANHOS[@]}"; do
    echo "== ${n}x${n}  ($(date +%H:%M:%S))"
    matar=0; [ "$n" -gt "$CHECK_ATE" ] && matar=1
    for v in "${VERSOES[@]}"; do
        tempos=()
        for rep in $(seq 1 "$REPS"); do
            t=$(rodar_uma_vez "build/test_matrix_$v" "$n" "matrix_${v}_${n}" "Matrix product" "$matar")
            if [ -z "$t" ]; then echo "   $v rep $rep: SEM TEMPO (execucao falhou)" >&2; continue; fi
            echo "matrix_matrix_mult,$v,$n,$rep,$t" >> "$RAW"
            tempos+=("$t")
        done
        printf '   %-13s mediana %14s ms  (%s)\n' "$v" "$(printf '%s\n' "${tempos[@]}" | mediana)" "${tempos[*]}"
    done
done

agregar "$RAW" results/matrix_results.csv "${VERSOES[@]}"

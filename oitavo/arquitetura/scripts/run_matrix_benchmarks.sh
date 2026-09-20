#!/usr/bin/env bash
#
# Benchmark de matrix_matrix_mult: convencional x somas parciais x AVX
# (e, opcionalmente, a variante FMA, so para o relatorio).
#
# Saidas:
#   results/matrix_raw.csv      uma linha por execucao (todas as repeticoes)
#   results/matrix_results.csv  mediana por tamanho, uma coluna por versao
#
# Uso:
#   scripts/run_matrix_benchmarks.sh 8 16 32 64 128 256 512 1024 2048
#   REPS=3 scripts/run_matrix_benchmarks.sh 4096
#   VERSOES="conventional matrix avx avx_fma" scripts/run_matrix_benchmarks.sh 1024
#   CHECK_ATE=512 ...   ate este tamanho o check_mult_errors() do professor roda
#                       inteiro; acima, o processo e encerrado assim que o check
#                       comeca (a corretude ja foi estabelecida pelos scripts de
#                       teste; o check convencional em -O0 levaria horas)
#
# Os tamanhos passados sao ACRESCENTADOS ao raw existente (para poder rodar os
# grandes em etapas); apague results/matrix_raw.csv para comecar do zero.
# Etapas com repeticoes diferentes por versao:
#   VERSOES="conventional" REPS=1 scripts/run_matrix_benchmarks.sh 8192
#   VERSOES="matrix avx"   REPS=3 scripts/run_matrix_benchmarks.sh 8192

set -uo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"
source scripts/bench_common.sh

VERSOES=(${VERSOES:-conventional matrix avx})
CHECK_ATE="${CHECK_ATE:-512}"
RAW=results/matrix_raw.csv
mkdir -p build data/outputs results

mapfile -t TAMANHOS < <(descobrir_tamanhos "$@")
[ ${#TAMANHOS[@]} -gt 0 ] || { echo "nenhum tamanho; passe-os na linha de comando" >&2; exit 1; }
for n in "${TAMANHOS[@]}"; do garantir_entrada "$n"; done

echo "== compilando ($CFLAGS)"
for v in "${VERSOES[@]}"; do
    gcc $CFLAGS -o "build/test_matrix_$v" matrix_lib_test.c "benchmarks/matrix/matrix_$v.c" || exit 1
done

[ -f "$RAW" ] || echo "funcao,versao,size,rep,tempo_ms" > "$RAW"
for n in "${TAMANHOS[@]}"; do
    echo "== ${n}x${n}  ($(date +%H:%M:%S))"
    # remove medicoes anteriores deste tamanho, SO das versoes que vao rodar
    # agora (permite medir o convencional com REPS=1 e as outras com REPS=3)
    awk -F, -v s="$n" -v vs="${VERSOES[*]}" 'BEGIN {split(vs, a, " "); for (i in a) v[a[i]]=1}
         NR==1 || $3!=s || !($2 in v)' "$RAW" > "$RAW.tmp" && mv "$RAW.tmp" "$RAW"
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

# agrega TODAS as versoes presentes no raw (nao so as desta rodada), em ordem fixa
PRESENTES=()
for v in conventional matrix avx avx_fma; do
    grep -q ",$v," "$RAW" && PRESENTES+=("$v")
done
agregar "$RAW" results/matrix_results.csv "${PRESENTES[@]}"

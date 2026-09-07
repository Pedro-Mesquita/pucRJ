#!/usr/bin/env bash
#
# Executa os benchmarks de scalar_matrix_mult e coleta os tempos.
#
# A medicao vem do proprio matrix_lib_test.c (fornecido pelo professor),
# que ja cronometra a chamada com clock(). Este script apenas executa o
# programa, captura o stdout e acumula os tempos -- nenhum arquivo do
# professor e alterado e nenhum clock() e adicionado a biblioteca.
#
# Saidas:
#   results/scalar_raw.csv      uma linha por execucao (todas as repeticoes)
#   results/scalar_results.csv  mediana por tamanho, uma coluna por versao
#
# Uso:
#   scripts/run_scalar_benchmarks.sh                  # tamanhos disponiveis
#   scripts/run_scalar_benchmarks.sh 1024 2048        # tamanhos escolhidos
#   REPS=10 scripts/run_scalar_benchmarks.sh          # muda as repeticoes

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

# -O0: sem otimizacao, para que a diferenca medida venha do codigo escrito
# e nao da auto-vetorizacao do GCC (com -O3 o compilador gera AVX sozinho
# na versao "row", o que invalidaria a comparacao). Mesma flag para todas.
CFLAGS="-Wall -std=c11 -mfma -O0 -I."
REPS="${REPS:-5}"
SCALAR="${SCALAR:-2.5}"

# ordem em que as versoes aparecem no CSV agregado
VERSOES=(column row pointer avx)

BIN="$ROOT/build"
OUT="$ROOT/data/outputs"
mkdir -p "$BIN" "$OUT" results

# tamanhos: os passados na linha de comando, ou tudo que existe em data/inputs
if [ $# -gt 0 ]; then
    TAMANHOS=("$@")
else
    mapfile -t TAMANHOS < <(ls data/inputs 2>/dev/null \
        | sed -n 's/^\([0-9]\+\)x\1$/\1/p' | sort -n)
fi

if [ ${#TAMANHOS[@]} -eq 0 ]; then
    echo "nenhuma matriz em data/inputs; rode scripts/generate_matrices.py" >&2
    exit 1
fi

RAW=results/scalar_raw.csv
echo "funcao,versao,size,rep,tempo_ms" > "$RAW"

for versao in "${VERSOES[@]}"; do
    fonte="benchmarks/scalar/scalar_${versao}.c"
    [ -f "$fonte" ] || { echo "== ${versao}: ausente, pulando"; continue; }

    echo "== compilando ${versao}"
    gcc $CFLAGS -o "$BIN/test_${versao}" matrix_lib_test.c "$fonte"

    for size in "${TAMANHOS[@]}"; do
        dir="data/inputs/${size}x${size}"
        if [ ! -f "$dir/A.dat" ]; then
            echo "   ${size}x${size}: entrada ausente, pulando"
            continue
        fi

        for rep in $(seq 1 "$REPS"); do
            # matrix_matrix_mult e um stub nos benchmarks escalares e faz o
            # programa sair com codigo != 0 apos imprimir o tempo do escalar;
            # por isso o "|| true".
            saida=$("$BIN/test_${versao}" -s "$SCALAR" \
                -r "$size" -c "$size" -C "$size" \
                -m "$dir/A.dat" -M "$dir/B.dat" \
                -o "$OUT/${versao}_${size}_o1.dat" \
                -O "$OUT/${versao}_${size}_o2.dat" 2>/dev/null || true)

            tempo=$(printf '%s\n' "$saida" \
                | sed -n 's/.*Scalar product: \([0-9.]*\) ms.*/\1/p')

            if [ -z "$tempo" ]; then
                echo "   ${size}x${size} rep ${rep}: SEM TEMPO (execucao falhou)" >&2
                continue
            fi
            echo "scalar_matrix_mult,${versao},${size},${rep},${tempo}" >> "$RAW"
        done

        # corretude: o resultado da ultima repeticao tem de bater com o numpy
        if ! python3 scripts/validate_results.py -s "$SCALAR" \
                -r "$size" -c "$size" \
                -m "$dir/A.dat" -o "$OUT/${versao}_${size}_o1.dat" \
                > /dev/null; then
            echo "   ${size}x${size}: VALIDACAO FALHOU" >&2
        fi

        mediana=$(awk -F, -v v="$versao" -v s="$size" \
            '$2==v && $3==s {print $5}' "$RAW" | sort -n \
            | awk '{a[NR]=$1} END {print (NR%2) ? a[(NR+1)/2] : (a[NR/2]+a[NR/2+1])/2}')
        echo "   ${size}x${size}: mediana ${mediana} ms (${REPS} execucoes)"
    done
done

# agrega: uma linha por tamanho, uma coluna por versao (mediana das repeticoes)
python3 - "$RAW" "${VERSOES[@]}" <<'PY'
import csv, statistics, sys
raw, versoes = sys.argv[1], sys.argv[2:]
dados = {}
with open(raw) as f:
    for r in csv.DictReader(f):
        dados.setdefault(int(r["size"]), {}).setdefault(r["versao"], []).append(
            float(r["tempo_ms"]))
with open("results/scalar_results.csv", "w", newline="") as f:
    w = csv.writer(f)
    w.writerow(["size", *versoes])
    for size in sorted(dados):
        w.writerow([size] + [
            f"{statistics.median(dados[size][v]):.6f}" if v in dados[size] else ""
            for v in versoes])
print("\nresults/scalar_results.csv gravado")
PY

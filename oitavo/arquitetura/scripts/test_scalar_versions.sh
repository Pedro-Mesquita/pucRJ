#!/usr/bin/env bash
#
# Teste de correcao das versoes de scalar_matrix_mult (sem medir tempo).
#
# Para cada versao em benchmarks/scalar/ e cada dimensao (quadrada ou nao):
#   1. compila com matrix_lib_test.c (professor, sem alteracoes);
#   2. executa e verifica que o programa nao reportou "Linear error";
#   3. valida a saida contra o numpy (scripts/validate_results.py);
#   4. compara byte a byte a saida das versoes entre si: como a operacao e a
#      mesma (um unico produto float por elemento) e so a ordem de acesso
#      muda, os resultados devem ser IDENTICOS, nao apenas proximos.
#
# Uso:
#   scripts/test_scalar_versions.sh                # dimensoes padrao
#   scripts/test_scalar_versions.sh 8 16x32 64x8   # dimensoes escolhidas
#   VERSOES="col row" scripts/test_scalar_versions.sh

set -uo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

CFLAGS="-Wall -std=c11 -mfma -O0 -I."
SCALAR="${SCALAR:-2.5}"
VERSOES=(${VERSOES:-col row increment avx})
DIMS=("$@")
[ ${#DIMS[@]} -eq 0 ] && DIMS=(8 16x32 64x8 8x128 128)

BIN="$ROOT/build"
OUT="$ROOT/data/outputs"
mkdir -p "$BIN" "$OUT"

# numpy e opcional: sem ele (ex.: IAC2) a validacao fica so com os checks do
# proprio matrix_lib_test.c e as entradas sao geradas pelo gerador em C.
if python3 -c "import numpy" 2>/dev/null; then TEM_NUMPY=1; else TEM_NUMPY=0; fi
gerar_entrada() {   # $1 = dimensao (N ou RxC)
    if [ "$TEM_NUMPY" = 1 ]; then
        python3 scripts/generate_matrices.py "$1" > /dev/null
    else
        [ -x "$BIN/generate_matrices" ] || gcc -O2 -o "$BIN/generate_matrices" scripts/generate_matrices.c
        "$BIN/generate_matrices" "$1" > /dev/null
    fi
}

[ "$TEM_NUMPY" = 1 ] || echo "(numpy indisponivel: validando so com check_linear_errors do professor)"

falhas=0

echo "== compilando"
for v in "${VERSOES[@]}"; do
    if ! gcc $CFLAGS -o "$BIN/test_$v" matrix_lib_test.c "benchmarks/scalar/scalar_$v.c"; then
        echo "   scalar_$v.c: FALHA DE COMPILACAO"; exit 1
    fi
    echo "   scalar_$v.c -> build/test_$v"
done

for dim in "${DIMS[@]}"; do
    rows="${dim%%x*}"; cols="${dim##*x}"
    dir="data/inputs/${rows}x${cols}"
    if [ ! -f "$dir/A.dat" ]; then
        gerar_entrada "$dim" || { falhas=$((falhas+1)); continue; }
    fi

    echo "== ${rows}x${cols}"
    for v in "${VERSOES[@]}"; do
        o1="$OUT/${v}_${rows}x${cols}_o1.dat"
        # matrix_matrix_mult e stub (retorna 1): o programa sai com codigo 10
        # depois de gravar e checar o resultado escalar -- esperado.
        saida=$("$BIN/test_$v" -s "$SCALAR" -r "$rows" -c "$cols" -C "$rows" \
            -m "$dir/A.dat" -M "$dir/B.dat" \
            -o "$o1" -O "$OUT/${v}_${rows}x${cols}_o2.dat" 2>&1)

        if ! grep -q "Scalar product:" <<<"$saida"; then
            echo "   $v: execucao falhou"; echo "$saida" | tail -3; falhas=$((falhas+1)); continue
        fi
        if grep -q "Linear error" <<<"$saida"; then
            echo "   $v: check_linear_errors reportou erro"; falhas=$((falhas+1)); continue
        fi
        if [ "$TEM_NUMPY" != 1 ]; then
            echo "   $v: check do professor OK"
        elif python3 scripts/validate_results.py -s "$SCALAR" -r "$rows" -c "$cols" \
                -m "$dir/A.dat" -o "$o1" > /dev/null; then
            echo "   $v: check do professor OK, numpy OK"
        else
            echo "   $v: numpy FALHOU"; falhas=$((falhas+1))
        fi
    done

    # equivalencia byte a byte entre as versoes
    ref="${VERSOES[0]}"
    for v in "${VERSOES[@]:1}"; do
        if cmp -s "$OUT/${ref}_${rows}x${cols}_o1.dat" "$OUT/${v}_${rows}x${cols}_o1.dat"; then
            echo "   $ref == $v (identicos byte a byte)"
        else
            echo "   $ref != $v: SAIDAS DIFEREM"; falhas=$((falhas+1))
        fi
    done
done

echo
if [ "$falhas" -eq 0 ]; then echo "TODOS OS TESTES PASSARAM"; else echo "$falhas FALHA(S)"; exit 1; fi

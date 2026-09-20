#!/usr/bin/env bash
#
# Teste de correcao das versoes de matrix_matrix_mult (sem medir tempo).
#
# Para cada versao em benchmarks/matrix/ e cada dimensao (quadrada ou nao):
#   1. compila com matrix_lib_test.c (professor, sem alteracoes);
#   2. executa e anota se check_mult_errors() reportou erro (informativo);
#   3. valida a saida contra o numpy (scripts/validate_results.py) -- e o
#      criterio que decide. check_mult_errors() compara com uma soma float32
#      sem FMA e exige erro relativo < 1e-4 em relacao a |C[i,j]|; em
#      elementos quase nulos por cancelamento (|C| ~ 1e-3 com termos ~1)
#      isso reprova qualquer arredondamento diferente do da propria
#      referencia, inclusive o FMA, sem que haja erro real;
#   4. compara as saidas das versoes entre si. Convencional e somas parciais
#      acumulam os produtos na MESMA ordem (k = 0..p-1), entao em -O0 as
#      saidas devem ser identicas; a versao AVX/FMA arredonda diferente
#      (FMA nao arredonda o produto intermediario), por isso a equivalencia
#      entre versoes e informativa e a validacao que decide e a do numpy.
#
# Entradas "RxC": A e RxC e B e CxR, entao C = A x B e RxR (-r R -c C -C R).
#
# Uso:
#   scripts/test_matrix_versions.sh                # dimensoes padrao
#   scripts/test_matrix_versions.sh 8 16x32 64x8   # dimensoes escolhidas
#   VERSOES="conventional matrix" scripts/test_matrix_versions.sh

set -uo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

CFLAGS="-Wall -std=c11 -mfma -O0 -I."
SCALAR="${SCALAR:-2.5}"
# nome da versao = sufixo do arquivo benchmarks/matrix/matrix_<versao>.c
VERSOES=(${VERSOES:-conventional matrix avx})
DIMS=("$@")
[ ${#DIMS[@]} -eq 0 ] && DIMS=(8 16x32 64x8 8x128 128 256)

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

[ "$TEM_NUMPY" = 1 ] || echo "(numpy indisponivel: validando so com check_mult_errors do professor)"

falhas=0

echo "== compilando"
for v in "${VERSOES[@]}"; do
    if ! gcc $CFLAGS -o "$BIN/test_matrix_$v" matrix_lib_test.c "benchmarks/matrix/matrix_$v.c"; then
        echo "   matrix_$v.c: FALHA DE COMPILACAO"; exit 1
    fi
    echo "   matrix_$v.c -> build/test_matrix_$v"
done

for dim in "${DIMS[@]}"; do
    rows="${dim%%x*}"; cols="${dim##*x}"
    dir="data/inputs/${rows}x${cols}"
    if [ ! -f "$dir/A.dat" ]; then
        gerar_entrada "$dim" || { falhas=$((falhas+1)); continue; }
    fi

    echo "== A ${rows}x${cols} * B ${cols}x${rows}"
    for v in "${VERSOES[@]}"; do
        o1="$OUT/matrix_${v}_${rows}x${cols}_o1.dat"
        o2="$OUT/matrix_${v}_${rows}x${cols}_o2.dat"
        saida=$("$BIN/test_matrix_$v" -s "$SCALAR" -r "$rows" -c "$cols" -C "$rows" \
            -m "$dir/A.dat" -M "$dir/B.dat" -o "$o1" -O "$o2" 2>&1)
        codigo=$?

        if [ $codigo -ne 0 ] || ! grep -q "Matrix product:" <<<"$saida"; then
            echo "   $v: execucao falhou (codigo $codigo)"; echo "$saida" | tail -3
            falhas=$((falhas+1)); continue
        fi
        check="check do professor OK"
        if grep -q "Multiplication error" <<<"$saida"; then
            check="check do professor divergiu em $(grep -o 'Multiplication error at \[[0-9, ]*\]' <<<"$saida" | head -1 | sed 's/Multiplication error at //')"
            # sem numpy o check do professor e o unico criterio: divergencia conta como falha
            [ "$TEM_NUMPY" = 1 ] || { echo "   $v: $check"; falhas=$((falhas+1)); continue; }
        fi
        if [ "$TEM_NUMPY" != 1 ]; then
            echo "   $v: $check"; continue
        fi
        # -o/-O sao validados juntos; scalar_matrix_mult e stub nestes
        # arquivos, entao so a parte matrix_matrix_mult do relatorio importa
        rel=$(python3 scripts/validate_results.py -s "$SCALAR" -r "$rows" -c "$cols" -C "$rows" \
                -m "$dir/A.dat" -M "$dir/B.dat" -o "$o1" -O "$o2" 2>/dev/null \
              | awk '/^matrix_matrix_mult/{f=1} f && /erro relativo maximo/{print $5} f && /resultado/{print $3; exit}')
        erro=$(sed -n 1p <<<"$rel"); ok=$(sed -n 2p <<<"$rel")
        if [ "$ok" = "OK" ]; then
            echo "   $v: $check, numpy OK (erro rel max $erro)"
        else
            echo "   $v: numpy FALHOU (erro rel max $erro)"; falhas=$((falhas+1))
        fi
    done

    ref="${VERSOES[0]}"
    for v in "${VERSOES[@]:1}"; do
        if cmp -s "$OUT/matrix_${ref}_${rows}x${cols}_o2.dat" "$OUT/matrix_${v}_${rows}x${cols}_o2.dat"; then
            echo "   $ref == $v (identicos byte a byte)"
        else
            echo "   $ref ~= $v (diferem em bits; ambos validados pelo numpy)"
        fi
    done
done

echo
if [ "$falhas" -eq 0 ]; then echo "TODOS OS TESTES PASSARAM"; else echo "$falhas FALHA(S)"; exit 1; fi

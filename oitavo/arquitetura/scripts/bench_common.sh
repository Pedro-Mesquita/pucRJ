#!/usr/bin/env bash
#
# Funcoes compartilhadas por run_scalar_benchmarks.sh e run_matrix_benchmarks.sh.
# Apenas bash + coreutils + awk: precisa rodar em maquinas sem Python/numpy.
#
# A medicao vem do proprio matrix_lib_test.c (professor, sem alteracoes), que
# cronometra cada funcao com clock(). Os scripts so executam o programa,
# capturam o stdout e agregam os tempos.

# -O0: sem otimizacao, para que a diferenca medida venha do codigo escrito e
# nao da auto-vetorizacao do GCC (com -O3 o compilador gera AVX sozinho nas
# versoes "row", o que invalidaria a comparacao). Mesma flag para todas.
CFLAGS="-Wall -std=c11 -mfma -O0 -I."
# repeticoes de cada versao em cada tamanho (escalar e matriz)
REPS="${REPS:-10}"

# Tamanhos (N x N) medidos pelos dois benchmarks, numa unica execucao do
# script. Lista fixa: todas as versoes sao medidas nos mesmos tamanhos; o
# maior e 4000 x 4000.
TAMANHOS=(8 16 32 64 128 256 512 1024 2048 4000)
SCALAR="${SCALAR:-2.5}"
# a partir deste tamanho a saida .dat e apagada depois de cada execucao
APAGAR_SAIDA_A_PARTIR="${APAGAR_SAIDA_A_PARTIR:-4000}"

# Garante que data/inputs/<N>x<N>/A.dat existe; gera com o gerador em C se nao.
garantir_entrada() {   # $1 = N
    local dir="data/inputs/${1}x${1}"
    [ -f "$dir/A.dat" ] && return 0
    [ -x build/generate_matrices ] || gcc -O2 -o build/generate_matrices scripts/generate_matrices.c
    echo "   gerando ${1}x${1}"
    build/generate_matrices "$1" > /dev/null
}

# Mediana de numeros lidos do stdin (um por linha).
mediana() {
    sort -n | awk '{a[NR]=$1} END {if (NR==0) {print ""; exit}
                                    print (NR%2) ? a[(NR+1)/2] : (a[NR/2]+a[NR/2+1])/2}'
}

# Le o CSV bruto (funcao,versao,size,rep,tempo_ms) e escreve o agregado
# (size,<versao1>,<versao2>,...) com a mediana das repeticoes.
#   $1 = raw csv   $2 = csv de saida   $3.. = versoes na ordem das colunas
agregar() {
    local raw="$1" out="$2"; shift 2
    local versoes=("$@")
    {
        printf 'size'; printf ',%s' "${versoes[@]}"; printf '\n'
        for size in $(awk -F, 'NR>1 {print $3}' "$raw" | sort -n | uniq); do
            printf '%s' "$size"
            for v in "${versoes[@]}"; do
                printf ',%s' "$(awk -F, -v v="$v" -v s="$size" '$2==v && $3==s {print $5}' "$raw" | mediana)"
            done
            printf '\n'
        done
    } > "$out"
    echo; echo "$out gravado (mediana das repeticoes):"; column -s, -t "$out"
}

# Roda o programa de teste uma vez e imprime o tempo pedido, ou nada se falhou.
#   $1 = binario  $2 = N  $3 = prefixo das saidas  $4 = "Scalar product" | "Matrix product"
#   $5 = 1 para encerrar o processo assim que o check do produto comecar
#        (check_mult_errors e O(n^3) convencional em -O0: horas para n grande;
#        o tempo e result2 ja foram gravados nesse ponto)
rodar_uma_vez() {
    local bin="$1" n="$2" prefixo="$3" rotulo="$4" matar="$5"
    local dir="data/inputs/${n}x${n}" tmp
    tmp=$(mktemp)
    stdbuf -oL "$bin" -s "$SCALAR" -r "$n" -c "$n" -C "$n" \
        -m "$dir/A.dat" -M "$dir/B.dat" \
        -o "data/outputs/${prefixo}_o1.dat" -O "data/outputs/${prefixo}_o2.dat" \
        > "$tmp" 2>&1 &
    local pid=$!
    if [ "$matar" = 1 ]; then
        while kill -0 "$pid" 2>/dev/null; do
            if grep -q '\[221\] Checking matrix result' "$tmp"; then kill "$pid" 2>/dev/null; break; fi
            sleep 0.2
        done
    fi
    wait "$pid" 2>/dev/null
    sed -n "s/.*${rotulo}: \([0-9.]*\) ms.*/\1/p" "$tmp"
    # divergencias reportadas pelo professor viram aviso (nao devem ocorrer nas
    # versoes mul+add). So as da funcao medida: a outra e stub nesses binarios.
    local padrao="Linear error|ERROR"
    [ "$rotulo" = "Matrix product" ] && padrao="Multiplication error|ERROR"
    grep -E "$padrao" "$tmp" | head -2 | sed 's/^/      aviso: /' >&2
    rm -f "$tmp"
    if [ "$n" -ge "$APAGAR_SAIDA_A_PARTIR" ]; then
        rm -f "data/outputs/${prefixo}_o1.dat" "data/outputs/${prefixo}_o2.dat"
    fi
}

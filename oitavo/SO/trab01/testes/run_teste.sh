#!/bin/bash
# Roda um cenário de teste do KernelSim e confere o resultado.
# Uso: testes/run_teste.sh N      (N = 1, 2, 3 ou 4; veja testes/README.md)

cd "$(dirname "$0")/.." || exit 1

case "$1" in
    1) ARGS="6:2R 6 6" ;;
    2) ARGS="8:3R,6W 5 5:2W" ;;
    3) ARGS="4:2R 4:1W 4:1R" ;;
    4) ARGS="4 4:1W 4:1R 5 3 4" ;;
    *) echo "uso: $0 N   (N = 1, 2, 3 ou 4; veja testes/README.md)"; exit 1 ;;
esac

ESPERADO="testes/esperado/teste$1.txt"
LOG="testes/saida/teste$1.log"
mkdir -p testes/saida
make -s || exit 1

echo "=== Teste $1: ./kernel $ARGS ==="
echo "(linha do tempo esperada em $ESPERADO)"
echo

# O kernel é infinito (decisão 17): o script espera todos os apps terminarem
# e então manda SIGINT, o mesmo que um Ctrl+C. O tee -i ignora o Ctrl+C para
# não fechar a saída antes de o kernel terminar a limpeza.
./kernel $ARGS > >(tee -i "$LOG") 2>&1 &
KPID=$!
trap 'kill -INT $KPID 2>/dev/null' INT

for ((i = 0; i < 600; i++)); do
    grep -q "todos os processos terminaram" "$LOG" 2>/dev/null && break
    kill -0 $KPID 2>/dev/null || break
    sleep 0.1
done
sleep 1
kill -INT $KPID 2>/dev/null
wait $KPID
sleep 0.3

echo
echo "=== Conferência ==="
FALHAS=0

# 1. Linha do tempo: os eventos do kernel, com o tempo em segundos inteiros,
#    até o evento em que todos os apps ficam (F).
normaliza() {
    sed -nE 's/^t= *([0-9]+)\.[0-9] +([^ |]+) *\| RUN=([^ |]+) *\|/t=\1 \2 | RUN=\3 |/p' "$1" |
        awk '{ print } /PC:/ && !/=[0-9]+\/[0-9]+( |$)/ { exit }'
}
if diff -u --label esperado --label obtido <(grep '^t=' "$ESPERADO") <(normaliza "$LOG") > testes/saida/teste$1.diff; then
    echo "[OK]     linha do tempo igual à esperada"
    rm -f testes/saida/teste$1.diff
else
    echo "[FALHOU] linha do tempo diferente da esperada (testes/saida/teste$1.diff):"
    sed 's/^/         /' testes/saida/teste$1.diff
    FALHAS=$((FALHAS + 1))
fi

# 2. Todo app termina com PC = MAX.
FINAL=$(grep 'PC:' "$LOG" | tail -1)
if echo "$FINAL" | grep -oE 'A[0-9]=[0-9]+/[0-9]+(\(F\))?' |
        awk -F'[=/(]' '$2 != $3 || $0 !~ /\(F\)/ { print "         " $0 " não terminou com PC = MAX"; ruim = 1 } END { exit ruim }'; then
    echo "[OK]     todos os apps terminaram com PC = MAX"
else
    echo "[FALHOU] algum app não terminou com PC = MAX"
    FALHAS=$((FALHAS + 1))
fi

# 3. Nenhum processo do teste sobrou (kernel, InterController e apps).
VIVOS=""
for pid in $(grep -oE 'pid=[0-9]+' "$LOG" | cut -d= -f2); do
    kill -0 "$pid" 2>/dev/null && VIVOS="$VIVOS $pid"
done
if [ -z "$VIVOS" ]; then
    echo "[OK]     nenhum processo sobrou"
else
    echo "[FALHOU] processos ainda vivos:$VIVOS"
    FALHAS=$((FALHAS + 1))
fi

# 4. A memória compartilhada foi removida.
SHMID=$(grep -oE 'shmid=[0-9]+' "$LOG" | head -1 | cut -d= -f2)
if [ -n "$SHMID" ] && ! ipcs -m | awk -v id="$SHMID" '$2 == id { achou = 1 } END { exit !achou }'; then
    echo "[OK]     memória compartilhada (shmid=$SHMID) removida"
else
    echo "[FALHOU] memória compartilhada shmid=$SHMID ainda existe"
    FALHAS=$((FALHAS + 1))
fi

echo
if [ "$FALHAS" -eq 0 ]; then
    echo "Teste $1: PASSOU"
else
    echo "Teste $1: $FALHAS conferência(s) falharam"
fi
exit "$FALHAS"

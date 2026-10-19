# Testes do KernelSim

Cada teste roda o próprio sistema (kernel, InterController e apps) com uma configuração fixa
e confere o resultado sozinho.

## Como rodar
Da raiz do projeto:
```
testes/run_teste.sh N        # N = 1, 2, 3 ou 4
```
O script compila (`make`), roda o kernel mostrando o log ao vivo e, quando todos os apps
terminam, manda `SIGINT` (o mesmo que um Ctrl+C), porque o kernel é infinito (decisão 17).
Depois ele faz 4 conferências:

1. **Linha do tempo:** os eventos do kernel (`INICIO`, `IRQ0`, `IRQ1`, `SYS`, `FIM`) são
   iguais aos de `esperado/testeN.txt`, com o tempo arredondado para o segundo inteiro.
2. **PC final:** todo app terminou com PC = MAX.
3. **Processos:** nenhum processo do teste sobrou (kernel, InterController e apps).
4. **IPC:** a memória compartilhada do teste foi removida.

O log completo fica em `saida/testeN.log`. Se a linha do tempo divergir, a diferença fica em
`saida/testeN.diff`.

## Os testes
| N | Comando | O que mostra |
|---|---|---|
| 1 | `./kernel 6:2R 6 6` | (a) um processo em E/S e os outros continuam; (d) quem volta da E/S entra na frente de quem saiu por time slice |
| 2 | `./kernel 8:3R,6W 5 5:2W` | (b) dois pedidos quase juntos: fila FIFO e E/S em série; CPU ociosa e retomada direta pelo IRQ1 |
| 3 | `./kernel 4:2R 4:1W 4:1R` | (c) todos bloqueados, a CPU fica ociosa e os IRQ1 liberam um por vez, a cada 3 s |
| 4 | `./kernel 4 4:1W 4:1R 5 3 4` | 6 processos com MAX diferentes: prioridade da E/S com fila longa e términos em momentos diferentes |

Cada `esperado/testeN.txt` explica o que deve acontecer e traz a linha do tempo prevista
pelas regras do escalonador.

## Como ler uma linha do log
```
t= 6.0 IRQ1(R) | RUN=A3  | READY=[A1 A2] | D1=[] | PC: A1=2/6 A2=3/6 A3=2/6
```
- `t`: segundos desde o início do kernel.
- Evento: `INICIO`, `IRQ0` (fim do time slice), `IRQ1(op)` (fim da E/S),
  `SYS(op)` (syscall) ou `FIM` (um app terminou).
- `RUN`: quem está com a CPU depois do evento (`--` = CPU ociosa).
- `READY`: fila de prontos, da frente para o fim.
- `D1`: fila de bloqueados em D1, com a operação pedida. O primeiro é o que o D1 está atendendo.
- `PC`: PC salvo no PCB de cada app (atualizado quando o app perde a CPU) / MAX.
  `(F)` = terminou.

## Por que as syscalls dos testes começam em t ≥ 1
Uma syscall em t=0 (por exemplo, `4:1R` no A1) faz o fim da E/S cair a poucos microssegundos
de um IRQ0. A ordem entre os dois passa a depender do acaso, e a linha do tempo muda de uma
execução para outra. Com as syscalls em t ≥ 1, o IRQ1 chega alguns milissegundos depois do
IRQ0 do mesmo segundo, e o resultado se repete.

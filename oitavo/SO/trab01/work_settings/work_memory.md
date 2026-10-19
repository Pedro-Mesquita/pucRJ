## Decisões sobre ambiguidades
1 - Um IRQ1 a cada 3s após o pedido de I/O
2 - Acredito que o syscall deve ser mais simples de ser implementado usando signals.
3 - No KernelSim uma estrutura por processo, já que é ele que vai lidar com os processos
4 - Deve ter um enum com os possíveis estados de um processo (ready, finished, etc). Quando um processo roda a qtd máxima o status dele vai para finished e ele não retorna para a lista de prontos.
5 - Fica ocioso
6 - É só informação/log e de contexto salvo

## Mapa de sinais
SIGUSR1  -> IRQ0
SIGUSR2  -> IRQ1
SIGRTMIN -> syscall R
SIGRTMIN + 1-> syscall W
SIGRTMIN + 2-> I/O 
## Decisões do passo 1
7 - app recebe nome e MAX por argv (`./app A1 10`): o enunciado só fala em "laço de até MAX iterações" e não diz se MAX é igual para todos. MAX por processo permite testar um processo terminando antes dos outros.
## Decisões do passo 3
8 - Apps começam parados com `raise(SIGSTOP)` no filho, antes do `exec`: o filho para a si mesmo, então não há corrida com o pai e o `app.c` não precisa saber que é escalonado. O pai confirma a parada com `waitpid(..., WUNTRACED)`.
## Decisões do passo 4 (ponto B)
9 - PC em memória compartilhada: um vetor de slots (um por app) em shm. O app incrementa o PC direto no slot. Salvar = o kernel lê o slot depois do `SIGSTOP` e guarda no PCB. Restaurar = o kernel escreve o PC do PCB de volta no slot antes do `SIGCONT`. Assim a troca de contexto é real, e o trabalho usa IPC além de sinais.
10 - Iteração do app = `PC++` e depois dormir 1 s em 100 fatias de 10 ms (a última iteração não dorme). Um `sleep(1)` direto conta tempo de relógio, até com o app parado, e como a iteração (1 s) coincidia com o time slice (1 s), alguns slices tinham 0 iterações e outros tinham 2. Com as fatias, o tempo parado conta no máximo 10 ms e cada time slice executa exatamente 1 iteração.
11 - Kernel recebe o MAX de cada app por argv: `./kernel 5 3 5` cria A1(MAX=5), A2(MAX=3), A3(MAX=5). Entre 3 e 6 apps.
## Decisões do passo 5
12 - Roteiro de syscalls no argv do kernel, no formato `MAX[:roteiro]`: `./kernel 8:3R,6W 5 5:2W` faz A1 (MAX=8) pedir R no PC 3 e W no PC 6, A2 (MAX=5) sem syscall e A3 (MAX=5) pedir W no PC 2. Esse formato é compatível com o do passo 4, e cada cenário de teste é só uma linha de comando. O kernel valida o roteiro e repassa para o app como 5º argumento. "3R" quer dizer: depois de executar a iteração 3 (PC=3), o app pede R; quando volta da E/S, vai direto para a iteração 4, sem dormir, porque a syscall já foi o "trabalho" da iteração 3.
13 - A syscall é um `kill` simples, sem `sigqueue`/`sigval`. A operação (R/W) já vem no número do sinal (`SIG_SYS_R`/`SIG_SYS_W`), e o PC o kernel lê da shm depois do `SIGSTOP`, como no IRQ0. Como nada vai junto, o `sigqueue` não traz vantagem: o enfileiramento vem de o sinal ser de tempo real e o `si_pid` também chega com `kill`.
14 - Espera do app durante a E/S: flag `bloqueado` no slot da shm. O app liga a flag, manda a syscall e espera em loop de nanosleep (10 ms) enquanto ela estiver ligada. O kernel desliga a flag no IRQ1 (passo 6), antes do `SIGCONT`. Isso evita que o app avance o PC no intervalo entre mandar o sinal e levar o `SIGSTOP`, e não tem a corrida do `pause()` + `SIGCONT`.
## Decisões do passo 6 (ponto C)
15 - E/S em série: o D1 atende um pedido por vez, em ordem FIFO, e cada atendimento leva IO_TIME (3 s) a partir do momento em que começa. Se o D1 está livre, isso dá 3 s após o pedido; se está ocupado, o pedido espera na fila. Justificativa: o enunciado fala de um único dispositivo que "precisa de 3 segundos para executar as operações", com fila FIFO de bloqueados, que é o modelo clássico de dispositivo dos livros de SO. Como os IRQ1 ficam sempre a pelo menos 3 s um do outro, o SIGUSR2 continua servindo como IRQ1 (o mapa de sinais não muda).
16 - O InterController não usa mais `sleep(TIMESLICE)`: ele controla o tempo por horários absolutos (`prox_irq0` e `t_fim`) e espera com `sigtimedwait` pelo aviso de E/S ou até o próximo horário. Um `sleep` seria interrompido a cada aviso de E/S e adiantaria o IRQ0, deixando o time slice irregular. O IRQ0 continua saindo a cada 1 s, como pede o enunciado.
## Decisões do passo 7 (ponto D)
17 - O kernel não encerra sozinho quando todos os apps ficam FINISHED: ele avisa "todos os processos terminaram" e continua em loop, recebendo IRQ0 com a CPU ociosa (`RUN=--`), até receber Ctrl+C (SIGINT) ou SIGTERM. Só então `encerrar()` mata o InterController e os apps vivos, recolhe todos com `waitpid` e remove a shm. Justificativa: o enunciado diz que KernelSim e InterControllerSim "devem ser processos infinitos que executam em paralelo".
## Decisões do passo 8
18 - Testes em `testes/`: `run_teste.sh N` roda o cenário N (1 a 4), espera todos os apps terminarem, manda SIGINT e confere quatro coisas: a linha do tempo contra `esperado/testeN.txt` (tempo em segundos inteiros), PC = MAX em todos, nenhum processo sobrando e shm removida. As syscalls dos testes começam em t ≥ 1: uma syscall em t=0 faz o IRQ1 cair a microssegundos de um IRQ0, e a ordem entre os dois vira sorteio.

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

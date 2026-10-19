# Plano do Trabalho — INF1316 Trab1 (Kernel Sim)

## Como usar este roteiro
Este roteiro é **mão na massa**. Cada passo produz código que roda e pode ser testado.
As decisões de projeto **não** são todas tomadas no começo: cada uma aparece no passo em que
ela passa a importar, num bloco **🔸 Decida aqui**. Antes de codar, anote a decisão no
`work_memory.md` (uma linha com o porquê).

Estrutura de cada passo:
- **Objetivo:** o que vai estar funcionando no fim do passo.
- **🔸 Decida aqui:** o que precisa ser decidido agora, só se houver.
- **Mão na massa:** arquivos, funções e chamadas de sistema, em ordem.
- **Armadilha:** o erro mais comum desse passo.
- **Como verificar:** comando ou cenário com o resultado esperado.

## Objetivo
Escrever em C um simulador de kernel (**KernelSim**) que escalona de 3 a 6 processos de
aplicação (A1..A6) de forma preemptiva. Ele recebe "interrupções" de um processo
**InterControllerSim**: IRQ0 marca o fim do time slice e IRQ1 marca o fim de uma E/S em D1.
Os processos são controlados com `SIGSTOP`/`SIGCONT`.

## Entregável
- Código-fonte (KernelSim, InterControllerSim, programa(s) de aplicação e programas de teste).
- Relatório com: os programas que compõem o trabalho, os programas de teste e **o que deve
  acontecer** na execução. Sugestão do enunciado: linha do tempo, fila de execução e PC de
  cada processo. Essa explicação também conta na nota.
- Apresentação no laboratório (a dupla inteira presente). Cada dia de atraso tira 1 ponto.

## Requisitos identificados
**Explícitos no enunciado**
- Linguagem C, com `fork`/`exec`, sinais e IPC do Unix.
- Cada elemento da Fig. 1 é um **processo Unix separado**: KernelSim, InterControllerSim, A1..An.
- KernelSim e InterControllerSim são **processos infinitos** rodando em paralelo.
- Entre 3 e 6 processos de aplicação.
- IRQ0 a cada 1 s (com `sleep()` no corpo do loop do InterController).
- IRQ1 3 s depois do pedido de E/S (o D1 leva 3 s por operação).
- Os tempos podem ser alterados se ficar difícil de visualizar.
- No IRQ0, o kernel faz `SIGSTOP` no processo atual e `SIGCONT` no próximo que **não** está bloqueado.
- `syscall(D1, R|W)`: o processo é parado na hora, com `SIGSTOP`, e vai para a **fila de bloqueados de D1 (FIFO)**.
- Cada IRQ1 libera o **primeiro** da fila de bloqueados.
- Aplicação: um loop de até MAX iterações, contador `PC`, `sleep(1)` no corpo e momentos
  predefinidos (valores de PC) em que chama `syscall(D1, R ou W)`.
- Troca de contexto: salvar e restaurar o **PC**, mais os **parâmetros da syscall** quando
  essa foi a causa da interrupção. Todos os processos têm que executar exatamente MAX iterações.
- Quem acabou de sair da E/S tem **prioridade** sobre quem perdeu a CPU por time slice
  (entra antes na fila de prontos).
- Não existe ShellSim: as estruturas do kernel já começam inicializadas.

## Decisões já tomadas (de `work_memory.md`)
1. **IRQ1 3 s após cada pedido de E/S.** O kernel repassa cada pedido ao InterController.
2. **Syscall por sinal.**
3. **Um PCB por processo, dentro do KernelSim.**
4. **Enum de estados** (`READY`, `RUNNING`, `BLOCKED`, `FINISHED`). Quem chega a MAX vira
   `FINISHED` e não volta para a fila de prontos.
5. **CPU ociosa:** o kernel fica ocioso até o próximo IRQ1.
6. **R/W:** a diferença aparece só no log e no contexto salvo.
7. **Mapa de sinais (parcial):** IRQ0 = `SIGUSR1`, IRQ1 = `SIGUSR2` (será revisto no passo 6),
   syscall R = `SIGRTMIN`, syscall W = `SIGRTMIN+1`, aviso de E/S = `SIGRTMIN+2`.

## Progresso
- [x] 0. Estrutura do projeto
- [x] 1. Aplicação Ai sem syscall
- [x] 2. Kernel mínimo + InterControllerSim (IRQ0)
- [x] 3. Kernel cria os apps parados
- [x] 4. Round-robin com IRQ0 (decide B: PC)
- [x] 5. Syscall e bloqueio (decide o roteiro de syscalls)
- [x] 6. IRQ1 e fila de D1 (decide C: modelo de E/S)
- [x] 7. Término e encerramento (decide D)
- [x] 8. Logs e cenários de teste
- [ ] 9. Relatório
- [ ] 10. Ensaio

---

### 0. Estrutura do projeto
**Objetivo:** `make` compila três executáveis vazios.

**Mão na massa:**
1. Crie `kernel.c`, `intercontroller.c`, `app.c` e `common.h`.
2. No `common.h`, coloque os `#define` dos sinais da decisão 7 e as constantes de tempo
   (`TIMESLICE 1`, `IO_TIME 3`). Os sinais de tempo real precisam ser macros com expressão,
   como `#define SIG_SYS_R (SIGRTMIN)`.
3. Escreva um `Makefile` com `-Wall -Wextra -g` e um alvo `clean`.

**Armadilha:** `SIGRTMIN` não é constante na glibc. Por isso não dá para usar `case SIG_SYS_R:`
num `switch`. Use `if`.

**Como verificar:** `make && ls` mostra `kernel`, `intercontroller` e `app`.

---

### 1. Aplicação Ai sem syscall
**Objetivo:** `./app A1 10` conta de 1 a 10, uma linha por segundo.

**🔸 Decida aqui:** o formato do `argv`. Sugestão: `app <nome> <MAX> [roteiro]`, com o roteiro
entrando só no passo 5. Defina também o valor de MAX dos testes (por exemplo, entre 8 e 15).

**Mão na massa:**
1. Leia nome e MAX do `argv`.
2. Faça `for (pc = 0; pc < MAX; ) { sleep(1); pc++; printf(...); }`.
3. Coloque `setvbuf(stdout, NULL, _IONBF, 0)` ou um `fflush` depois de cada `printf`. Sem
   isso, a saída de vários processos chega fora de ordem.

**Armadilha:** um `sleep(1)` interrompido por sinal volta antes da hora. Por enquanto isso não
faz diferença, mas lembre quando o PC "andar rápido".

**Como verificar:** rode `./app A1 10 &` e depois `kill -STOP <pid>`. Espere 5 s e faça
`kill -CONT <pid>`. O PC continua de onde parou, sem pular nem repetir números.

---

### 2. Kernel mínimo + InterControllerSim (IRQ0)
**Objetivo:** `./kernel` cria o InterController e imprime "IRQ0" a cada 1 s. Ainda não há apps.

O próprio kernel é o receptor do IRQ0, então não precisa de nenhum programa descartável
para testar o InterController.

**Mão na massa:**
1. **`intercontroller.c`:** o destino é `getppid()`, porque quem cria o InterController é o
   kernel. O laço é `while (1) { sleep(TIMESLICE); kill(kernel, SIG_IRQ0); }`. Se o `kill`
   falhar, o kernel morreu: saia, para não deixar um órfão. Por enquanto não tem IRQ1.
2. **`kernel.c`:** instale o handler de `SIG_IRQ0` com `sigaction` (`sa_handler` simples; o `SA_SIGINFO` só entra no passo 5). O handler
   só faz `irq0_flag = 1`, com a flag declarada `volatile sig_atomic_t`.
3. **`kernel.c`:** só **depois** de instalar o handler, faça `fork` + `exec` do InterController.
4. **`kernel.c`:** o loop principal fica em `pause()`. Quando `irq0_flag` estiver ligada,
   desligue a flag e imprima "IRQ0".

**Armadilha:** a ação padrão do `SIGUSR1` é **matar** o processo. Se o InterController for
criado antes do handler, o primeiro IRQ0 pode matar o kernel.

**Como verificar:**
- `./kernel` imprime "IRQ0" uma vez por segundo, com intervalo regular.
- Em outro terminal, `kill -USR1 <pid_kernel>` faz aparecer um IRQ0 extra. Isso mostra que o
  handler está instalado.
- Mate o kernel com `kill -9 <pid_kernel>`. Em até 1 s o InterController também deve sumir
  (`pgrep intercontroller` não mostra nada).

---

### 3. Kernel cria os apps parados
**Objetivo:** o kernel cria N apps, todos começam **parados**, e o IRQ0 continua chegando.

**🔸 Decida aqui:** como garantir que nenhum Ai roda nenhuma iteração antes de ser escalonado.
As opções são:
- (a) o filho dá `raise(SIGSTOP)` antes do `exec`;
- (b) o pai dá `kill(pid, SIGSTOP)` logo depois do `fork`, o que tem corrida;
- (c) o app espera um primeiro `SIGCONT`.

A opção (a) é a mais simples e não tem corrida.

**Mão na massa:**
1. Defina o `PCB` (pid, nome, estado, pc) e um vetor `pcb[6]`.
2. Para cada app, faça `fork`. No filho, `raise(SIGSTOP)` e depois `execl("./app", ...)`.
   No pai, preencha o PCB com estado `READY`.
3. No pai, use `waitpid(pid, &st, WUNTRACED)` para confirmar que o filho já parou.
4. Crie os apps antes do InterController, para que o primeiro IRQ0 já encontre todos os PCBs
   prontos.

**Armadilha:** sem `WUNTRACED`, o `waitpid` só retorna quando o filho **termina**. Como o
filho está parado, o kernel ficaria travado para sempre esperando por ele.

**Como verificar:** com `ps -o pid,stat,cmd --ppid <kernel>`, todos os apps aparecem em `T`
e o kernel continua imprimindo "IRQ0" a cada segundo.

---

### 4. Round-robin com IRQ0
**Objetivo:** 3 apps sem syscall se alternam a cada 1 s e todos terminam com PC = MAX.

**🔸 Decida aqui (ponto B): como o kernel sabe o PC do processo que ele parou?** No IRQ0 o Ai
é parado sem aviso. As opções são:
- **shm:** cada Ai escreve `pc` num slot da memória compartilhada. O kernel lê o slot depois do
  `SIGSTOP`. "Restaurar" quer dizer escrever o PC salvo de volta no slot antes do `SIGCONT`,
  com o Ai lendo o PC do slot no começo de cada iteração.
- **só sinal:** a cada iteração o Ai faz `sigqueue(kernel, ..., pc)`. São muitos sinais, e o
  "restaurar" fica simbólico.

Escolha uma, anote no `work_memory.md` e defina o que "restaurar o PC" quer dizer.

**Mão na massa:**
1. Implemente o mecanismo escolhido. Se for shm: `shmget` e `shmat` no kernel antes dos
   `fork`, e o id vai para o app por `argv`.
2. Faça a fila de prontos como uma FIFO circular de índices de PCB, com `push_back`,
   `push_front` (já pensando no passo 6) e `pop_front`.
3. No IRQ0: `SIGSTOP` no atual, salva o PC, `push_back` do atual, `pop_front` do próximo,
   restaura o PC dele e manda `SIGCONT`.
4. Escreva uma função `log_estado("IRQ0")` que imprime o tempo, quem está executando, a fila
   de prontos e o PC de cada processo.

**Armadilha:** o processo atual pode ter acabado de terminar. Por enquanto, se `pc == MAX`,
não coloque ele de volta na fila (o tratamento completo fica para o passo 7).

**Como verificar:** com 3 apps e MAX = 5, o log mostra A1, A2, A3, A1... e cada app imprime
de 1 a 5 sem buracos.

---

### 5. Syscall e bloqueio
**Objetivo:** um Ai que chega no PC do roteiro fica bloqueado. Os outros continuam e ele
**não** volta, porque o IRQ1 só chega no passo 6.

**🔸 Decida aqui:**
- o formato do roteiro no `argv`, por exemplo `3R,7W` para R no PC 3 e W no PC 7;
- se o PC vai junto no `sigval` da syscall.

**Mão na massa:**
1. **App:** no PC do roteiro, faça `kill(getppid(), SIG_SYS_R ou SIG_SYS_W)` e
   **não avance o PC** até ser retomado. O app continua rodando entre mandar o sinal e levar
   o `SIGSTOP`. Uma forma simples de esperar é `pause()` em loop até o kernel liberar.
2. **Kernel:** o handler com `SA_SIGINFO` guarda `si_pid` e R/W numa pequena fila (podem
   chegar duas syscalls seguidas).
3. No loop principal, para cada syscall: `SIGSTOP`, salva o PC e R/W no PCB, muda o estado
   para `BLOCKED`, coloca na fila de D1, manda `SIG_IO_REQ` ao InterController e escalona o
   próximo **na hora**.

**Armadilha:** se o IRQ0 chega junto com a syscall e é tratado primeiro, o processo volta para
a fila de prontos. Trate as syscalls pendentes **antes** do IRQ0 no loop principal.

**Como verificar:** rode A1 com `3R` e A2 e A3 sem syscall. A1 para no PC 3, fica em `T` para
sempre e A2/A3 terminam normalmente.

---

### 6. IRQ1 e fila de D1
**Objetivo:** o processo bloqueado volta 3 s depois e passa à frente de quem saiu por time slice.

**🔸 Decida aqui (ponto C):** os pedidos de E/S rodam **em paralelo** (cada um termina 3 s
depois do próprio pedido) ou **em série** (o D1 atende um por vez)?
- Em paralelo, dois IRQ1 podem ficar pendentes juntos, e `SIGUSR2` **se perde**. Nesse caso
  troque o IRQ1 para um sinal de tempo real (por exemplo, `SIGRTMIN+3`).
- Em série, os IRQ1 ficam pelo menos 3 s distantes e o `SIGUSR2` serve.

Atualize a decisão 7.

**Mão na massa:**
1. **InterController:** o handler de `SIG_IO_REQ` registra o pedido. Em paralelo, cada pedido
   guarda um `t_fim`. Em série, há um contador de pendentes e um único `t_fim`. A cada volta do
   laço, se `agora >= t_fim`, manda o IRQ1.
2. Troque o `sleep(1)` por um controle de tempo absoluto: guarde `proximo_irq0` e durma o que
   falta, porque o aviso de E/S interrompe o `sleep`.
3. **Kernel:** no IRQ1, `pop_front` da fila de D1, muda o estado para `READY` e faz `push_front`
   na fila de prontos. Se a CPU estava ociosa, retoma esse processo direto.

**Armadilha:** sem o passo 2, cada aviso de E/S adianta o IRQ0, e o time slice fica irregular.

**Como verificar:** preveja o log no papel antes de rodar.
- (a) Um processo faz E/S e os outros continuam rodando.
- (b) Dois pedidos quase juntos são liberados na ordem FIFO.
- (c) Todos bloqueiam, a CPU fica ociosa e volta com o IRQ1.
- (d) Um IRQ1 e um IRQ0 chegam juntos, e quem saiu da E/S roda antes.

---

### 7. Término e encerramento
**Objetivo:** os processos que chegam a MAX saem do escalonamento, e no fim não sobra
nenhum zumbi, órfão ou shm.

**🔸 Decida aqui (ponto D):** o kernel encerra sozinho quando todos ficam `FINISHED`, ou só
quando recebe Ctrl+C (`SIGINT`)? O enunciado diz que o kernel é infinito.

**Mão na massa:**
1. Use `sigaction(SIGCHLD)` com `SA_NOCLDSTOP`. Sem essa flag, cada `SIGSTOP` gera um `SIGCHLD`.
2. No loop principal, faça `while ((pid = waitpid(-1, &st, WNOHANG)) > 0)`, mude o estado para
   `FINISHED` e, se era o processo atual, escalone o próximo.
3. No encerramento: `kill` no InterController e em qualquer app vivo, `waitpid` em todos,
   e `shmdt` + `shmctl(IPC_RMID)` se usou shm.

**Como verificar:** depois de sair, `ps -ef | grep -E 'app|intercontroller'` não mostra nada e
`ipcs -m` não mostra segmentos seus. Nenhum app termina com PC ≠ MAX.

---

### 8. Logs e cenários de teste
**Objetivo:** cenários fáceis de repetir, com uma saída que se explica sozinha.

**Mão na massa:**
1. Padronize o log com uma linha por evento:
   `t=03 IRQ0 | RUN=A2 | READY=[A3 A1] | D1=[] | PC: A1=2 A2=1 A3=0`.
2. Crie `testes/` com 2 a 4 configurações: os cenários a–d do passo 6, com 3 e com 6
   processos. Faça um script `run_teste.sh N` para cada uma. Os processos começam com 1 s de
   intervalo, como o enunciado sugere.
3. Para cada teste, escreva a linha do tempo **esperada** num `.txt` antes de rodar.

**Como verificar:** compare o log obtido com o esperado. Se uma linha não tem explicação
pelas regras, há um bug ou o log está incompleto.

---

### 9. Relatório
**Objetivo:** um documento que explique o sistema e o que se espera de cada teste.

**Mão na massa:** monte o relatório a partir do `work_memory.md`, porque as decisões já estão
lá com o porquê. Inclua:
- a lista de programas e o papel de cada um;
- como compilar e executar;
- as decisões de projeto, com justificativa;
- sinais e IPC usados;
- estruturas (PCB e filas);
- para cada teste, a linha do tempo esperada comparada com a obtida;
- limitações conhecidas.

**Como verificar:** alguém que não conhece o código consegue rodar um teste e prever a saída
lendo só o relatório.

---

### 10. Ensaio da apresentação
**Objetivo:** os dois integrantes conseguem explicar qualquer parte do sistema.

**Mão na massa:** rodem um cenário ao vivo e narrem cada linha do log enquanto ela aparece.
Cada um deve explicar sem ler:
- o fluxo de uma syscall;
- o fluxo de um IRQ0 e de um IRQ1;
- o que é salvo na troca de contexto;
- como a prioridade de E/S é garantida.

## Checklist final
- [ ] Tudo em C usando fork/exec, sinais e IPC Unix
- [ ] KernelSim, InterControllerSim e cada Ai são processos Unix separados
- [ ] KernelSim e InterControllerSim rodam em loop infinito e em paralelo
- [ ] Funciona com 3 e com 6 processos de aplicação
- [ ] IRQ0 a cada TimeSlice (1 s ou o valor escolhido e documentado)
- [ ] IRQ1 marca o fim da E/S 3 s depois (ou o valor documentado), conforme o modelo escolhido
- [ ] `SIGSTOP`/`SIGCONT` usados para parar e retomar os Ai
- [ ] Só um Ai executa por vez
- [ ] Syscall bloqueia o processo na hora e ele vai para a fila FIFO de D1
- [ ] Cada IRQ1 libera o primeiro da fila de D1
- [ ] Quem sai da E/S tem prioridade sobre quem saiu por time slice
- [ ] Contexto salvo e restaurado: PC e parâmetros da syscall
- [ ] Todos os Ai terminam com exatamente MAX iterações
- [ ] O kernel trata processos que terminaram e o caso de CPU ociosa
- [ ] Nenhum processo órfão, zumbi ou recurso de IPC sobrando no fim
- [ ] O log mostra linha do tempo, fila de execução e PC de cada processo
- [ ] O relatório lista os programas e os testes e explica o que deve acontecer
- [ ] Todas as decisões do `work_memory.md` estão no relatório com justificativa
- [ ] Nenhum sinal se perde quando dois eventos chegam juntos (testado)
- [ ] A dupla está pronta para apresentar e entregou no prazo (−1 ponto por dia de atraso)

# Plano do Trabalho — INF1316 Trab1 (Kernel Sim)

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

## Decisões tomadas (de `work_memory.md`)
1. **IRQ1 3 s após cada pedido de E/S.** Consequência: o InterControllerSim precisa ser
   avisado de cada pedido. Pela Fig. 1, as aplicações só falam com o KernelSim, então o
   **kernel repassa o aviso** ao InterController.
2. **Syscall por sinal.** Consequências:
   - um sinal sozinho não carrega dados, e o kernel precisa saber **quem** pediu e se foi
     **R ou W**;
   - os sinais padrão (`SIGUSR1/2`) **não se acumulam**: dois iguais pendentes viram um só.
3. **Um PCB por processo, dentro do KernelSim.**
4. **Enum de estados** (por exemplo, `READY`, `RUNNING`, `BLOCKED`, `FINISHED`). Um processo
   que chega a MAX vai para `FINISHED` e não volta para a fila de prontos.
5. **CPU ociosa:** o kernel fica ocioso até o próximo IRQ1.
6. **R/W:** a diferença aparece só no log e no contexto salvo; o comportamento é o mesmo.

## Pontos que ainda faltam decidir
As decisões acima deixam estas perguntas em aberto. Responda no `work_memory.md` antes do passo 3.

- **A. Mapa de sinais.** Liste todos os eventos que viram sinal:
  - IRQ0 (InterController → kernel);
  - IRQ1 (InterController → kernel);
  - syscall R e syscall W (Ai → kernel);
  - aviso de pedido de E/S (kernel → InterController).

  Isso dá mais eventos do que `SIGUSR1/2`. Pesquise **sinais de tempo real**
  (`SIGRTMIN+n`), que se acumulam, e `sigqueue` com `SA_SIGINFO`, que permite mandar um
  inteiro junto e informa o pid de quem enviou em `si_pid`. Decida qual sinal representa cada evento.
- **B. Como o PC chega ao kernel.** Na syscall o processo pode mandar o PC junto. Mas no
  **IRQ0** o kernel interrompe o processo sem aviso. Como ele fica sabendo do PC naquele
  momento? Decida e justifique se o PC também vai por sinal ou se você aceita um pequeno
  canal de IPC só para isso, como memória compartilhada. Defina também o que "restaurar o PC"
  significa na sua implementação.
- **C. Vários pedidos de E/S pendentes.** Cada pedido termina 3 s depois do **seu próprio**
  pedido, ou o D1 atende um de cada vez, com o segundo começando quando o primeiro termina?
  Isso muda como o InterController guarda os pedidos pendentes.
- **D. Fim da simulação.** Quando todos ficam `FINISHED`, o kernel continua ocioso para
  sempre? Defina como encerrar (por exemplo, Ctrl+C → matar os filhos e sair).

## Plano

### 1. Fechar os pontos em aberto (A–D)
**Objetivo:** completar o `work_memory.md` com o mapa de sinais, o mecanismo do PC, o modelo
de E/S e a forma de encerrar. Inclua também os valores de MAX e o "roteiro" de syscalls de
cada Ai (em qual PC cada um faz R ou W).

**O que você precisa entender:** um processo parado por `SIGSTOP` não roda nada, nem os
handlers, até receber `SIGCONT`. Além disso, entre o Ai mandar o sinal de syscall e o kernel
mandar o `SIGSTOP`, o Ai **continua executando**.

**Como verificar:** desenhe um diagrama de sequência para uma syscall completa: Ai → kernel →
InterController → (3 s) → kernel → Ai. Cada seta precisa ter um sinal definido e cada dado
precisa de um meio de transporte.

### 2. Desenhar as estruturas de dados do kernel
**Objetivo:** ter o PCB e as filas definidos no papel ou num `.h`.

**O que fazer:** Defina o enum de estados (decisão 4) e o que o PCB guarda: pid, nome,
estado, PC salvo, parâmetros da syscall pendente (dispositivo e R/W) e, se quiser, contadores
para o relatório.
Defina a fila de prontos e a fila de bloqueados de D1, as duas FIFO.
Defina também onde entra a regra de prioridade: quem sai da E/S entra antes de quem saiu
por time slice.

**Como verificar:** simule no papel um cenário com 3 processos, onde 2 fazem E/S. Escreva o
estado das filas a cada segundo. Se você consegue preencher a tabela sem "inventar" uma
regra nova, as estruturas bastam.

### 3. Fazer o programa de aplicação (Ai) sozinho
**Objetivo:** um executável que recebe identificação e parâmetros por `argv`, faz o loop até
MAX com `sleep(1)`, incrementa o PC e imprime o progresso.

**O que fazer:** Primeiro sem syscall nenhuma. Depois acrescente os pontos do roteiro onde
ele faz `syscall(D1, R/W)`: manda o sinal (ou sinais) definido no ponto A para o kernel, que
recebe o pid por argumento ou por `getppid()`. Depois disso, **não avança o PC** até ser
retomado. Lembre que o `SIGSTOP` do kernel pode chegar um pouco depois do sinal.

**Como verificar:**
- Rode sozinho e, em outro terminal, use `kill -STOP <pid>` e `kill -CONT <pid>`. O PC tem
  que pausar e continuar sem pular nem repetir números.
- Para a syscall, use um receptor de teste que imprime o sinal, o `si_pid` e o valor recebido.
  O Ai deve ficar parado esperando até levar `SIGCONT`.

### 4. Fazer o InterControllerSim sozinho
**Objetivo:** um processo infinito que manda IRQ0 a cada 1 s e IRQ1 conforme o modelo
escolhido no passo 1.

**O que fazer:** Implemente o laço com `sleep()`. A cada volta ele manda o IRQ0 e atualiza
os pedidos pendentes (decisão 1 e ponto C). Quando um pedido completa 3 s, ele manda o IRQ1.
O aviso de "novo pedido" chega do kernel por sinal (ponto A); o handler só registra o pedido.
O destino dos sinais é o pid do kernel, recebido por `getppid()` ou por argumento.

**O que você precisa entender:** um sinal que chega durante o `sleep()` faz ele **voltar
antes da hora**. Sem tratar isso, o relógio do IRQ0 se adianta. Teste se o IRQ0 continua
regular quando chegam avisos de pedido.

**Como verificar:** use um receptor fake que imprime os sinais com hora e, com `kill`/`sigqueue`
manual, simule avisos de pedido. O IRQ0 deve sair a cada 1 s, e cada IRQ1 deve sair 3 s
depois do pedido correspondente, na ordem dos pedidos. Teste com 2 pedidos próximos.

### 5. Esqueleto do KernelSim: criar processos e receber IRQs
**Objetivo:** o kernel faz `fork`+`exec` do InterController e de A1..An e recebe os sinais.

**O que fazer:** Crie os filhos e pare cada Ai logo depois de criado, para que só um rode
de cada vez. Instale os handlers das IRQs e de `SIGCHLD`. Nos handlers, faça o mínimo, como
marcar uma flag, e trate o evento no loop principal.

**O que você precisa entender:** o problema de corrida em que um Ai começa a rodar antes de
você parar ele. Pense em como garantir que nenhum Ai executa iteração nenhuma antes de ser
escalonado.

**Como verificar:** use `ps -o pid,stat,cmd` para ver que só um Ai fica em estado `S/R` e os
outros ficam em `T` (stopped). O kernel deve imprimir cada IRQ recebida.

### 6. Round-robin com IRQ0
**Objetivo:** time-sharing funcionando, ainda sem E/S.

**O que fazer:** No IRQ0: salve o contexto (PC) do processo atual, dê `SIGSTOP` nele, coloque
no fim da fila de prontos, tire o próximo da fila e dê `SIGCONT`. Imprima uma linha de log com
o tempo, quem está executando, a fila de prontos e o PC de cada processo.

**Como verificar:** rode com 3 processos e sem syscalls. Eles têm que se alternar a cada
1 s, e cada um tem que terminar com exatamente MAX iterações.

### 7. Syscall e fila de bloqueados com IRQ1
**Objetivo:** E/S funcionando.

**O que fazer:** Quando o kernel recebe o sinal de syscall, use o `si_pid` para descobrir
quem pediu, e então:
- para o processo na hora;
- salva o PC e R/W no PCB e muda o estado para `BLOCKED`;
- coloca o processo na fila de D1;
- **avisa o InterController**;
- escalona outro processo **imediatamente**, sem esperar o próximo IRQ0.

No IRQ1, tire o primeiro da fila de D1, mude para `READY` e coloque na fila de prontos **com
prioridade**. Se não houver ninguém pronto, o kernel fica ocioso (decisão 5). Quando chegar
um IRQ1, ele retoma direto o processo liberado.

**O que você precisa entender:** cuidado com o IRQ0 chegando **entre** o sinal de syscall e
o tratamento dele. O processo que já pediu E/S não pode voltar para a fila de prontos.

**O que você precisa entender:** a diferença entre o processo sair da CPU por time slice e
sair por syscall. A segunda forma não pode "devolver" o processo à fila de prontos.

**Como verificar:** monte cenários e preveja o log no papel antes de rodar:
- (a) um processo faz E/S e os outros continuam rodando;
- (b) dois processos fazem E/S quase juntos e o IRQ1 libera na ordem FIFO;
- (c) todos ficam bloqueados e a CPU fica ociosa;
- (d) um processo sai da E/S ao mesmo tempo que outro perde o time slice, e o que saiu da E/S vai na frente.

### 8. Término dos processos e encerramento limpo
**Objetivo:** o kernel tira da fila os processos que terminaram e sai sem deixar lixo para trás.

**O que fazer:**
- Trate `SIGCHLD` com `waitpid(WNOHANG)` e mude o estado para `FINISHED` (decisão 4).
- Se o processo que terminou era o que estava executando, escalone outro.

  Atenção: `SIGCHLD` também chega quando um filho é **parado** por `SIGSTOP`. Veja a flag
  `SA_NOCLDSTOP`.
- Implemente o encerramento do ponto D: matar os filhos e liberar a IPC extra, se houver.

**Como verificar:** depois de encerrar, `ps` não pode mostrar processos órfãos ou zumbis, e
`ipcs -m` não pode mostrar segmentos sobrando, se você usou shm. Confira também se algum
processo terminou com PC ≠ MAX.

### 9. Logs e programas de teste para a apresentação
**Objetivo:** uma saída que mostre o comportamento sem precisar abrir o código, e cenários fáceis de repetir.

**O que fazer:** Padronize uma linha de log por evento: tempo relativo, evento (IRQ0, IRQ1,
SYSCALL, FIM), processo em execução, fila de prontos, fila de D1 e PC de cada um. Prepare de
2 a 4 configurações de teste, como os cenários do passo 7 com 3 e com 6 processos. Os
processos devem começar com intervalo de 1 s, como o enunciado sugere.

**Como verificar:** pegue um log e confira à mão se cada transição segue as regras. Se uma
linha não tem explicação pelas regras, há um bug ou o log está incompleto.

### 10. Relatório
**Objetivo:** um documento que explique o sistema e o que se espera de cada teste.

**O que fazer:** Inclua:
- a lista de programas e o papel de cada um;
- como compilar e executar;
- as decisões de projeto do passo 1, com justificativa;
- sinais e IPC usados;
- estruturas (PCB e filas);
- para cada teste, a linha do tempo **esperada** comparada com a **obtida**;
- limitações conhecidas.

**Como verificar:** alguém que não conhece o código deve conseguir rodar um teste e prever a
saída lendo só o relatório.

### 11. Ensaio da apresentação
**Objetivo:** os dois integrantes conseguem explicar qualquer parte do sistema.

**O que fazer:** Cada um deve explicar sem ler:
- o fluxo de uma syscall;
- o fluxo de um IRQ0 e de um IRQ1;
- o que é salvo na troca de contexto;
- como a prioridade de E/S é garantida.

**Como verificar:** rodem um cenário ao vivo e narrem cada linha do log enquanto ela aparece.

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
- [ ] As decisões 1–6 e os pontos A–D estão documentados no relatório com justificativa
- [ ] Nenhum sinal se perde quando dois eventos chegam juntos (testado)
- [ ] A dupla está pronta para apresentar e entregou no prazo (−1 ponto por dia de atraso)

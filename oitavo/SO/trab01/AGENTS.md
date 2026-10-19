# INF1316 — Trab1 (KernelSim)

Instruções para qualquer assistente de IA (Claude, Codex, Cursor, Copilot, Gemini...) usado
neste trabalho.

## Ao iniciar uma sessão
Antes de responder a primeira mensagem, leia:
1. `work_settings/roadmap.md`: o plano do trabalho e a seção **Progresso**, que mostra o passo atual.
2. `work_settings/work_memory.md`: as decisões de projeto já tomadas.

Depois diga em uma ou duas linhas em qual passo estamos e qual é o próximo item a fazer.

## Estilo da tutoria (mão na massa)
- **Prazo apertado (decidido pelo aluno):** o assistente escreve o código de cada passo e explica
  as linhas importantes; o aluno revisa e roda o "Como verificar".
- **Prática primeiro, teoria junto.** Cada resposta deve terminar com uma ação concreta para o
  aluno, por exemplo "rode `comando` e confira Y". Nada de aula longa antes do código.
- **Um passo por vez.** Entregue um passo do roadmap que compila e roda e só avance depois que o
  aluno conferir o "Como verificar".
- **Conceito colado no código.** Explique o conceito de SO no momento em que ele aparece numa
  linha, em 1 a 3 frases. Diga para que serve a linha, o que quebra sem ela e qual ideia de SO
  ela mostra. Prefira linguagem simples a jargão.
- **Foco na entrega.** Nada de experimentos paralelos ou simulações à parte. Os únicos testes
  são os do "Como verificar" de cada passo, rodando o próprio sistema.
- Dê esqueletos e assinaturas (nomes de funções, chamadas de sistema e flags que vai usar),
  mas deixe o aluno escrever o corpo. Se ele travar, dê uma dica mais específica antes de
  mostrar a solução.
- Ao revisar o código do aluno, aponte o problema e pergunte ou dê uma pista; não reescreva.
- Decisões de projeto aparecem só no passo em que passam a importar (blocos `🔸 Decida aqui`
  do roadmap). Apresente as opções, espere a escolha e registre no `work_memory.md`.
- Não invente regras: o que o enunciado não define é decisão do aluno.
- Não crie arquivos descartáveis de teste. Teste com `kill`, `ps`, `pgrep`, `ipcs` ou com o
  próprio componente real.
- Ao terminar um passo, marque-o no **Progresso** do roadmap.

## Projeto
- Enunciado: `work_settings/INF1316 - Trab1 - SO - 2026-2.pdf`.
- Compilar com `make`. Os executáveis (`kernel`, `intercontroller`, `app`) ficam fora do git.
- Sinais e constantes compartilhados ficam em `common.h`.

## Git
Commits em português, sem `Co-Authored-By` e sem nenhuma menção a IA.

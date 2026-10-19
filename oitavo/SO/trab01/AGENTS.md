# INF1316 — Trab1 (KernelSim)

Instruções para qualquer assistente de IA (Claude, Codex, Cursor, Copilot, Gemini...) usado
neste trabalho.

## Ao iniciar uma sessão
Antes de responder a primeira mensagem, leia:
1. `work_settings/roadmap.md`: o plano do trabalho e a seção **Progresso**, que mostra o passo atual.
2. `work_settings/work_memory.md`: as decisões de projeto já tomadas.

Depois diga em uma ou duas linhas em qual passo estamos e qual é o próximo item a fazer.

## Estilo da tutoria
- O aluno escreve o código e o assistente explica. Só escreva ou corrija código quando o aluno pedir.
- Explique o **porquê de cada linha**: para que serve, o que quebra sem ela e qual conceito de
  SO ela mostra. Prefira linguagem simples a jargão.
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

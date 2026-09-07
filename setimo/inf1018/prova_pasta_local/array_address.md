# Cálculo de Endereço de Array em Assembly x86-64

## Fórmula geral

```
endereço de v[i] = base + i * sizeof(tipo)
```

Em assembly, isso se traduz em um **modo de endereçamento escalado**:

```asm
(%base, %indice, escala)
# endereço = base + indice * escala
```

---

## Array simples na pilha

Para `int v[5]` com base em `-24(%rbp)` e índice `i` em `%rax`:

```c
int v[5];   // 5 * 4 = 20 bytes reservados na pilha
v[i] = 7;
```

```asm
subq $20, %rsp              # reserva espaço

movslq -4(%rbp), %rax      # rax = i (sign-extend int → 64-bit)
movl $7, -24(%rbp, %rax, 4) # v[i] = 7  →  endereço = rbp-24 + rax*4
```

> O índice **deve ser 64-bit** (`%rax`, não `%eax`) para usar no modo de endereçamento.

---

## Array de structs na pilha

Para `struct exam { float val; int tipo; }  temp[5]`:

- `sizeof(struct exam)` = 8 bytes (float 4 + int 4)
- `temp[i].val`  = base + i*8 + 0
- `temp[i].tipo` = base + i*8 + 4

Supondo `temp[0]` em `-56(%rbp)` e `i` em `-4(%rbp)`:

```asm
movslq -4(%rbp), %rax           # rax = i

# temp[i].val  (offset 0 na struct)
movss %xmm0, -56(%rbp, %rax, 8)

# temp[i].tipo (offset 4 na struct)
movl %ecx,   -52(%rbp, %rax, 8)  # -56 + 4 = -52
```

---

## Quando a escala não é potência de 2

As escalas válidas são **1, 2, 4, 8** apenas.  
Para structs de tamanho diferente (ex: 12 bytes), é necessário calcular manualmente:

```asm
movslq -4(%rbp), %rax   # rax = i
imulq $12, %rax         # rax = i * 12
# agora usa rax como offset: -base(%rbp, %rax, 1)
```

---

## Resumo do layout de pilha típico

```
rbp  →  [ saved rbp        ]  (8 bytes)
-8   →  [ variável local 1 ]
-12  →  [ variável local 2 ]
-16  →  [ i (int)          ]
-56  →  [ temp[0]          ]  (8 bytes cada struct)
-48  →  [ temp[1]          ]
-40  →  [ temp[2]          ]
-32  →  [ temp[3]          ]
-24  →  [ temp[4]          ]
```

> Variáveis com endereços menores (mais negativos) ficam **mais abaixo** na pilha.

---

## Erros comuns

| Erro | Problema |
|------|----------|
| Usar `%eax` (32-bit) no modo de endereçamento | Inválido em 64-bit; use `%rax` |
| Offset positivo em `(%rax, %rbp)` | Aponta acima do `%rbp`, fora da área local |
| Esquecer de fazer `movslq` para o índice | `%eax` com bit 31=1 vira número enorme em 64-bit |
| Usar `movsd` para armazenar `float` | `movsd` é double (8 bytes); use `movss` para float (4 bytes) |

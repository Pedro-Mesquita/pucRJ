#!/usr/bin/env python3
"""Valida os resultados produzidos por matrix_lib_test.c.

Le as matrizes binarias de float32 (row-major, sem cabecalho) e compara
os resultados do programa em C com o calculo feito em numpy.

A comparacao usa erro relativo, do mesmo modo que check_linear_errors()
e check_mult_errors() em matrix_lib_test.c: implementacoes diferentes
somam na mesma ordem, entao igualdade exata de float nao e um criterio
valido -- sobretudo depois que FMA entra em cena.

Uso:
    python3 scripts/validate_results.py -s 2.5 -r 8 -c 8 -C 8 \
        -m data/inputs/8x8/A.dat -M data/inputs/8x8/B.dat \
        -o data/outputs/o1.dat -O data/outputs/o2.dat

O argumento -O e opcional: quando matrix_matrix_mult ainda nao esta
implementada (benchmarks escalares), basta omitir.
"""

import argparse
import pathlib
import sys

import numpy as np

TOL = 1e-4  # mesma tolerancia usada em matrix_lib_test.c


def carregar(caminho: str, rows: int, cols: int) -> np.ndarray:
    """Le um arquivo binario de float32 e o reorganiza em rows x cols."""
    arq = pathlib.Path(caminho)
    if not arq.is_file():
        sys.exit(f"arquivo nao encontrado: {caminho}")

    dados = np.fromfile(arq, dtype=np.float32)
    if dados.size != rows * cols:
        sys.exit(f"{caminho}: esperados {rows * cols} floats, "
                 f"encontrados {dados.size}")
    return dados.reshape(rows, cols)


def comparar(nome: str, esperado: np.ndarray, obtido: np.ndarray,
             escala: np.ndarray = None) -> bool:
    """Compara duas matrizes por erro relativo e imprime um resumo.

    `escala`, quando dada, substitui |esperado| no denominador. Para o
    produto de matrizes usa-se (|A| @ |B|)[i, j]: com entradas em [-1, 1]
    varios C[i, j] ficam perto de zero por cancelamento e o erro relativo a
    |C[i, j]| explode sem que haja erro real; a soma dos modulos dos produtos
    e o limite natural do erro de arredondamento de um produto escalar.
    """
    # denominador protegido: evita divisao por zero em elementos nulos
    denom = np.maximum(np.abs(esperado) if escala is None else escala, 1e-30)
    erro_rel = np.abs(esperado - obtido) / denom
    pior = int(erro_rel.argmax())
    i, j = divmod(pior, esperado.shape[1])
    ok = bool(erro_rel.max() <= TOL)

    print(f"{nome}:")
    print(f"  erro absoluto maximo : {np.abs(esperado - obtido).max():.6e}")
    print(f"  erro relativo maximo : {erro_rel.max():.6e}  (tolerancia {TOL:g})")
    print(f"  pior elemento        : [{i}, {j}] "
          f"esperado {esperado[i, j]:.8f} / obtido {obtido[i, j]:.8f}")
    print(f"  resultado            : {'OK' if ok else 'FALHOU'}")
    return ok


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("-s", "--scalar", type=float, required=True)
    p.add_argument("-r", "--rows1", type=int, required=True)
    p.add_argument("-c", "--cols1", type=int, required=True,
                   help="colunas de A (= linhas de B)")
    p.add_argument("-C", "--cols2", type=int,
                   help="colunas de B; obrigatorio junto com -M/-O")
    p.add_argument("-m", "--matrix1", required=True)
    p.add_argument("-M", "--matrix2")
    p.add_argument("-o", "--output1", required=True)
    p.add_argument("-O", "--output2")
    args = p.parse_args()

    tudo_ok = True

    # Parte 1: result1 = scalar * A
    a = carregar(args.matrix1, args.rows1, args.cols1)
    r1 = carregar(args.output1, args.rows1, args.cols1)
    tudo_ok &= comparar("scalar_matrix_mult", np.float32(args.scalar) * a, r1)

    # Parte 2: result2 = A * B (so quando os arquivos da parte 2 sao passados)
    if args.output2:
        if not (args.matrix2 and args.cols2):
            sys.exit("-O exige tambem -M e -C")
        b = carregar(args.matrix2, args.cols1, args.cols2)
        r2 = carregar(args.output2, args.rows1, args.cols2)
        # acumula em float64 para que a referencia seja mais precisa
        # que qualquer uma das implementacoes em C sendo testadas
        a64, b64 = a.astype(np.float64), b.astype(np.float64)
        esperado = (a64 @ b64).astype(np.float32)
        escala = (np.abs(a64) @ np.abs(b64)).astype(np.float32)
        print()
        tudo_ok &= comparar("matrix_matrix_mult", esperado, r2, escala)

    sys.exit(0 if tudo_ok else 1)


if __name__ == "__main__":
    main()

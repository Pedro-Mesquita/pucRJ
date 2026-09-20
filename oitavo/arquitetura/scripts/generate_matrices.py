#!/usr/bin/env python3
"""Gera matrizes binarias de floats (float32) para os benchmarks.

O formato e o mesmo esperado por load_matrix() em matrix_lib_test.c:
apenas os N = rows * cols floats de 32 bits, em ordem row-major,
sem cabecalho e no endianness nativo da maquina.

Uso:
    python3 scripts/generate_matrices.py            # gera 8x8
    python3 scripts/generate_matrices.py 8 16 1024  # gera os tamanhos pedidos
    python3 scripts/generate_matrices.py 16x32      # nao quadrada: A 16x32, B 32x16

Para "RxC", A tem R linhas e C colunas e B tem C linhas e R colunas, de modo
que A x B continua bem definido para matrix_lib_test.c (-r R -c C -C R).
"""

import argparse
import pathlib
import sys

import numpy as np

ROOT = pathlib.Path(__file__).resolve().parent.parent
INPUTS = ROOT / "data" / "inputs"

# semente fixa: as mesmas matrizes sao usadas por todas as implementacoes,
# o que e requisito para comparacoes de desempenho e de corretude justas.
SEED = 42

# linhas geradas por vez (ver comentario em gerar)
BLOCO_LINHAS = 1024


def gerar(rows: int, cols: int, low: float, high: float) -> None:
    """Cria A.dat (rows x cols) e B.dat (cols x rows) em data/inputs/<rows>x<cols>/."""
    if rows % 8 != 0 or cols % 8 != 0:
        sys.exit(f"tamanho invalido: {rows}x{cols} (precisa ser multiplo de 8)")

    dest = INPUTS / f"{rows}x{cols}"
    dest.mkdir(parents=True, exist_ok=True)

    # quadradas mantem SEED + size (compativel com os dados ja gerados);
    # nao quadradas usam uma semente que nao colide com nenhuma quadrada
    semente = SEED + rows if rows == cols else SEED + rows * cols + cols
    rng = np.random.default_rng(semente)
    for nome, shape in (("A", (rows, cols)), ("B", (cols, rows))):
        arq = dest / f"{nome}.dat"
        # gera em blocos de linhas: uniform() produz um float64 intermediario
        # (8 bytes/elemento) e uma 24000x24000 estouraria a memoria. O fluxo
        # de numeros do gerador e o mesmo em blocos ou de uma vez, entao os
        # arquivos ficam identicos aos gerados sem blocos.
        with open(arq, "wb") as f:
            for inicio in range(0, shape[0], BLOCO_LINHAS):
                linhas = min(BLOCO_LINHAS, shape[0] - inicio)
                rng.uniform(low, high, size=(linhas, shape[1])) \
                   .astype(np.float32).tofile(f)
        print(f"{arq.relative_to(ROOT)}: {shape[0]}x{shape[1]} = "
              f"{shape[0] * shape[1]} floats, {arq.stat().st_size} bytes")


def dimensoes(texto: str) -> tuple:
    """Converte "N" em (N, N) e "RxC" em (R, C)."""
    partes = texto.lower().split("x")
    if len(partes) == 1:
        return int(partes[0]), int(partes[0])
    if len(partes) == 2:
        return int(partes[0]), int(partes[1])
    sys.exit(f"tamanho invalido: {texto} (use N ou RxC)")


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("sizes", nargs="*", default=["8"],
                   help="N (quadrada) ou RxC, multiplos de 8; padrao: 8")
    p.add_argument("--low", type=float, default=-1.0)
    p.add_argument("--high", type=float, default=1.0)
    args = p.parse_args()

    for size in args.sizes:
        rows, cols = dimensoes(size)
        gerar(rows, cols, args.low, args.high)


if __name__ == "__main__":
    main()

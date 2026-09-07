#!/usr/bin/env python3
"""Gera matrizes binarias de floats (float32) para os benchmarks.

O formato e o mesmo esperado por load_matrix() em matrix_lib_test.c:
apenas os N = rows * cols floats de 32 bits, em ordem row-major,
sem cabecalho e no endianness nativo da maquina.

Uso:
    python3 scripts/generate_matrices.py            # gera 8x8
    python3 scripts/generate_matrices.py 8 16 1024  # gera os tamanhos pedidos
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


def gerar(size: int, low: float, high: float) -> None:
    """Cria A.dat e B.dat (size x size) em data/inputs/<size>x<size>/."""
    if size % 8 != 0:
        sys.exit(f"tamanho invalido: {size} (precisa ser multiplo de 8)")

    dest = INPUTS / f"{size}x{size}"
    dest.mkdir(parents=True, exist_ok=True)

    rng = np.random.default_rng(SEED + size)
    for nome in ("A", "B"):
        m = rng.uniform(low, high, size=(size, size)).astype(np.float32)
        arq = dest / f"{nome}.dat"
        m.tofile(arq)
        print(f"{arq.relative_to(ROOT)}: {size}x{size} = "
              f"{m.size} floats, {arq.stat().st_size} bytes")


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("sizes", nargs="*", type=int, default=[8],
                   help="dimensoes quadradas (multiplos de 8); padrao: 8")
    p.add_argument("--low", type=float, default=-1.0)
    p.add_argument("--high", type=float, default=1.0)
    args = p.parse_args()

    for size in args.sizes:
        gerar(size, args.low, args.high)


if __name__ == "__main__":
    main()

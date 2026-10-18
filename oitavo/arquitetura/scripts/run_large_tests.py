#!/usr/bin/env python3
"""Testa scalar_matrix_mult e matrix_matrix_mult em varios tamanhos de matriz,
de 8x8 ate o maior que a maquina aguentar (sempre multiplos de 8).

Para cada tamanho e cada versao, roda o matrix_lib_test.c do professor (sem
alteracoes), captura os tempos que ele imprime e valida as saidas com numpy.
Nao e o benchmark: e um teste de correcao em escala, com os tempos anotados
apenas como referencia.

Por que a validacao do produto nao fica so com check_mult_errors(): ele faz a
multiplicacao convencional (linha x coluna, sem otimizacao) em -O0, que para
10000x10000 leva horas. Como o programa grava result2 ANTES de chamar
check_mult_errors(), a partir de LIMITE_CHECK_C o processo e encerrado assim
que a linha "[221] Checking matrix result" aparece e a validacao e feita pelo
numpy (referencia em float64). O tempo ja foi impresso antes desse ponto.

O escalar e validado em blocos (np.memmap) para nao carregar 3 matrizes de
2 GB na memoria de uma vez; as saidas de tamanhos >= APAGAR_SAIDA_A_PARTIR
sao apagadas depois de validadas para nao encher o disco.

Os tamanhos sao fixos (TAMANHOS, 8 ate 4000 x 4000), numa unica execucao.

Uso:
    python3 scripts/run_large_tests.py
    python3 scripts/run_large_tests.py --skip-matrix          # so o escalar
    python3 scripts/run_large_tests.py --scalar col,increment
    python3 scripts/run_large_tests.py --scalar ""            # so o produto

Binarios (compilados pelo proprio script, mesmas flags do benchmark):
    build/test_<versao>          <- benchmarks/scalar/scalar_<versao>.c
    build/test_matrix_<versao>   <- FONTES_MATRIX[versao]
"""

import argparse
import csv
import pathlib
import subprocess
import sys
import time

import numpy as np

ROOT = pathlib.Path(__file__).resolve().parent.parent
INPUTS = ROOT / "data" / "inputs"
OUTPUTS = ROOT / "data" / "outputs"
RESULTS = ROOT / "results" / "large_tests.csv"
BUILD = ROOT / "build"

CFLAGS = ["-Wall", "-std=c11", "-mfma", "-O0", f"-I{ROOT}"]

FONTES_MATRIX = {
    "row_partial": ROOT / "benchmarks" / "matrix" / "matrix_matrix.c",
}

TAMANHOS = [8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4000]
SCALAR = 2.5
TOL = 1e-4
LIMITE_CHECK_C = 2048          # daqui em diante pula o check_mult_errors()
APAGAR_SAIDA_A_PARTIR = 4000   # daqui em diante apaga o .dat de saida
BLOCO_LINHAS = 512             # linhas por bloco na validacao do escalar


def compilar(fonte: pathlib.Path, binario: pathlib.Path) -> None:
    cmd = ["gcc", *CFLAGS, "-o", str(binario), str(ROOT / "matrix_lib_test.c"), str(fonte)]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        sys.exit(f"falha ao compilar {fonte.name}:\n{r.stderr}")
    if r.stderr.strip():
        print(f"   avisos em {fonte.name}:\n{r.stderr}")


def memmap(caminho: pathlib.Path, rows: int, cols: int) -> np.memmap:
    if not caminho.is_file():
        sys.exit(f"arquivo nao encontrado: {caminho}")
    esperado = rows * cols * 4
    if caminho.stat().st_size != esperado:
        sys.exit(f"{caminho}: esperados {esperado} bytes, "
                 f"encontrados {caminho.stat().st_size}")
    return np.memmap(caminho, dtype=np.float32, mode="r", shape=(rows, cols))


def rodar(binario: pathlib.Path, n: int, tag: str, matar_no_check: bool):
    """Executa o programa do professor.

    Devolve (tempo_escalar_ms, tempo_matriz_ms, o1, o2, mensagens). Se
    matar_no_check for True, encerra o processo assim que a linha do
    check_mult_errors() aparece (result2 ja foi gravado nesse ponto).
    """
    entrada = INPUTS / f"{n}x{n}"
    o1 = OUTPUTS / f"{tag}_{n}_o1.dat"
    o2 = OUTPUTS / f"{tag}_{n}_o2.dat"
    cmd = ["stdbuf", "-oL", str(binario), "-s", str(SCALAR),
           "-r", str(n), "-c", str(n), "-C", str(n),
           "-m", str(entrada / "A.dat"), "-M", str(entrada / "B.dat"),
           "-o", str(o1), "-O", str(o2)]

    t_escalar = t_matriz = None
    mensagens = []
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, text=True)
    for linha in proc.stdout:
        linha = linha.rstrip()
        if "Scalar product:" in linha:
            t_escalar = float(linha.split("Scalar product:")[1].split("ms")[0])
        elif "Matrix product:" in linha:
            t_matriz = float(linha.split("Matrix product:")[1].split("ms")[0])
        elif "error at" in linha or "ERROR" in linha or "Unable" in linha \
                or "alloc" in linha.lower():
            mensagens.append(linha)
        elif matar_no_check and "[221] Checking matrix result" in linha:
            proc.kill()
            break
    proc.wait()
    return t_escalar, t_matriz, o1, o2, mensagens


def validar_escalar(a: np.memmap, o1: pathlib.Path, n: int) -> tuple:
    """Compara o1 com SCALAR * A bloco a bloco; devolve (ok, erro_rel_max)."""
    r1 = memmap(o1, n, n)
    esc = np.float32(SCALAR)
    pior = 0.0
    for i in range(0, n, BLOCO_LINHAS):
        esperado = esc * np.asarray(a[i:i + BLOCO_LINHAS])
        denom = np.maximum(np.abs(esperado), 1e-30)
        pior = max(pior, float((np.abs(esperado - r1[i:i + BLOCO_LINHAS]) / denom).max()))
    del r1
    return pior <= TOL, pior


def validar_matriz(a: np.memmap, b: np.memmap, o2: pathlib.Path, n: int) -> tuple:
    """Compara o2 com A @ B (float64) usando (|A| @ |B|)[i,j] como denominador.

    Dividir por |C[i,j]| nao serve: com entradas em [-1, 1] varios elementos
    de C ficam perto de zero por cancelamento e o erro relativo explode sem
    que haja erro real. A soma dos modulos dos produtos e o limite natural do
    erro de arredondamento de um produto escalar.
    """
    a64 = np.asarray(a, dtype=np.float64)
    b64 = np.asarray(b, dtype=np.float64)
    esperado = (a64 @ b64).astype(np.float32)
    escala = (np.abs(a64) @ np.abs(b64)).astype(np.float32)
    del a64, b64
    r2 = memmap(o2, n, n)
    pior = float((np.abs(esperado - r2) / np.maximum(escala, 1e-30)).max())
    del r2, esperado, escala
    return pior <= TOL, pior


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--scalar", default="col,row,increment", metavar="V1,V2",
                   help="versoes de scalar_matrix_mult (vazio = nenhuma)")
    p.add_argument("--matrix", default=",".join(FONTES_MATRIX), metavar="V1,V2",
                   help="versoes de matrix_matrix_mult (vazio = nenhuma)")
    p.add_argument("--skip-matrix", action="store_true",
                   help="nao testa matrix_matrix_mult")
    p.add_argument("--keep-outputs", action="store_true",
                   help="nao apaga as saidas grandes depois de validar")
    args = p.parse_args()
    args.scalar = [v for v in args.scalar.split(",") if v]
    args.matrix = [] if args.skip_matrix else [v for v in args.matrix.split(",") if v]

    OUTPUTS.mkdir(parents=True, exist_ok=True)
    BUILD.mkdir(exist_ok=True)
    RESULTS.parent.mkdir(parents=True, exist_ok=True)

    print("== compilando")
    bin_scalar, bin_matrix = {}, {}
    for v in args.scalar:
        bin_scalar[v] = BUILD / f"test_{v}"
        compilar(ROOT / "benchmarks" / "scalar" / f"scalar_{v}.c", bin_scalar[v])
    for v in args.matrix:
        bin_matrix[v] = BUILD / f"test_matrix_{v}"
        compilar(FONTES_MATRIX[v], bin_matrix[v])

    novo = not RESULTS.exists()
    falhas = 0
    with open(RESULTS, "a", newline="") as f:
        w = csv.writer(f)
        if novo:
            w.writerow(["size", "funcao", "versao", "tempo_ms", "validacao",
                        "erro_rel_max", "wall_s"])

        for n in TAMANHOS:
            entrada = INPUTS / f"{n}x{n}"
            if not (entrada / "A.dat").is_file():
                print(f"== {n}x{n}: gerando entradas")
                subprocess.run([sys.executable, str(ROOT / "scripts" / "generate_matrices.py"),
                                str(n)], check=True, stdout=subprocess.DEVNULL)
            a = memmap(entrada / "A.dat", n, n)
            print(f"== {n}x{n}")

            for v, binario in bin_scalar.items():
                t0 = time.time()
                # matrix_matrix_mult e stub nas versoes escalares: o programa
                # sai (codigo 10) logo depois do escalar, nao precisa matar
                t_esc, _, o1, _, msgs = rodar(binario, n, v, matar_no_check=False)
                wall = time.time() - t0
                for m in msgs:
                    if "Linear error" in m or "ERROR" in m or "alloc" in m.lower() or "Unable" in m:
                        print(f"      {m}")
                if t_esc is None:
                    print(f"   scalar/{v:<9}: EXECUCAO FALHOU")
                    w.writerow([n, "scalar_matrix_mult", v, "", "FALHOU", "", f"{wall:.1f}"])
                    falhas += 1
                    continue
                ok, erro = validar_escalar(a, o1, n)
                falhas += not ok
                print(f"   scalar/{v:<9}: {t_esc:>12.3f} ms  erro rel {erro:.1e}  "
                      f"{'OK' if ok else 'FALHOU'}  (total {wall:.1f}s)")
                w.writerow([n, "scalar_matrix_mult", v, t_esc, "OK" if ok else "FALHOU",
                            f"{erro:.3e}", f"{wall:.1f}"])
                f.flush()
                if n >= APAGAR_SAIDA_A_PARTIR and not args.keep_outputs:
                    o1.unlink(missing_ok=True)

            if bin_matrix:
                b = memmap(entrada / "B.dat", n, n)
            for v, binario in bin_matrix.items():
                t0 = time.time()
                _, t_mat, o1, o2, msgs = rodar(binario, n, v,
                                               matar_no_check=(n >= LIMITE_CHECK_C))
                wall = time.time() - t0
                for m in msgs:
                    if "Multiplication error" in m or "ERROR" in m or "alloc" in m.lower():
                        print(f"      {m}")
                if t_mat is None:
                    print(f"   matrix/{v:<9}: EXECUCAO FALHOU")
                    w.writerow([n, "matrix_matrix_mult", v, "", "FALHOU", "", f"{wall:.1f}"])
                    falhas += 1
                    continue
                ok, erro = validar_matriz(a, b, o2, n)
                falhas += not ok
                print(f"   matrix/{v:<9}: {t_mat:>12.3f} ms  erro rel {erro:.1e}  "
                      f"{'OK' if ok else 'FALHOU'}  (total {wall:.1f}s)")
                w.writerow([n, "matrix_matrix_mult", v, t_mat, "OK" if ok else "FALHOU",
                            f"{erro:.3e}", f"{wall:.1f}"])
                f.flush()
                if n >= APAGAR_SAIDA_A_PARTIR and not args.keep_outputs:
                    o1.unlink(missing_ok=True)
                    o2.unlink(missing_ok=True)
            del a

    print(f"\n{RESULTS.relative_to(ROOT)} atualizado")
    print("TODOS OS TESTES PASSARAM" if falhas == 0 else f"{falhas} FALHA(S)")
    sys.exit(1 if falhas else 0)


if __name__ == "__main__":
    main()

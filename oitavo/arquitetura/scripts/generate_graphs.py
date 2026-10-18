#!/usr/bin/env python3
"""
Gera os graficos do relatorio a partir dos resultados medidos na IAC2.

Le (somente leitura, nunca altera):
    results/iac2/scalar_results.csv   medianas por tamanho (col,row,increment,avx)
    results/iac2/matrix_results.csv   medianas por tamanho (conventional,matrix,avx)
    results/iac2/scalar_raw.csv       todas as repeticoes
    results/iac2/matrix_raw.csv       todas as repeticoes

Escreve em results/graphs/ (PNG para visualizar, PDF vetorial para o relatorio):
    scalar_tempo         tempo x N, log-log, 4 versoes
    scalar_speedup       speedup vs AVX (col em um painel, row/increment em outro)
    matrix_tempo         tempo x N, log-log, 3 versoes + referencia N^3
    matrix_speedup       speedup (conv/row-partial, conv/avx | row-partial/avx)
    variabilidade        dispersao (max-min)/mediana das repeticoes, por versao
    scalar_stats.csv, matrix_stats.csv   n, min, mediana, max por versao/tamanho

Uso: python3 scripts/generate_graphs.py
Dependencias: matplotlib, numpy (sem pandas).
"""

import csv
import os
import statistics
from collections import defaultdict

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter, NullFormatter

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
IN_DIR = os.path.join(ROOT, "results", "iac2")
OUT_DIR = os.path.join(ROOT, "results", "graphs")

# Cor e marcador fixos por versao, iguais em todos os graficos.
# Paleta categorica validada (daltonismo) do skill dataviz.
STYLE = {
    "col":          dict(color="#2a78d6", marker="s", label="coluna a coluna"),
    "row":          dict(color="#eb6834", marker="o", label="linha a linha"),
    "increment":    dict(color="#1baf7a", marker="^", label="ponteiro incremental"),
    "avx":          dict(color="#eda100", marker="D", label="AVX (8 floats)"),
    "conventional": dict(color="#2a78d6", marker="s", label="convencional (linha x coluna)"),
    "matrix":       dict(color="#eb6834", marker="o", label="somas parciais por linha"),
}
STYLE["avx_m"] = dict(STYLE["avx"], label="somas parciais + AVX")

LINE = dict(linewidth=1.6, markersize=5, markeredgewidth=0.8, markeredgecolor="white")
GRID = dict(color="#d0d0d0", linewidth=0.6)
REF = dict(color="#8a8a8a", linewidth=1.2, linestyle="--")

plt.rcParams.update({
    "font.size": 9,
    "axes.titlesize": 10,
    "axes.labelsize": 9,
    "legend.fontsize": 8,
    "axes.spines.top": False,
    "axes.spines.right": False,
    "axes.edgecolor": "#8a8a8a",
    "figure.dpi": 100,
})


def read_results(name):
    """Le um *_results.csv -> (sizes, {versao: [tempo ou None]})."""
    with open(os.path.join(IN_DIR, name), newline="") as f:
        rows = list(csv.DictReader(f))
    sizes = [int(r["size"]) for r in rows]
    cols = [c for c in rows[0].keys() if c != "size"]
    data = {c: [float(r[c]) if r[c] else None for r in rows] for c in cols}
    return sizes, data


def read_raw(name):
    """Le um *_raw.csv -> {(versao, size): [tempos]}."""
    reps = defaultdict(list)
    with open(os.path.join(IN_DIR, name), newline="") as f:
        for r in csv.DictReader(f):
            reps[(r["versao"], int(r["size"]))].append(float(r["tempo_ms"]))
    return reps


def size_axis(ax, sizes):
    """Eixo X em log2 com um rotulo por tamanho medido (90 graus: 8192 e 10000 sao vizinhos)."""
    ax.set_xscale("log", base=2)
    ax.set_xticks(sizes)
    ax.set_xticklabels([str(s) for s in sizes], rotation=90, fontsize=7.5)
    ax.xaxis.set_minor_formatter(NullFormatter())
    ax.set_xlabel("N (matriz N x N)")


def time_formatter(v, _):
    if v >= 1000:
        return f"{v/1000:g} s"
    return f"{v:g} ms"


def finish(fig, name):
    fig.tight_layout()
    for ext in ("png", "pdf"):
        fig.savefig(os.path.join(OUT_DIR, f"{name}.{ext}"), dpi=200 if ext == "png" else None)
    plt.close(fig)
    print(f"  {name}.png / {name}.pdf")


def plot_series(ax, sizes, values, style):
    xs = [s for s, v in zip(sizes, values) if v is not None]
    ys = [v for v in values if v is not None]
    ax.plot(xs, ys, **style, **LINE)


def speedup(base, opt):
    return [b / o if (b is not None and o is not None) else None for b, o in zip(base, opt)]


# ---------------------------------------------------------------- escalar

def graph_scalar_time(sizes, d):
    fig, ax = plt.subplots(figsize=(7, 4.2))
    for v in ("col", "row", "increment", "avx"):
        plot_series(ax, sizes, d[v], STYLE[v])
    ax.set_yscale("log")
    ax.yaxis.set_major_formatter(FuncFormatter(time_formatter))
    size_axis(ax, sizes)
    ax.set_ylabel("tempo de execucao (mediana)")
    ax.set_title("Escalar x matriz: tempo por tamanho (IAC2, -O0)")
    ax.grid(True, which="major", **GRID)
    ax.legend(frameon=False, loc="upper left")
    finish(fig, "scalar_tempo")


def graph_scalar_speedup(sizes, d):
    fig, (a1, a2) = plt.subplots(1, 2, figsize=(9, 3.8))

    plot_series(a1, sizes, speedup(d["col"], d["avx"]), dict(STYLE["col"], label="coluna / AVX"))
    a1.set_title("Speedup do AVX sobre a versao coluna a coluna")

    plot_series(a2, sizes, speedup(d["row"], d["avx"]), dict(STYLE["row"], label="linha / AVX"))
    plot_series(a2, sizes, speedup(d["increment"], d["avx"]),
                dict(STYLE["increment"], label="ponteiro / AVX"))
    a2.set_title("Speedup do AVX sobre linha e ponteiro")

    for ax in (a1, a2):
        ax.axhline(1, **REF)
        size_axis(ax, sizes)
        ax.set_ylabel("speedup (x)")
        ax.set_ylim(bottom=0)
        ax.grid(True, axis="y", **GRID)
        ax.legend(frameon=False, loc="upper left")
    finish(fig, "scalar_speedup")


# ---------------------------------------------------------------- matriz

def graph_matrix_time(sizes, d):
    fig, ax = plt.subplots(figsize=(7, 4.2))
    # referencia N^3 ancorada no AVX em 1024: reta em log-log. Comeca em 256
    # porque abaixo disso o custo fixo domina e a reta so esticaria o eixo.
    i0 = sizes.index(1024)
    ref_sizes = [s for s in sizes if s >= 256]
    ref = [d["avx"][i0] * (s / 1024) ** 3 for s in ref_sizes]
    ax.plot(ref_sizes, ref, label="crescimento ideal N^3 (ref. AVX em 1024)", **REF)
    plot_series(ax, sizes, d["conventional"], STYLE["conventional"])
    plot_series(ax, sizes, d["matrix"], STYLE["matrix"])
    plot_series(ax, sizes, d["avx"], STYLE["avx_m"])
    ax.set_yscale("log")
    ax.yaxis.set_major_formatter(FuncFormatter(time_formatter))
    size_axis(ax, sizes)
    ax.set_ylabel("tempo de execucao (mediana)")
    ax.set_title("Matriz x matriz: tempo por tamanho (IAC2, -O0)")
    ax.grid(True, which="major", **GRID)
    ax.legend(frameon=False, loc="upper left")
    finish(fig, "matrix_tempo")


def graph_matrix_speedup(sizes, d):
    fig, (a1, a2) = plt.subplots(1, 2, figsize=(9, 3.8))

    plot_series(a1, sizes, speedup(d["conventional"], d["matrix"]),
                dict(STYLE["matrix"], label="convencional / somas parciais"))
    plot_series(a1, sizes, speedup(d["conventional"], d["avx"]),
                dict(STYLE["avx_m"], label="convencional / AVX"))
    a1.set_title("Ganho sobre a versao convencional")

    plot_series(a2, sizes, speedup(d["matrix"], d["avx"]),
                dict(STYLE["avx_m"], label="somas parciais / AVX"))
    a2.set_title("Ganho do AVX sobre as somas parciais")

    for ax, loc in ((a1, "upper left"), (a2, "lower right")):
        ax.axhline(1, **REF)
        size_axis(ax, sizes)
        ax.set_ylabel("speedup (x)")
        ax.set_ylim(bottom=0)
        ax.grid(True, axis="y", **GRID)
        ax.legend(frameon=False, loc=loc)
    finish(fig, "matrix_speedup")


# ---------------------------------------------------------------- repeticoes

def write_stats(reps, versions, name):
    """CSV com n/min/mediana/max por versao e tamanho; retorna dispersao relativa."""
    spread = defaultdict(dict)
    with open(os.path.join(OUT_DIR, name), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["versao", "size", "n", "min_ms", "mediana_ms", "max_ms", "dispersao_pct"])
        for v in versions:
            for (vv, s), t in sorted(reps.items(), key=lambda kv: (kv[0][0], kv[0][1])):
                if vv != v:
                    continue
                med = statistics.median(t)
                pct = 100 * (max(t) - min(t)) / med if med > 0 else 0
                w.writerow([v, s, len(t), f"{min(t):g}", f"{med:g}", f"{max(t):g}", f"{pct:.1f}"])
                spread[v][s] = (pct, len(t))
    print(f"  {name}")
    return spread


def graph_variability(sp_scalar, sp_matrix):
    fig, (a1, a2) = plt.subplots(1, 2, figsize=(9, 3.8))
    for ax, spread, keys, title in (
        (a1, sp_scalar, ("col", "row", "increment", "avx"), "Escalar x matriz (10 repeticoes)"),
        (a2, sp_matrix, ("conventional", "matrix", "avx"), "Matriz x matriz (10 repeticoes)"),
    ):
        for v in keys:
            st = STYLE["avx_m"] if (v == "avx" and ax is a2) else STYLE[v]
            # tamanhos com uma unica execucao nao tem dispersao: ficam fora
            pts = sorted((s, p) for s, (p, n) in spread[v].items() if n > 1)
            ax.plot([s for s, _ in pts], [p for _, p in pts], **st, **LINE)
        sizes = sorted({s for v in keys for s, (_, n) in spread[v].items() if n > 1})
        size_axis(ax, sizes)
        ax.set_ylabel("(max - min) / mediana (%)")
        ax.set_title(title)
        ax.grid(True, axis="y", **GRID)
        ax.legend(frameon=False)
    finish(fig, "variabilidade")


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    print(f"gerando em {OUT_DIR}")

    sizes_s, d_s = read_results("scalar_results.csv")
    sizes_m, d_m = read_results("matrix_results.csv")
    graph_scalar_time(sizes_s, d_s)
    graph_scalar_speedup(sizes_s, d_s)
    graph_matrix_time(sizes_m, d_m)
    graph_matrix_speedup(sizes_m, d_m)

    sp_s = write_stats(read_raw("scalar_raw.csv"), ("col", "row", "increment", "avx"), "scalar_stats.csv")
    sp_m = write_stats(read_raw("matrix_raw.csv"), ("conventional", "matrix", "avx"), "matrix_stats.csv")
    graph_variability(sp_s, sp_m)


if __name__ == "__main__":
    main()

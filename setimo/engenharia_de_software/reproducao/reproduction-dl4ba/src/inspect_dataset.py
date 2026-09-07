"""
Dataset inspection script for dl4ba reproduction.
Saves summary to outputs/dataset_info.txt
"""

import os
import pandas as pd
import sys

OUTPUT_DIR = os.path.join(os.path.dirname(__file__), "..", "outputs")
DATASET_PATH = os.path.join(os.path.dirname(__file__), "..", "gcc_data.csv")

lines = []

def log(msg=""):
    print(msg)
    lines.append(msg)

log("=" * 60)
log("INSPECAO DO DATASET: gcc_data.csv")
log("=" * 60)

df = pd.read_csv(DATASET_PATH)

log(f"\n[1] Formato do dataset")
log(f"    Linhas    : {df.shape[0]}")
log(f"    Colunas   : {df.shape[1]}")
log(f"    Colunas   : {df.columns.tolist()}")

log(f"\n[2] Tipos de dados")
for col, dtype in df.dtypes.items():
    log(f"    {col:<20} {dtype}")

log(f"\n[3] Valores ausentes por coluna")
for col in df.columns:
    n_miss = df[col].isna().sum()
    pct = n_miss / len(df) * 100
    log(f"    {col:<20} {n_miss:>5} ausentes ({pct:.1f}%)")

log(f"\n[4] Desenvolvedores (Assignees)")
n_assignees = df["Assignee"].nunique()
log(f"    Total de desenvolvedores unicos : {n_assignees}")

counts = df["Assignee"].value_counts()
log(f"    Max bugs por desenvolvedor      : {counts.max()}")
log(f"    Min bugs por desenvolvedor      : {counts.min()}")
log(f"    Mediana bugs por desenvolvedor  : {counts.median():.0f}")

log(f"\n[5] Top 10 desenvolvedores com mais bugs")
for dev, cnt in counts.head(10).items():
    log(f"    {dev:<35} {cnt:>4} bugs")

log(f"\n[6] Distribuicao de tamanho do campo Summary")
summary_len = df["Summary"].dropna().str.len()
log(f"    Media    : {summary_len.mean():.1f} caracteres")
log(f"    Mediana  : {summary_len.median():.1f} caracteres")
log(f"    Max      : {summary_len.max()} caracteres")

log(f"\n[7] Distribuicao de tamanho do campo Description")
desc_len = df["Description"].dropna().str.len()
log(f"    Media    : {desc_len.mean():.1f} caracteres")
log(f"    Mediana  : {desc_len.median():.1f} caracteres")
log(f"    Max      : {desc_len.max()} caracteres")

log(f"\n[8] Valores unicos do campo Status")
for status, cnt in df["Status"].value_counts().items():
    log(f"    {status:<30} {cnt:>4}")

log(f"\n[9] Desenvolvedores com ao menos 10 bugs: "
    f"{(counts >= 10).sum()} de {n_assignees}")
log(f"    Desenvolvedores com ao menos 5 bugs : "
    f"{(counts >= 5).sum()} de {n_assignees}")

log("\n" + "=" * 60)
log("Inspecao concluida.")
log("=" * 60)

os.makedirs(OUTPUT_DIR, exist_ok=True)
out_path = os.path.join(OUTPUT_DIR, "dataset_info.txt")
with open(out_path, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print(f"\nSalvo em: {out_path}")

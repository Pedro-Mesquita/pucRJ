"""
Simplified reproduction of:
  "An Empirical Assessment of Different Word Embedding and Deep Learning
   Models for Bug Assignment" (Wang et al., JSS 2024)

Scope:
  - Dataset  : GCC (gcc_data.csv)
  - Features : Summary + Description (concatenated)
  - Model    : TF-IDF + Logistic Regression
  - Metrics  : Top-1, Top-5, Top-10 Accuracy and MRR
  - Validation: 10-fold cross-validation (same as paper)

Note: The original paper uses deep learning models (TextCNN, LSTM, Bi-LSTM)
with pre-trained embeddings (Word2Vec, GloVe, ELMo, BERT). This reproduction
uses TF-IDF + Logistic Regression as a lightweight, reproducible baseline,
since PyTorch 1.12.1 is incompatible with the available Python 3.13 environment.
"""

import os
import re
import csv
import time
import numpy as np
import pandas as pd
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import StratifiedKFold
from sklearn.utils import resample

DATASET_PATH = os.path.join(os.path.dirname(__file__), "..", "gcc_data.csv")
OUTPUT_DIR   = os.path.join(os.path.dirname(__file__), "..", "outputs")

MIN_SAMPLES_PER_CLASS = 10
K_FOLDS               = 10
MAX_FEATURES_TFIDF    = 20000
LR_MAX_ITER           = 1000
RANDOM_SEED           = 1234

os.makedirs(OUTPUT_DIR, exist_ok=True)
lines = []

def log(msg=""):
    print(msg)
    lines.append(str(msg))


# ── helpers ─────────────────────────────────────────────────────────────────

def clean_text(text: str) -> str:
    """Minimal text cleaning matching utils.py from the original repo."""
    if not isinstance(text, str):
        return ""
    text = text.lower()
    text = re.sub(r"[^a-z0-9\s]", " ", text)
    text = re.sub(r"\s+", " ", text).strip()
    return text


def top_k_accuracy(y_true, y_prob, k: int) -> float:
    top_k = np.argsort(y_prob, axis=1)[:, -k:]
    correct = sum(y_true[i] in top_k[i] for i in range(len(y_true)))
    return correct / len(y_true)


def mean_reciprocal_rank(y_true, y_prob) -> float:
    ranked = np.argsort(y_prob, axis=1)[:, ::-1]
    rr = []
    for i, true_label in enumerate(y_true):
        rank_positions = np.where(ranked[i] == true_label)[0]
        if len(rank_positions) > 0:
            rr.append(1.0 / (rank_positions[0] + 1))
        else:
            rr.append(0.0)
    return float(np.mean(rr))


# ── 1. Load & preprocess ─────────────────────────────────────────────────────

log("=" * 60)
log("REPRODUCAO SIMPLIFICADA — dl4ba (GCC dataset)")
log("=" * 60)

log("\n[1] Carregando dataset...")
df = pd.read_csv(DATASET_PATH)
log(f"    Linhas originais : {len(df)}")

# Drop rows missing text or assignee
df = df.dropna(subset=["Summary", "Description", "Assignee"])
log(f"    Apos remover NaN : {len(df)}")

# Concatenate summary + description (same as paper's RQ3 finding)
df["text"] = df["Summary"].apply(clean_text) + " " + df["Description"].apply(clean_text)
df = df[df["text"].str.strip().str.len() > 10]
log(f"    Apos filtrar texto vazio : {len(df)}")

# Keep only developers with >= MIN_SAMPLES_PER_CLASS reports
counts = df["Assignee"].value_counts()
valid_assignees = counts[counts >= MIN_SAMPLES_PER_CLASS].index
df = df[df["Assignee"].isin(valid_assignees)].copy()
log(f"    Apos filtrar classes com <{MIN_SAMPLES_PER_CLASS} amostras : {len(df)}")
log(f"    Numero de desenvolvedores (classes) : {df['Assignee'].nunique()}")


# ── 2. Encode labels ─────────────────────────────────────────────────────────

le = LabelEncoder()
y = le.fit_transform(df["Assignee"].values)
X_text = df["text"].values
n_classes = len(le.classes_)

log(f"\n[2] Preparacao")
log(f"    Total de amostras usadas : {len(X_text)}")
log(f"    Total de classes         : {n_classes}")
log(f"    Folds de cross-validation: {K_FOLDS}")
log(f"    Max features TF-IDF      : {MAX_FEATURES_TFIDF}")


# ── 3. 10-fold cross-validation ──────────────────────────────────────────────

log(f"\n[3] Executando {K_FOLDS}-fold cross-validation...")
skf = StratifiedKFold(n_splits=K_FOLDS, shuffle=True, random_state=RANDOM_SEED)

fold_top1, fold_top5, fold_top10, fold_mrr = [], [], [], []
start_time = time.time()

for fold_idx, (train_idx, test_idx) in enumerate(skf.split(X_text, y), 1):
    X_train, X_test = X_text[train_idx], X_text[test_idx]
    y_train, y_test = y[train_idx], y[test_idx]

    # TF-IDF vectorization (fitted on train only)
    tfidf = TfidfVectorizer(
        max_features=MAX_FEATURES_TFIDF,
        ngram_range=(1, 2),
        sublinear_tf=True,
        min_df=2,
    )
    X_train_vec = tfidf.fit_transform(X_train)
    X_test_vec  = tfidf.transform(X_test)

    # Logistic Regression
    clf = LogisticRegression(
        C=1.0,
        max_iter=LR_MAX_ITER,
        solver="lbfgs",
        multi_class="multinomial",
        random_state=RANDOM_SEED,
        n_jobs=-1,
    )
    clf.fit(X_train_vec, y_train)

    y_prob = clf.predict_proba(X_test_vec)

    k_max = min(10, n_classes)
    t1  = top_k_accuracy(y_test, y_prob, 1)
    t5  = top_k_accuracy(y_test, y_prob, min(5,  n_classes))
    t10 = top_k_accuracy(y_test, y_prob, min(10, n_classes))
    mrr = mean_reciprocal_rank(y_test, y_prob)

    fold_top1.append(t1)
    fold_top5.append(t5)
    fold_top10.append(t10)
    fold_mrr.append(mrr)

    log(f"    Fold {fold_idx:>2}: Top-1={t1:.4f}  Top-5={t5:.4f}  "
        f"Top-10={t10:.4f}  MRR={mrr:.4f}")

elapsed = time.time() - start_time


# ── 4. Aggregate results ─────────────────────────────────────────────────────

log(f"\n[4] Resultados (media ± desvio padrao — {K_FOLDS} folds)")
log(f"    Top-1  Accuracy : {np.mean(fold_top1):.4f} ± {np.std(fold_top1):.4f}")
log(f"    Top-5  Accuracy : {np.mean(fold_top5):.4f} ± {np.std(fold_top5):.4f}")
log(f"    Top-10 Accuracy : {np.mean(fold_top10):.4f} ± {np.std(fold_top10):.4f}")
log(f"    MRR             : {np.mean(fold_mrr):.4f} ± {np.std(fold_mrr):.4f}")
log(f"    Tempo total     : {elapsed:.1f}s")

log(f"\n    Amostras usadas : {len(X_text)}")
log(f"    Classes         : {n_classes}")
log(f"    Modelo          : TF-IDF (max_features={MAX_FEATURES_TFIDF}, "
    f"ngram=(1,2)) + Logistic Regression (C=1.0)")
log(f"    Validacao       : {K_FOLDS}-fold stratified CV")


# ── 5. Save metrics.txt ──────────────────────────────────────────────────────

metrics_path = os.path.join(OUTPUT_DIR, "metrics.txt")
with open(metrics_path, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
print(f"\nMetricas salvas em: {metrics_path}")


# ── 6. Save results.csv ──────────────────────────────────────────────────────

results_path = os.path.join(OUTPUT_DIR, "results.csv")
with open(results_path, "w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f)
    writer.writerow(["fold", "top1", "top5", "top10", "mrr"])
    for i, (t1, t5, t10, mrr) in enumerate(
            zip(fold_top1, fold_top5, fold_top10, fold_mrr), 1):
        writer.writerow([i, f"{t1:.4f}", f"{t5:.4f}", f"{t10:.4f}", f"{mrr:.4f}"])
    writer.writerow([
        "mean",
        f"{np.mean(fold_top1):.4f}",
        f"{np.mean(fold_top5):.4f}",
        f"{np.mean(fold_top10):.4f}",
        f"{np.mean(fold_mrr):.4f}",
    ])
    writer.writerow([
        "std",
        f"{np.std(fold_top1):.4f}",
        f"{np.std(fold_top5):.4f}",
        f"{np.std(fold_top10):.4f}",
        f"{np.std(fold_mrr):.4f}",
    ])
print(f"Resultados por fold salvos em: {results_path}")

log("\n" + "=" * 60)
log("Reproducao concluida.")
log("=" * 60)

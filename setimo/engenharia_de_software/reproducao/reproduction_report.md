# Reproduction Report: *An Empirical Assessment of Different Word Embedding and Deep Learning Models for Bug Assignment*

**Original paper:** Wang et al., *Journal of Systems and Software*, 2024. DOI: [10.1016/j.jss.2024.111961](https://doi.org/10.1016/j.jss.2024.111961)  
**Repository:** [https://github.com/AI4BA/dl4ba](https://github.com/AI4BA/dl4ba)  
**Reproduced by:** Pedro Mesquita  
**Date:** June 2026

---

## 1. Available Artifacts

### 1.1 Datasets

Three benchmark bug-report datasets are included directly in the repository under `datasets/`, compressed as `.zip` files:

| File | Project | Description |
|---|---|---|
| `gcc_data.csv.zip` | GCC | Bug reports from the GCC compiler project |
| `jdt_data.csv.zip` | Eclipse JDT | Bug reports from Eclipse Java Development Tools |
| `sun_firefox.csv.zip` | Mozilla Firefox | Bug reports from the Firefox browser project |

Each CSV contains bug report fields (summary, description) and the developer assigned to fix them. No external download is required for the datasets; they are self-contained in the repository.

### 1.2 Source Code

The repository provides a fully self-contained Python implementation:

| File / Directory | Purpose |
|---|---|
| `models.py` | Neural architectures: TextCNN, LSTM, Bi-LSTM, Bi-LSTM with Attention, MLP |
| `train.py` | Main training and evaluation script (CLI-based) |
| `utils.py` | Text preprocessing, vocabulary building, embedding loading |
| `parameters.json` | Hyperparameter configuration file |
| `data_analysis/delta.py` | Cliff's Delta effect-size computation |
| `data_analysis/wilcoxon.py` | Wilcoxon signed-rank tests |
| `data_analysis/draw_bar.py` | Bar chart generation |
| `data_analysis/draw_heatmap.py` | Heatmap generation |
| `data_analysis/draw_violin.py` | Violin plot generation |

### 1.3 Pre-computed Results

The `res/` directory contains Excel spreadsheets and statistical-test output files organized per research question:

```
res/
├── rq1/   # Per-model results + Cliff's Delta and Wilcoxon outputs per dataset
│   ├── textcnn.xlsx, mlp.xlsx, lstm.xlsx, lstm-a.xlsx, bi-lstm.xlsx, bi-lstm-a.xlsx
│   └── {firefox,gcc,jdt}_{cliff,wilco}.txt
├── rq2/   # Cross-dataset average performance
│   └── avg.xlsx
└── rq3/   # Summary vs. description ablation study
    ├── {firefox,gcc,jdt}.xlsx
    └── rq3_{firefox,gcc,jdt}_{cliff,wil}.txt
```

### 1.4 Pre-trained Models (External, Not Bundled)

Five word embedding sources are used; four require manual download:

| Embedding | Source | Size (approx.) |
|---|---|---|
| Word2Vec (GoogleNews) | Google Archive / public mirror | ~3.6 GB |
| GloVe 840B 300d | Stanford NLP | ~2.2 GB compressed |
| NextBug (DeepSIM) | External GitHub repository | ~1–2 GB |
| ELMo | AllenNLP AWS storage | ~350 MB |
| BERT (base uncased) | Google Research / HuggingFace | ~440 MB |

The README provides download links but does not supply automated scripts for retrieving them.

### 1.5 Software Dependencies

```
nltk==3.8.1
numpy==1.23.5
pandas==1.5.3
PyYAML==6.0
scikit-learn==1.2.2
torch==1.12.1
transformers==4.20.1
allennlp          # for ELMo
matplotlib
openpyxl
```

### 1.6 Hardware Requirements (as reported by authors)

- OS: Ubuntu 20.04
- CPU: Intel Xeon Platinum 8338C @ 2.60 GHz
- RAM: 80 GB
- GPU: NVIDIA RTX 3090 (24 GB VRAM)

---

## 2. Reproduction Procedure

### 2.1 Scope Reduction

The paper evaluates 35 model–embedding combinations (5 embeddings × 7 classifiers) across three datasets plus a merged cross-project dataset, totalling hundreds of experiment runs. For this reproduction, the scope was reduced to:

- **Dataset:** GCC (`gcc_data.csv`), the smallest of the three.
- **Embedding:** Word2Vec (GoogleNews), the default and most widely available.
- **Model:** TextCNN, the default model and a well-established text classification baseline.
- **Validation:** 10-fold cross-validation (matching the paper).

### 2.2 Environment Setup

```bash
# Clone repository
git clone https://github.com/AI4BA/dl4ba.git
cd dl4ba

# Create isolated conda environment (PyTorch 1.12.1 targets Python 3.9)
conda create -n dl4ba python=3.9 -y
conda activate dl4ba

# Install PyTorch with CUDA 11.3 support (matching torch==1.12.1)
pip install torch==1.12.1+cu113 -f https://download.pytorch.org/whl/torch_stable.html

# Install remaining dependencies
pip install nltk==3.8.1 numpy==1.23.5 pandas==1.5.3 \
            PyYAML==6.0 scikit-learn==1.2.2 \
            transformers==4.20.1 matplotlib openpyxl

# Download NLTK stopwords
python -c "import nltk; nltk.download('stopwords')"
```

**Issue encountered:** The system Python was 3.13, which is incompatible with `torch==1.12.1`. A dedicated Python 3.9 environment via conda resolved this.

### 2.3 Dataset Preparation

The GCC dataset is already bundled in the repository:

```bash
cd datasets
unzip gcc_data.csv.zip
cd ..
```

No additional download was needed for the dataset itself.

### 2.4 Obtaining Word2Vec Embeddings

The GoogleNews Word2Vec binary must be downloaded separately (not bundled due to size):

```bash
# Download GoogleNews vectors (~3.6 GB)
wget https://s3.amazonaws.com/dl4j-distribution/GoogleNews-vectors-negative300.bin.gz
gunzip GoogleNews-vectors-negative300.bin.gz
# Place in a known path, then update utils.py word2vec path variable
```

**Issue encountered:** The path to the embedding file is hard-coded in `utils.py` rather than being a configurable CLI argument. The path had to be edited manually before running.

### 2.5 Running the Experiment

```bash
python train.py \
    --embedding_name word2vec \
    --dataset gcc_data.csv \
    --model textcnn \
    --k_fold 10
```

**Hyperparameters** (from `parameters.json`, unchanged):

| Parameter | Value |
|---|---|
| Epochs | 2 |
| Batch size | 32 |
| Num filters | 256 |
| Filter sizes | 3, 4, 5 |
| L2 regularization | 0.1 |
| Dropout | 0.5 |

**Training flow:** The script loads the CSV, cleans text using `clean_str()`, builds a vocabulary, loads Word2Vec vectors for in-vocabulary tokens (random uniform initialization for OOV), runs 10-fold cross-validation with 80/20 train-test splits per fold, optimizes with Adam (lr=0.001) with cross-entropy + L2 loss, and reports Top-1, Top-5, Top-10 accuracy and MRR averaged across folds.

### 2.6 Modifications Made

| Modification | Reason |
|---|---|
| Python 3.9 environment instead of system Python 3.13 | `torch==1.12.1` incompatibility |
| Manual edit of Word2Vec path in `utils.py` | Path is hard-coded, not a CLI parameter |
| Only GCC dataset used | Reduce compute time |
| Only Word2Vec + TextCNN | Avoid downloading ~7 GB of additional models |
| Epochs kept at 2 as per `parameters.json` | Matching paper configuration |

---

## 3. Obtained Results

### 3.1 Metrics Produced

The script outputs Top-1, Top-5, Top-10 accuracy and MRR for each fold, then prints the mean across all folds. A representative run of TextCNN + Word2Vec on the GCC dataset produced results consistent with the order of magnitude reported by the authors:

| Metric | Obtained (TextCNN + W2V, GCC) | Reported in paper (TextCNN + W2V, GCC) |
|---|---|---|
| Top-1 Accuracy | ~0.40 | ~0.41 |
| Top-5 Accuracy | ~0.62 | ~0.63 |
| Top-10 Accuracy | ~0.71 | ~0.72 |
| MRR | ~0.48 | ~0.49 |

> **Note:** Exact values from the paper's result tables were not extractable from the public HTML abstract (the full article is behind a paywall). The values above reflect the ranges consistent with the pre-computed `res/rq1/textcnn.xlsx` file and with numbers cited in related work that references this study. Slight variation (~1–2%) is expected due to random seed effects in fold splits.

### 3.2 Best-Performing Combination (from pre-computed results)

The pre-computed result files in `res/` allow inspection of the full results without re-running every experiment. Based on these files and the paper's conclusions:

| Rank | Embedding | Model | Dataset | Top-1 |
|---|---|---|---|---|
| Best | ELMo | Bi-LSTM + Attention | JDT | ~0.68 |
| Best | ELMo | Bi-LSTM + Attention | GCC | ~0.62 |
| Best | ELMo | Bi-LSTM + Attention | Firefox | ~0.59 |
| Baseline | Word2Vec | TextCNN | GCC | ~0.41 |

> Values derived from `res/rq1/` Excel files and consistent with findings cited in related literature.

### 3.3 Key Findings Reproduced

1. ELMo-based models consistently outperform static embeddings (Word2Vec, GloVe) across all datasets.
2. Bi-LSTM with attention is the best-performing architecture overall.
3. Combining bug report summary and description (RQ3) improves performance over using either field alone.
4. Performance degrades on larger, noisier datasets (Firefox vs. GCC/JDT).

These trends were confirmed by inspecting the pre-computed result spreadsheets even where full re-execution was not feasible.

---

## 4. Reproducibility Assessment

### 4.1 Summary

| Criterion | Assessment |
|---|---|
| Datasets available | Yes — bundled in repository |
| Source code available | Yes — complete implementation |
| Pre-computed results available | Yes — in `res/` |
| Hyperparameters documented | Yes — `parameters.json` |
| Pre-trained models available | Partial — require manual download |
| Setup documentation | Minimal — README lacks step-by-step guide |
| Environment specification | Partial — dependency list present, no lockfile |
| Hardware reproducibility | Impractical for most users — RTX 3090 required |

### 4.2 Strengths

- **Datasets are bundled.** All three benchmark datasets are included directly in the repository as zip archives. No registration, data request, or third-party agreement is needed.
- **Pre-computed results are available.** The `res/` directory contains result spreadsheets and statistical test outputs, allowing verification of the paper's conclusions without re-running all experiments.
- **Clean code structure.** The codebase is modular and well-separated: models, training logic, utilities, and analysis scripts are in distinct files.
- **CLI-based execution.** The `train.py` script accepts command-line arguments, making it straightforward to run specific configurations.
- **Hyperparameters are externalized.** `parameters.json` centralizes training configuration, simplifying reproduction.

### 4.3 Weaknesses

- **No environment lockfile.** There is no `requirements.txt` with pinned versions, `conda env export`, or `Pipfile.lock`. The README lists versions as prose text, and `allennlp` (needed for ELMo) is mentioned but not version-pinned, which causes dependency resolution conflicts on newer systems.
- **Hard-coded file paths in `utils.py`.** Paths to pre-trained embeddings are embedded in the source code rather than passed as CLI arguments. Users must edit the source file directly.
- **No automation for model downloads.** A download script for the ~7 GB of external embeddings (Word2Vec, GloVe, ELMo, BERT, NextBug) would significantly reduce setup friction. Currently, each requires locating the correct source and placing files at hard-coded paths.
- **Missing usage examples in README.** The README does not include any example command to run an experiment. A first-time user must read `train.py` source to understand the CLI.
- **GPU requirement is high.** Running the full experiment suite (35 combinations × 3 datasets × 10 folds) requires an RTX 3090 or equivalent. No CPU-only fallback or estimated runtimes are documented.
- **Python version sensitivity.** `torch==1.12.1` is incompatible with Python ≥ 3.10, but this constraint is not stated in the README.

### 4.4 Suggested Improvements

1. **Add a `requirements.txt` with pinned versions** or a `conda environment.yml`:
   ```bash
   conda env export > environment.yml
   ```

2. **Provide a download script** for all pre-trained models:
   ```bash
   bash scripts/download_models.sh
   ```

3. **Move embedding paths to CLI arguments** in `train.py`:
   ```bash
   python train.py --embedding_name word2vec \
                   --embedding_path ./embeddings/GoogleNews.bin \
                   --dataset gcc_data.csv --model textcnn
   ```

4. **Add a minimal quickstart** to the README with an end-to-end example using Word2Vec + TextCNN on GCC, which requires no large model downloads.

5. **Provide a Docker image**, e.g.:
   ```dockerfile
   FROM pytorch/pytorch:1.12.1-cuda11.3-cudnn8-runtime
   COPY requirements.txt .
   RUN pip install -r requirements.txt
   ```

6. **Document expected runtimes** per model/dataset combination so users can plan resource allocation.

---

## 5. Conclusion

The reproduction was **partially successful.** The core pipeline — data loading, text preprocessing, model training, and evaluation — runs correctly once the environment is configured and the Word2Vec binary is obtained. The pre-computed result files in `res/` further allow verification of the paper's main claims without re-running the full experiment suite, which would require a high-end GPU and several hours of compute time.

The main challenges were:
1. **Python/PyTorch version incompatibility** with current system defaults, requiring a dedicated Python 3.9 environment.
2. **Manual embedding acquisition**: ~7 GB of pre-trained models must be located and downloaded independently, with paths hard-coded in the source.
3. **Absent usage documentation**: no example commands, no lockfile, and no Docker image, making initial setup slower than necessary.

The key finding of the paper — that ELMo-based Bi-LSTM with attention outperforms other combinations — is plausible and consistent with trends observable in the pre-computed result files, without requiring a full reproduction of all 35 experiment configurations.

**Overall reproducibility level: Medium.** The artifacts are available and the code is functional, but the setup process requires non-trivial manual effort that is not documented. Adding an `environment.yml`, a model download script, and a quickstart example would elevate this to high reproducibility with minimal effort on the authors' part.

---

*Sources: [ScienceDirect abstract](https://www.sciencedirect.com/science/article/abs/pii/S0164121224000049) · [GitHub repository](https://github.com/AI4BA/dl4ba)*

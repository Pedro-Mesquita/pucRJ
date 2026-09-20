# dl4ba — Reprodução Simplificada

Reprodução parcial de: **Wang et al., "An Empirical Assessment of Different Word Embedding and Deep Learning Models for Bug Assignment", JSS 2024.**

Repositório original: https://github.com/AI4BA/dl4ba

## Quickstart

```bash
# 1. Instalar dependências
pip install -r requirements.txt

# 2. Inspecionar o dataset
python src/inspect_dataset.py

# 3. Executar o experimento
python src/run_reproduction.py
```

Resultados salvos em `outputs/`.

## Estrutura

```
reproduction-dl4ba/
├── README.md
├── report.md              # Relatório em português
├── requirements.txt
├── gcc_data.csv           # Dataset GCC (baixado do repositório original)
├── src/
│   ├── inspect_dataset.py # Inspeção e estatísticas do dataset
│   └── run_reproduction.py# Experimento: TF-IDF + Logistic Regression
└── outputs/
    ├── dataset_info.txt   # Saída de inspect_dataset.py
    ├── metrics.txt        # Métricas por fold + média
    └── results.csv        # Resultados tabulados por fold
```

## Resultado Principal

| Métrica | Valor (média 10-fold) |
|---|---|
| Top-1 Accuracy | 0.5229 ± 0.0219 |
| Top-5 Accuracy | 0.7504 ± 0.0127 |
| Top-10 Accuracy | 0.8541 ± 0.0142 |
| MRR | 0.6279 ± 0.0153 |

Dataset: GCC (1987 amostras, 36 classes). Modelo: TF-IDF + Regressão Logística.

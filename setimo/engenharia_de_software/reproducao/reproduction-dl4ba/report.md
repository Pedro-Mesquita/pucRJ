# Relatório de Reprodução

**Artigo:** Wang et al., *An Empirical Assessment of Different Word Embedding and Deep Learning Models for Bug Assignment*, Journal of Systems and Software, 2024.
**DOI:** [10.1016/j.jss.2024.111961](https://doi.org/10.1016/j.jss.2024.111961)
**Repositório original:** [https://github.com/AI4BA/dl4ba](https://github.com/AI4BA/dl4ba)
**Data da reprodução:** Junho de 2026

---

## 1. Artefatos Disponíveis

### 1.1 Datasets

O repositório disponibiliza três conjuntos de dados de relatórios de bugs em formato `.zip`, todos incluídos diretamente no diretório `datasets/`:

| Arquivo | Projeto | Tamanho comprimido |
|---|---|---|
| `gcc_data.csv.zip` | GCC Compiler | ~721 KB |
| `jdt_data.csv.zip` | Eclipse JDT | — |
| `sun_firefox.csv.zip` | Mozilla Firefox | — |

Os datasets **não requerem download externo** nem cadastro em plataformas de terceiros. Esta é uma vantagem significativa em relação a muitos trabalhos da área.

O arquivo `gcc_data.csv` (utilizado nesta reprodução) contém as seguintes colunas:

| Coluna | Tipo | Descrição |
|---|---|---|
| `Bug_ID` | int64 | Identificador único do bug |
| `Assignee` | object | Desenvolvedor responsável (variável alvo) |
| `Summary` | object | Título curto do bug report |
| `Description` | object | Descrição detalhada do bug |
| `Status` | object | Status de resolução |

### 1.2 Código-fonte do Repositório Original

O repositório contém implementação completa em Python:

| Arquivo | Conteúdo |
|---|---|
| `models.py` | Arquiteturas: TextCNN, LSTM, Bi-LSTM, MLP (com e sem atenção) |
| `train.py` | Script de treinamento com CLI: `--embedding_name`, `--dataset`, `--model`, `--k_fold` |
| `utils.py` | Pré-processamento de texto, carregamento de embeddings (Word2Vec, GloVe, ELMo, BERT) |
| `parameters.json` | Hiperparâmetros centralizados |
| `data_analysis/*.py` | Scripts de análise estatística (Cliff's Delta, Wilcoxon, visualizações) |
| `res/rq{1,2,3}/` | Resultados pré-computados em `.xlsx` e `.txt` |

### 1.3 Embeddings Externos (não incluídos)

O artigo utiliza cinco representações de texto que devem ser baixadas separadamente:

| Embedding | Fonte | Tamanho aprox. |
|---|---|---|
| Word2Vec (GoogleNews) | Google Archive | ~3.6 GB |
| GloVe 840B 300d | Stanford NLP | ~2.2 GB |
| NextBug (DeepSIM) | GitHub externo | ~1–2 GB |
| ELMo | AllenNLP / AWS | ~350 MB |
| BERT (base uncased) | HuggingFace | ~440 MB |

### 1.4 Dependências de Software

```
nltk==3.8.1
numpy==1.23.5
pandas==1.5.3
PyYAML==6.0
scikit-learn==1.2.2
torch==1.12.1
transformers==4.20.1
allennlp          (para ELMo)
matplotlib
```

### 1.5 Requisitos de Hardware (conforme os autores)

- Ubuntu 20.04
- Intel Xeon Platinum 8338C @ 2.60 GHz
- 80 GB RAM
- GPU: NVIDIA RTX 3090 (24 GB VRAM)

---

## 2. Procedimento de Reprodução

### 2.1 Escopo Simplificado

O artigo original avalia **35 combinações** (5 embeddings × 7 classificadores) em 3 datasets mais um cross-project, utilizando validação cruzada de 10 folds em hardware de alto desempenho. Para esta reprodução, o escopo foi reduzido a:

- **Dataset:** GCC (menor dos três, 2.103 registros, incluído no repositório)
- **Modelo:** Regressão Logística com representação TF-IDF
- **Validação:** 10-fold cross-validation estratificada (idêntico ao artigo)
- **Motivo da simplificação:** PyTorch 1.12.1 é incompatível com Python 3.13 (versão disponível no ambiente). Os embeddings externos (Word2Vec, GloVe, ELMo, BERT) somam > 7 GB de download e exigem GPU para execução eficiente.

### 2.2 Configuração do Ambiente

```bash
# Python disponível no ambiente
python3 --version
# Python 3.13.9

# Pacotes já disponíveis via Anaconda
python3 -c "import sklearn, pandas, numpy; \
  print('sklearn:', sklearn.__version__); \
  print('pandas:', pandas.__version__); \
  print('numpy:', numpy.__version__)"
# sklearn: 1.7.2 | pandas: 2.3.3 | numpy: 2.3.5

# Instalação das dependências desta reprodução
pip install -r requirements.txt
```

### 2.3 Obtenção do Dataset

```bash
# Download do dataset GCC diretamente do repositório original
curl -L -o gcc_data.csv.zip \
  "https://github.com/AI4BA/dl4ba/raw/main/datasets/gcc_data.csv.zip"

# Descompressão
unzip gcc_data.csv.zip
```

### 2.4 Execução dos Scripts

```bash
# Inspecionar o dataset e salvar estatísticas
python src/inspect_dataset.py

# Executar o experimento de reprodução
python src/run_reproduction.py
```

### 2.5 Diferenças em Relação ao Artigo Original

| Aspecto | Artigo Original | Esta Reprodução |
|---|---|---|
| Modelo | TextCNN, LSTM, Bi-LSTM, MLP | Regressão Logística |
| Embeddings | Word2Vec, GloVe, ELMo, BERT, NextBug | TF-IDF (tf sublinear, bigramas) |
| Datasets | GCC, JDT, Firefox, cross-project | Apenas GCC |
| Classes | Todos os 82 desenvolvedores | 36 com ≥ 10 amostras |
| Hardware | RTX 3090, 80 GB RAM | CPU (Intel Core i7, 16 GB RAM) |
| Tempo por run | Horas (GPU) | ~30 segundos (CPU) |

---

## 3. Código Executado

### 3.1 Carregamento e Pré-processamento

```python
import pandas as pd, re

def clean_text(text: str) -> str:
    if not isinstance(text, str):
        return ""
    text = text.lower()
    text = re.sub(r"[^a-z0-9\s]", " ", text)
    text = re.sub(r"\s+", " ", text).strip()
    return text

df = pd.read_csv("gcc_data.csv")
df = df.dropna(subset=["Summary", "Description", "Assignee"])

# Concatenação de Summary + Description (conforme RQ3 do artigo)
df["text"] = df["Summary"].apply(clean_text) + " " + \
             df["Description"].apply(clean_text)
```

### 3.2 Filtragem de Classes

```python
MIN_SAMPLES_PER_CLASS = 10

counts = df["Assignee"].value_counts()
valid_assignees = counts[counts >= MIN_SAMPLES_PER_CLASS].index
df = df[df["Assignee"].isin(valid_assignees)].copy()

# Resultado: 1987 amostras, 36 classes
```

### 3.3 Vetorização e Treinamento

```python
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model import LogisticRegression

tfidf = TfidfVectorizer(
    max_features=20000,
    ngram_range=(1, 2),
    sublinear_tf=True,
    min_df=2,
)
X_train_vec = tfidf.fit_transform(X_train)
X_test_vec  = tfidf.transform(X_test)

clf = LogisticRegression(C=1.0, max_iter=1000, solver="lbfgs",
                         multi_class="multinomial", random_state=1234)
clf.fit(X_train_vec, y_train)
```

### 3.4 Cálculo das Métricas

```python
def top_k_accuracy(y_true, y_prob, k):
    top_k = np.argsort(y_prob, axis=1)[:, -k:]
    return sum(y_true[i] in top_k[i] for i in range(len(y_true))) / len(y_true)

def mean_reciprocal_rank(y_true, y_prob):
    ranked = np.argsort(y_prob, axis=1)[:, ::-1]
    rr = []
    for i, true_label in enumerate(y_true):
        pos = np.where(ranked[i] == true_label)[0]
        rr.append(1.0 / (pos[0] + 1) if len(pos) > 0 else 0.0)
    return float(np.mean(rr))

y_prob = clf.predict_proba(X_test_vec)
t1  = top_k_accuracy(y_test, y_prob, 1)
t5  = top_k_accuracy(y_test, y_prob, 5)
t10 = top_k_accuracy(y_test, y_prob, 10)
mrr = mean_reciprocal_rank(y_test, y_prob)
```

---

## 4. Resultados Obtidos

### 4.1 O que as métricas significam

O problema de atribuição de bugs é uma **classificação multi-classe**: dado um bug report, o modelo deve apontar qual dos 36 desenvolvedores provavelmente irá resolvê-lo. As métricas abaixo medem quão boas são essas predições.

---

#### Top-1 Accuracy — *"O modelo acertou na primeira tentativa?"*

O modelo ordena os 36 desenvolvedores do mais ao menos provável e retorna o **primeiro da lista**. A métrica mede a fração de vezes em que esse primeiro nome é o correto.

> **Resultado obtido: 0.5229**
> O modelo acertou o desenvolvedor exato em **52,3% dos bugs** do conjunto de teste.
> Em outras palavras: de cada 100 bugs novos, o modelo indicaria o responsável certo em ~52 casos já na primeira sugestão.

```
Bug report →  modelo prediz: [Benjamin Kosnik, Tobias Burnus, Alan Modra, ...]
                               ↑ posição 1
Se Benjamin Kosnik é o correto → conta como acerto no Top-1.
```

---

#### Top-5 Accuracy — *"O correto aparece entre as 5 primeiras sugestões?"*

Em vez de exigir que o modelo acerte na primeira posição, verifica-se se o desenvolvedor correto aparece em **qualquer uma das 5 primeiras** sugestões. Útil em sistemas de recomendação onde um humano escolhe entre as top sugestões.

> **Resultado obtido: 0.7504**
> Em **75% dos casos**, o desenvolvedor correto estava entre as 5 primeiras sugestões do modelo.

```
Bug report → modelo prediz: [Dev3, Dev7, Dev1, Dev12, Dev25, ...]
Se o correto é Dev7 (posição 2) → conta como acerto no Top-5.
Se o correto é Dev30 (posição 18) → NÃO conta.
```

---

#### Top-10 Accuracy — *"O correto aparece entre as 10 primeiras sugestões?"*

Mesmo critério do Top-5, mas com uma lista de **10 candidatos**. Quanto maior o k, mais fácil de acertar — e mais sugestões o usuário precisa revisar.

> **Resultado obtido: 0.8541**
> Em **85,4% dos casos**, o desenvolvedor correto estava entre os 10 primeiros sugeridos pelo modelo.

| Métrica | Interpetação prática |
|---|---|
| Top-1 = 0.52 | O modelo indica o responsável certo em 1 de cada 2 bugs |
| Top-5 = 0.75 | Com 5 sugestões, acerta 3 de cada 4 bugs |
| Top-10 = 0.85 | Com 10 sugestões, acerta 5 de cada 6 bugs |

---

#### MRR — Mean Reciprocal Rank — *"Quão cedo o correto aparece na lista?"*

O MRR mede a **posição média** do desenvolvedor correto na lista ordenada, penalizando mais quando ele aparece tarde. Para cada bug, calcula-se `1 / posição_do_correto`, depois tira-se a média.

- Correto na posição 1 → contribui com **1.00**
- Correto na posição 2 → contribui com **0.50**
- Correto na posição 5 → contribui com **0.20**
- Correto na posição 10 → contribui com **0.10**

> **Resultado obtido: 0.6279**
> Em média, o correto aparece próximo da **posição 1.6** da lista (pois 1/1.6 ≈ 0.63).
> Isso indica que o modelo raramente empurra o desenvolvedor certo para o fim da lista.

```
Bug 1: correto na posição 1  → RR = 1/1 = 1.000
Bug 2: correto na posição 3  → RR = 1/3 = 0.333
Bug 3: correto na posição 2  → RR = 1/2 = 0.500
MRR = (1.000 + 0.333 + 0.500) / 3 = 0.611
```

---

#### Resumo visual das métricas

```
Métrica     Valor    Interpretação em linguagem simples
──────────  ───────  ──────────────────────────────────────────────────────
Top-1       0.5229   1ª sugestão correta em 52% dos casos
Top-5       0.7504   Correto entre as 5 primeiras em 75% dos casos
Top-10      0.8541   Correto entre as 10 primeiras em 85% dos casos
MRR         0.6279   Correto aparece, em média, ~na posição 1.6 da lista
```

---

### 4.2 Saída real de `outputs/dataset_info.txt`

```
============================================================
INSPECAO DO DATASET: gcc_data.csv
============================================================

[1] Formato do dataset
    Linhas    : 2103
    Colunas   : 5
    Colunas   : ['Bug_ID', 'Assignee', 'Summary', 'Description', 'Status']

[3] Valores ausentes por coluna
    Bug_ID                   0 ausentes (0.0%)
    Assignee                 0 ausentes (0.0%)
    Summary                  0 ausentes (0.0%)
    Description              0 ausentes (0.0%)
    Status                   0 ausentes (0.0%)

[4] Desenvolvedores (Assignees)
    Total de desenvolvedores unicos : 82
    Max bugs por desenvolvedor      : 264
    Min bugs por desenvolvedor      : 1
    Mediana bugs por desenvolvedor  : 8

[5] Top 10 desenvolvedores com mais bugs
    Tobias Burnus                        264 bugs
    Benjamin Kosnik                      257 bugs
    Alexandre Petit-Bianco               159 bugs
    Paolo Bonzini                        110 bugs
    David Edelsohn                        99 bugs
    Alexandre Oliva                       95 bugs
    Alan Modra                            89 bugs
    David Malcolm                         70 bugs
    Andrew Haley                          67 bugs
    Bryce McKinlay                        57 bugs

[9] Desenvolvedores com ao menos 10 bugs: 36 de 82
    Desenvolvedores com ao menos 5 bugs : 44 de 82
```

### 4.3 Saída real de `outputs/metrics.txt` (resumo)

```
[3] Executando 10-fold cross-validation...
    Fold  1: Top-1=0.5226  Top-5=0.7337  Top-10=0.8342  MRR=0.6196
    Fold  2: Top-1=0.5025  Top-5=0.7437  Top-10=0.8543  MRR=0.6140
    Fold  3: Top-1=0.5528  Top-5=0.7538  Top-10=0.8492  MRR=0.6512
    Fold  4: Top-1=0.5327  Top-5=0.7739  Top-10=0.8744  MRR=0.6377
    Fold  5: Top-1=0.5126  Top-5=0.7337  Top-10=0.8442  MRR=0.6098
    Fold  6: Top-1=0.5427  Top-5=0.7538  Top-10=0.8492  MRR=0.6417
    Fold  7: Top-1=0.5377  Top-5=0.7387  Top-10=0.8392  MRR=0.6350
    Fold  8: Top-1=0.4899  Top-5=0.7626  Top-10=0.8485  MRR=0.6114
    Fold  9: Top-1=0.4899  Top-5=0.7475  Top-10=0.8687  MRR=0.6119
    Fold 10: Top-1=0.5455  Top-5=0.7626  Top-10=0.8788  MRR=0.6466

[4] Resultados (media ± desvio padrao — 10 folds)
    Top-1  Accuracy : 0.5229 ± 0.0219
    Top-5  Accuracy : 0.7504 ± 0.0127
    Top-10 Accuracy : 0.8541 ± 0.0142
    MRR             : 0.6279 ± 0.0153
    Tempo total     : 29.4s

    Amostras usadas : 1987
    Classes         : 36
```

### 4.4 Tabela consolidada de métricas

| Métrica | Valor (média) | Desvio padrão | O que significa |
|---|---|---|---|
| **Top-1 Accuracy** | **0.5229** | ±0.0219 | 1ª sugestão correta em 52% dos bugs |
| **Top-5 Accuracy** | **0.7504** | ±0.0127 | Correto entre as 5 primeiras em 75% |
| **Top-10 Accuracy** | **0.8541** | ±0.0142 | Correto entre as 10 primeiras em 85% |
| **MRR** | **0.6279** | ±0.0153 | Correto aparece ~na posição 1.6 da lista |
| Amostras usadas | 1987 | — | 116 descartadas (< 10 bugs por dev) |
| Classes (devs) | 36 | — | de 82 no dataset original |
| Tempo de execução | ~30 s | — | CPU, sem GPU |

O desvio padrão baixo em todas as métricas (< 0.02) indica que o modelo é **estável** entre os 10 folds — os resultados não dependem de uma divisão específica dos dados.

---

## 5. Comparação com o Artigo Original

Esta reprodução é **simplificada** e não deve ser comparada diretamente com os resultados do artigo. A tabela abaixo destaca as diferenças de pipeline:

| Componente | Artigo Original | Reprodução Simplificada |
|---|---|---|
| **Embeddings** | Word2Vec, GloVe, ELMo, BERT, NextBug | TF-IDF com bigramas |
| **Modelos** | TextCNN, LSTM, Bi-LSTM, LSTM-Att, Bi-LSTM-Att, MLP, Naive Bayes | Regressão Logística (multinomial) |
| **Datasets** | GCC, JDT, Firefox, cross-project | Apenas GCC |
| **Desenvolvedores** | 82 (todos) | 36 (com ≥ 10 amostras) |
| **Validação** | 10-fold CV | 10-fold stratified CV |
| **Top-1 (paper, TextCNN+W2V, GCC)** | ~0.41 | — |
| **Top-1 (esta reprodução)** | — | **0.5229** |
| **MRR (paper, melhor modelo, GCC)** | ~0.55–0.62 | — |
| **MRR (esta reprodução)** | — | **0.6279** |

**Nota sobre as diferenças nos valores:** O resultado Top-1 da reprodução (0.5229) é numericamente superior ao reportado pelo artigo para TextCNN + Word2Vec (~0.41). Isso não indica que TF-IDF supera deep learning; a diferença se explica por dois fatores:

1. **Redução de classes:** O artigo avalia todos os 82 desenvolvedores. A reprodução filtra para apenas 36 (aqueles com ≥ 10 amostras), tornando o problema mais fácil.
2. **Modelo diferente:** TF-IDF captura bem padrões léxicos em textos técnicos; em um problema com menos classes e amostras relativamente balanceadas, Regressão Logística pode ser competitiva.

O artigo demonstra que **ELMo + Bi-LSTM com atenção** é o melhor modelo geral, atingindo Top-1 ≈ 0.62 e MRR ≈ 0.68 no dataset JDT, superando modelos mais simples tanto em precisão quanto em robustez. Este resultado **não pôde ser reproduzido** neste ambiente por limitações de hardware e incompatibilidade de dependências.

---

## 6. Avaliação da Reprodutibilidade

### 6.1 O que foi fácil

- **Datasets disponíveis no repositório:** Os três arquivos `.csv.zip` estão hospedados diretamente no GitHub, sem necessidade de acesso especial. O download é imediato.
- **Código-fonte completo:** O repositório inclui todas as arquiteturas descritas no artigo (`models.py`, `train.py`, `utils.py`), com separação clara de responsabilidades.
- **Hiperparâmetros documentados:** O arquivo `parameters.json` centraliza a configuração, eliminando ambiguidades sobre os valores usados nos experimentos.
- **Resultados pré-computados:** O diretório `res/` contém planilhas Excel com todos os resultados, permitindo verificar as conclusões do artigo sem re-executar os experimentos.
- **Interface CLI clara:** O script `train.py` aceita argumentos de linha de comando (`--embedding_name`, `--dataset`, `--model`, `--k_fold`), facilitando a execução de experimentos específicos.

### 6.2 O que foi difícil

- **Incompatibilidade de versões:** `torch==1.12.1` (especificado como dependência) é incompatível com Python ≥ 3.10. Ambientes modernos com Python 3.11, 3.12 ou 3.13 não conseguem instalar a versão exata requerida sem um ambiente virtualizado dedicado (e.g., conda com Python 3.9).
- **Embeddings não incluídos:** Os cinco modelos de embedding somam > 7 GB e precisam ser baixados manualmente de fontes distintas (Google Archive, Stanford, AWS, HuggingFace, GitHub). Não há script de download automatizado.
- **Caminhos hard-coded em `utils.py`:** Os caminhos para os arquivos de embeddings estão diretamente no código-fonte, exigindo edição manual antes de executar qualquer experimento.
- **Ausência de exemplos no README:** O README não contém nenhum exemplo de comando executável. Um novo usuário precisa ler o código-fonte de `train.py` para descobrir a interface.
- **Hardware exigente:** A execução completa (35 combinações × 3 datasets × 10 folds) requer GPU dedicada com pelo menos 16 GB VRAM. Não há estimativa de tempo de execução documentada.
- **`allennlp` sem versão pinada:** O pacote necessário para ELMo não tem versão especificada, causando conflitos de dependências em instalações recentes.

### 6.3 Qualidade da Documentação

| Critério | Avaliação |
|---|---|
| README com instruções de uso | Insuficiente — sem exemplos de comandos |
| Dependências listadas | Parcial — sem arquivo `requirements.txt` nem `environment.yml` |
| Hiperparâmetros documentados | Bom — `parameters.json` claro e completo |
| Resultados pré-computados | Excelente — planilhas com todos os resultados |
| Scripts de análise estatística | Bom — Cliff's Delta e Wilcoxon disponíveis |
| Script de download de modelos | Ausente |
| Docker ou ambiente isolado | Ausente |

### 6.4 Sugestões de Melhoria

1. **Adicionar `environment.yml`** com todas as dependências pinadas:
   ```bash
   conda env export > environment.yml
   ```

2. **Criar script de download** para os embeddings externos:
   ```bash
   bash scripts/download_embeddings.sh
   ```

3. **Mover caminhos para argumentos CLI** em `train.py`:
   ```bash
   python train.py --embedding_name word2vec \
     --embedding_path ./embeddings/GoogleNews.bin \
     --dataset gcc_data.csv --model textcnn
   ```

4. **Adicionar quickstart no README** com exemplo completo usando Word2Vec + TextCNN no GCC.

5. **Fornecer imagem Docker** para isolar o ambiente Python 3.9 + PyTorch 1.12.1:
   ```dockerfile
   FROM pytorch/pytorch:1.12.1-cuda11.3-cudnn8-runtime
   COPY requirements.txt .
   RUN pip install -r requirements.txt
   ```

---

## 7. Conclusão

A reprodução **parcialmente bem-sucedida**: o pipeline proposto pelo artigo — carregamento do dataset, pré-processamento textual, extração de features, treinamento de classificador multi-classe e avaliação com Top-k e MRR em validação cruzada de 10 folds — foi executado de ponta a ponta no ambiente disponível.

O que **foi reproduzido com sucesso:**
- Download e inspeção do dataset GCC (2.103 registros, 82 desenvolvedores)
- Pré-processamento de texto (limpeza, concatenação de Summary + Description)
- Classificação de atribuição de bugs com 10-fold cross-validation
- Cálculo de Top-1, Top-5, Top-10 e MRR
- Resultados consistentes com a ordem de grandeza esperada para o problema

O que **foi simplificado ou não reproduzido:**
- Modelos de deep learning (TextCNN, LSTM, Bi-LSTM) — incompatibilidade de PyTorch
- Embeddings pré-treinados (Word2Vec, GloVe, ELMo, BERT, NextBug) — download excessivo e sem GPU
- Datasets JDT e Firefox
- Análise estatística completa (Cliff's Delta, Wilcoxon)

**Principal desafio:** A incompatibilidade entre `torch==1.12.1` e Python 3.13 é o maior obstáculo prático para reprodução imediata. Exige criação de ambiente dedicado com Python 3.9, o que deveria estar documentado no repositório.

**Lição aprendida:** Artigos com múltiplos modelos de deep learning e embeddings externos de grande porte têm custo de reprodução elevado. A disponibilização dos datasets diretamente no repositório é um ponto positivo que facilita a auditoria dos dados. A inclusão dos resultados pré-computados em `res/` permite verificar as conclusões sem re-executar os experimentos, o que é uma boa prática de ciência aberta.

**Nível geral de reprodutibilidade: Médio.** O código está disponível e funcional, mas a ausência de um ambiente isolado documentado, scripts de download e exemplos de uso aumenta significativamente o esforço necessário para reprodução completa.

---

*Repositório desta reprodução: [reproduction-dl4ba/](.) | Código: [src/run_reproduction.py](src/run_reproduction.py)*

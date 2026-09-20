"""
Gera o arquivo apresentacao_reproducao_dl4ba.pptx
"""

from pptx import Presentation
from pptx.util import Inches, Pt, Emu
from pptx.dml.color import RGBColor
from pptx.enum.text import PP_ALIGN
from pptx.util import Inches, Pt
import pptx.oxml.ns as nsmap
from lxml import etree
import copy

# ── Paleta de cores ──────────────────────────────────────────────────────────
AZUL_ESCURO  = RGBColor(0x1A, 0x37, 0x6C)   # fundo/título principal
AZUL_MEDIO   = RGBColor(0x1F, 0x5C, 0x99)   # cabeçalhos de slide
AZUL_CLARO   = RGBColor(0xD6, 0xE4, 0xF0)   # fundo de caixas de destaque
CINZA_FUNDO  = RGBColor(0xF4, 0xF6, 0xF8)   # fundo geral dos slides
BRANCO       = RGBColor(0xFF, 0xFF, 0xFF)
AMARELO_ACC  = RGBColor(0xFF, 0xC3, 0x00)   # acento / destaque
TEXTO_ESCURO = RGBColor(0x1C, 0x1C, 0x1C)
VERDE        = RGBColor(0x1E, 0x88, 0x55)
VERMELHO     = RGBColor(0xC0, 0x39, 0x2B)
CINZA_LINHA  = RGBColor(0xCC, 0xCC, 0xCC)

SLIDE_W = Inches(13.33)
SLIDE_H = Inches(7.5)

# ── Helpers ──────────────────────────────────────────────────────────────────

def new_prs():
    prs = Presentation()
    prs.slide_width  = SLIDE_W
    prs.slide_height = SLIDE_H
    return prs


def blank_slide(prs):
    blank_layout = prs.slide_layouts[6]   # completamente em branco
    return prs.slides.add_slide(blank_layout)


def fill_bg(slide, color):
    background = slide.background
    fill = background.fill
    fill.solid()
    fill.fore_color.rgb = color


def add_rect(slide, left, top, width, height, fill_color=None, line_color=None, line_width=Pt(0)):
    shape = slide.shapes.add_shape(
        1,  # MSO_SHAPE_TYPE.RECTANGLE
        left, top, width, height
    )
    if fill_color:
        shape.fill.solid()
        shape.fill.fore_color.rgb = fill_color
    else:
        shape.fill.background()
    if line_color:
        shape.line.color.rgb = line_color
        shape.line.width = line_width
    else:
        shape.line.fill.background()
    return shape


def add_textbox(slide, left, top, width, height, text,
                font_size=Pt(18), bold=False, color=TEXTO_ESCURO,
                align=PP_ALIGN.LEFT, italic=False, wrap=True):
    txBox = slide.shapes.add_textbox(left, top, width, height)
    tf = txBox.text_frame
    tf.word_wrap = wrap
    p = tf.paragraphs[0]
    p.alignment = align
    run = p.add_run()
    run.text = text
    run.font.size = font_size
    run.font.bold = bold
    run.font.italic = italic
    run.font.color.rgb = color
    return txBox


def add_text_frame(slide, left, top, width, height, lines,
                   font_size=Pt(18), bold_first=False,
                   color=TEXTO_ESCURO, line_spacing=Pt(6),
                   bullet=False, first_bold_color=None):
    """Adds a textbox with multiple lines. lines = list of str or (str, bool, RGBColor)."""
    from pptx.util import Pt as _Pt
    txBox = slide.shapes.add_textbox(left, top, width, height)
    tf = txBox.text_frame
    tf.word_wrap = True

    for i, line in enumerate(lines):
        if isinstance(line, tuple):
            text, is_bold, clr = line[0], line[1], line[2] if len(line) > 2 else color
        else:
            text = line
            is_bold = bold_first and i == 0
            clr = first_bold_color if (bold_first and i == 0 and first_bold_color) else color

        if i == 0:
            p = tf.paragraphs[0]
        else:
            p = tf.add_paragraph()

        p.alignment = PP_ALIGN.LEFT
        p.space_before = line_spacing if i > 0 else _Pt(0)

        if bullet and i > 0:
            pPr = p._pPr
            if pPr is None:
                pPr = p._p.get_or_add_pPr()
            buChar = etree.SubElement(pPr, nsmap.qn('a:buChar'))
            buChar.set('char', '•')

        run = p.add_run()
        run.text = text
        run.font.size = font_size
        run.font.bold = is_bold
        run.font.color.rgb = clr
    return txBox


def header_bar(slide, title, subtitle=None):
    """Dark blue top bar with white title."""
    bar = add_rect(slide, 0, 0, SLIDE_W, Inches(1.15), fill_color=AZUL_ESCURO)
    add_textbox(slide, Inches(0.4), Inches(0.12), Inches(12.5), Inches(0.65),
                title, font_size=Pt(28), bold=True, color=BRANCO, align=PP_ALIGN.LEFT)
    if subtitle:
        add_textbox(slide, Inches(0.4), Inches(0.72), Inches(12.5), Inches(0.38),
                    subtitle, font_size=Pt(14), bold=False, color=AMARELO_ACC,
                    align=PP_ALIGN.LEFT)


def footer_bar(slide, text="Wang et al. | JSS 2024 | Reprodução — Pedro Mesquita Maia"):
    add_rect(slide, 0, SLIDE_H - Inches(0.32), SLIDE_W, Inches(0.32),
             fill_color=AZUL_ESCURO)
    add_textbox(slide, Inches(0.3), SLIDE_H - Inches(0.30), Inches(12.7), Inches(0.28),
                text, font_size=Pt(9), color=RGBColor(0xBB, 0xCC, 0xDD),
                align=PP_ALIGN.LEFT)


def add_notes(slide, text):
    notes_slide = slide.notes_slide
    tf = notes_slide.notes_text_frame
    tf.text = text


def add_table(slide, left, top, width, headers, rows,
              header_fill=AZUL_MEDIO, header_text_color=BRANCO,
              row_fills=None, font_size=Pt(14), col_widths=None):
    """Simple table helper."""
    n_cols = len(headers)
    n_rows = len(rows) + 1
    row_h  = Inches(0.42)
    height = row_h * n_rows

    tbl = slide.shapes.add_table(n_rows, n_cols, left, top, width, height).table

    if col_widths:
        for i, w in enumerate(col_widths):
            tbl.columns[i].width = w

    def set_cell(cell, text, bold=False, fill=None, txt_color=TEXTO_ESCURO, align=PP_ALIGN.CENTER):
        cell.text = text
        p = cell.text_frame.paragraphs[0]
        p.alignment = align
        run = p.runs[0] if p.runs else p.add_run()
        run.font.size = font_size
        run.font.bold = bold
        run.font.color.rgb = txt_color
        if fill:
            cell.fill.solid()
            cell.fill.fore_color.rgb = fill

    for ci, h in enumerate(headers):
        set_cell(tbl.cell(0, ci), h, bold=True, fill=header_fill,
                 txt_color=header_text_color)

    for ri, row in enumerate(rows):
        fill = row_fills[ri] if row_fills and ri < len(row_fills) else (
            BRANCO if ri % 2 == 0 else AZUL_CLARO
        )
        for ci, val in enumerate(row):
            align = PP_ALIGN.LEFT if ci == 0 else PP_ALIGN.CENTER
            set_cell(tbl.cell(ri + 1, ci), val, fill=fill, align=align)

    return tbl


# ╔══════════════════════════════════════════════════════════════════════════╗
# ║  SLIDES                                                                  ║
# ╚══════════════════════════════════════════════════════════════════════════╝

prs = new_prs()

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 1 — Título
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, AZUL_ESCURO)

# Faixa de acento superior
add_rect(s, 0, 0, SLIDE_W, Inches(0.18), fill_color=AMARELO_ACC)

# Caixa central branca
add_rect(s, Inches(0.7), Inches(0.8), Inches(11.9), Inches(5.5),
         fill_color=BRANCO, line_color=AZUL_MEDIO, line_width=Pt(1.5))

# Título do artigo
add_textbox(s, Inches(1.0), Inches(1.0), Inches(11.3), Inches(1.6),
            "An Empirical Assessment of Different Word Embedding\nand Deep Learning Models for Bug Assignment",
            font_size=Pt(26), bold=True, color=AZUL_ESCURO, align=PP_ALIGN.LEFT)

# Linha separadora
add_rect(s, Inches(1.0), Inches(2.72), Inches(11.3), Inches(0.05),
         fill_color=AMARELO_ACC)

# Metadados
meta = [
    ("Autores: Wang et al.", Pt(15), False, TEXTO_ESCURO),
    ("Journal of Systems and Software, Elsevier — 2024", Pt(15), False, AZUL_MEDIO),
    ("DOI: 10.1016/j.jss.2024.111961", Pt(13), False, RGBColor(0x55, 0x55, 0x55)),
]
y = Inches(2.9)
for text, sz, bold, clr in meta:
    add_textbox(s, Inches(1.0), y, Inches(11.3), Inches(0.4),
                text, font_size=sz, bold=bold, color=clr)
    y += Inches(0.42)

# Apresentador
add_rect(s, Inches(0.7), Inches(5.0), Inches(11.9), Inches(0.9),
         fill_color=AZUL_ESCURO)
add_textbox(s, Inches(1.0), Inches(5.08), Inches(11.3), Inches(0.45),
            "Apresentado por: Pedro Mesquita Maia",
            font_size=Pt(16), bold=True, color=AMARELO_ACC)
add_textbox(s, Inches(1.0), Inches(5.48), Inches(11.3), Inches(0.35),
            "Engenharia de Software — PUC-Rio  |  Junho de 2026",
            font_size=Pt(12), color=BRANCO)

# Faixa inferior
add_rect(s, 0, SLIDE_H - Inches(0.32), SLIDE_W, Inches(0.32),
         fill_color=RGBColor(0x0D, 0x1B, 0x3E))

add_notes(s, """Bom dia/boa tarde a todos.
Hoje vou apresentar a reprodução do artigo 'An Empirical Assessment of Different Word Embedding and Deep Learning Models for Bug Assignment', publicado no Journal of Systems and Software em 2024.
O artigo foi escrito por Wang et al. e propõe uma avaliação empírica de várias combinações de embeddings e modelos de deep learning para o problema de atribuição automática de bugs a desenvolvedores.
Meu nome é Pedro Mesquita Maia e esta apresentação faz parte da disciplina de Engenharia de Software.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 2 — Informações Gerais
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Informações Gerais", "Sobre o artigo selecionado")
footer_bar(s)

items = [
    ("Publicação", [
        "Periódico: Journal of Systems and Software (JSS)",
        "Editora:  Elsevier",
        "Ano: 2024  |  Acesso: ScienceDirect",
        "DOI: 10.1016/j.jss.2024.111961",
    ]),
    ("Indicadores de Qualidade", [
        "Indexado: Scopus e Web of Science",
        "Área: Engenharia de Software internacional",
        "Repositório público: github.com/AI4BA/dl4ba",
        "Código-fonte e datasets disponibilizados pelos autores",
    ]),
]

cols = [Inches(0.4), Inches(6.85)]
for col_x, (box_title, bullet_lines) in zip(cols, items):
    add_rect(s, col_x, Inches(1.3), Inches(6.1), Inches(5.6),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_textbox(s, col_x + Inches(0.15), Inches(1.35), Inches(5.8), Inches(0.45),
                box_title, font_size=Pt(15), bold=True, color=AZUL_MEDIO)
    add_rect(s, col_x + Inches(0.15), Inches(1.78), Inches(5.8), Inches(0.04),
             fill_color=AZUL_CLARO)
    y = Inches(1.88)
    for line in bullet_lines:
        add_textbox(s, col_x + Inches(0.3), y, Inches(5.6), Inches(0.5),
                    "• " + line, font_size=Pt(14), color=TEXTO_ESCURO)
        y += Inches(0.52)

add_notes(s, """Este artigo foi publicado no Journal of Systems and Software, um dos principais periódicos internacionais de engenharia de software.
É indexado no Scopus e no Web of Science, o que indica alta relevância acadêmica.
Os autores disponibilizaram o código-fonte e os datasets no GitHub, o que facilitou a tentativa de reprodução.
O artigo foi selecionado justamente por combinar um problema prático — atribuição automática de bugs — com técnicas modernas de NLP e aprendizado de máquina.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 3 — Contexto
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Contexto", "O que é atribuição automática de bugs?")
footer_bar(s)

# Diagrama: Bug Report → Modelo → Desenvolvedor
boxes = [
    (Inches(0.5),  "Bug\nReport",    AZUL_ESCURO, BRANCO),
    (Inches(4.1),  "Modelo\nNLP/ML", AZUL_MEDIO,  BRANCO),
    (Inches(7.7),  "Desenvolvedor\nPrevisto", VERDE, BRANCO),
]
for bx, label, fill, tc in boxes:
    add_rect(s, bx, Inches(2.0), Inches(2.9), Inches(1.5),
             fill_color=fill, line_color=None)
    add_textbox(s, bx, Inches(2.0), Inches(2.9), Inches(1.5),
                label, font_size=Pt(17), bold=True, color=tc, align=PP_ALIGN.CENTER)

# Setas entre caixas
for ax in [Inches(3.45), Inches(7.05)]:
    add_textbox(s, ax, Inches(2.5), Inches(0.6), Inches(0.5),
                "→", font_size=Pt(28), bold=True, color=AZUL_MEDIO, align=PP_ALIGN.CENTER)

# Bullets de explicação
bullets = [
    "Entrada: texto do bug report (título + descrição)",
    "Saída: desenvolvedor mais adequado para corrigir o bug",
    "Aplicação: sistemas de rastreamento de bugs (Bugzilla, Jira, GitHub Issues)",
    "Objetivo: automatizar uma tarefa repetitiva e crítica no desenvolvimento de software",
]
y = Inches(4.0)
for b in bullets:
    add_textbox(s, Inches(0.5), y, Inches(12.3), Inches(0.42),
                "• " + b, font_size=Pt(15), color=TEXTO_ESCURO)
    y += Inches(0.46)

add_notes(s, """Atribuição de bugs é o processo de decidir qual desenvolvedor deve corrigir um determinado bug report.
Normalmente isso é feito manualmente por um gerente ou triador de bugs.
O que o artigo propõe é automatizar esse processo usando técnicas de processamento de linguagem natural e aprendizado de máquina.
A ideia é simples: o texto do bug report é passado para um modelo que retorna o desenvolvedor mais provável de resolver o problema.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 4 — Problema
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Problema", "Por que é necessário automatizar?")
footer_bar(s)

problems = [
    ("Processo manual e lento",
     "Em projetos grandes, centenas de bugs são reportados por semana.\nA triagem manual não acompanha o volume."),
    ("Dependência de experiência humana",
     "O triador precisa conhecer a expertise de cada desenvolvedor.\nEsse conhecimento não é facilmente transferível."),
    ("Atribuição incorreta gera retrabalho",
     "Um bug atribuído ao desenvolvedor errado precisa ser reatribuído,\natrasando a correção e aumentando o custo."),
    ("Escala dos projetos open-source",
     "GCC, Firefox e Eclipse JDT têm dezenas de milhares de bugs\ne dezenas de desenvolvedores ativos."),
]

cols_x = [Inches(0.4), Inches(6.85)]
rows_y = [Inches(1.35), Inches(3.55)]

for idx, (title, desc) in enumerate(problems):
    cx = cols_x[idx % 2]
    cy = rows_y[idx // 2]
    add_rect(s, cx, cy, Inches(6.0), Inches(1.9),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_rect(s, cx, cy, Inches(0.12), Inches(1.9), fill_color=VERMELHO)
    add_textbox(s, cx + Inches(0.22), cy + Inches(0.1), Inches(5.65), Inches(0.42),
                title, font_size=Pt(14), bold=True, color=VERMELHO)
    add_textbox(s, cx + Inches(0.22), cy + Inches(0.52), Inches(5.65), Inches(1.3),
                desc, font_size=Pt(13), color=TEXTO_ESCURO)

add_notes(s, """O problema de atribuição manual de bugs se torna crítico à medida que os projetos crescem.
Projetos como GCC, Firefox e Eclipse recebem centenas de bugs por semana, e a triagem manual é simplesmente insustentável nessa escala.
Além disso, quando um bug é atribuído ao desenvolvedor errado, ele precisa ser reatribuído — o que atrasa a correção e aumenta o custo de manutenção.
Este é justamente o problema que o artigo busca resolver com aprendizado de máquina.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 5 — Motivação para Seleção
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Motivação para Seleção", "Por que este artigo foi escolhido?")
footer_bar(s)

motivations = [
    ("🔬", "Problema relevante", "Atribuição de bugs é um desafio real e prático em\nEngenharia de Software"),
    ("🧠", "NLP + Machine Learning", "Combina técnicas modernas: embeddings contextuais,\nredes neurais profundas"),
    ("📦", "Artefatos disponíveis", "Datasets e código-fonte públicos no GitHub,\npermitindo verificação independente"),
    ("⚡", "Reprodução viável", "O escopo pode ser simplificado para um\nambiente sem GPU de alto desempenho"),
]

for idx, (icon, title, desc) in enumerate(motivations):
    cx = cols_x[idx % 2]
    cy = rows_y[idx // 2]
    add_rect(s, cx, cy, Inches(6.0), Inches(1.9),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_textbox(s, cx + Inches(0.15), cy + Inches(0.1), Inches(5.65), Inches(0.55),
                title, font_size=Pt(15), bold=True, color=AZUL_MEDIO)
    add_textbox(s, cx + Inches(0.15), cy + Inches(0.62), Inches(5.65), Inches(1.2),
                desc, font_size=Pt(13), color=TEXTO_ESCURO)

add_notes(s, """Escolhi este artigo por combinar um problema prático com técnicas modernas de NLP.
O fato de os autores disponibilizarem o código e os datasets no GitHub foi fundamental — isso é raro e demonstra um compromisso com reprodutibilidade.
Além disso, o problema é escalável: é possível rodar uma versão simplificada com recursos computacionais modestos, o que tornou a reprodução viável dentro do prazo da disciplina.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 6 — Questões de Pesquisa
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Questões de Pesquisa", "O que o artigo investiga?")
footer_bar(s)

rqs = [
    ("RQ1", "Qual combinação de embedding e modelo obtém\nmelhor desempenho na atribuição de bugs?",
     "Compara 5 embeddings × 7 classificadores = 35 combinações"),
    ("RQ2", "Os resultados são consistentes entre\ndiferentes datasets?",
     "Avalia GCC, Eclipse JDT e Firefox separadamente e em conjunto"),
    ("RQ3", "Usar summary + description melhora\no desempenho comparado a usar apenas um campo?",
     "Testa a concatenação dos dois campos textuais do bug report"),
]

y = Inches(1.4)
for rq, question, detail in rqs:
    add_rect(s, Inches(0.4), y, Inches(12.5), Inches(1.55),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_rect(s, Inches(0.4), y, Inches(0.85), Inches(1.55), fill_color=AZUL_ESCURO)
    add_textbox(s, Inches(0.4), y + Inches(0.5), Inches(0.85), Inches(0.55),
                rq, font_size=Pt(14), bold=True, color=BRANCO, align=PP_ALIGN.CENTER)
    add_textbox(s, Inches(1.38), y + Inches(0.08), Inches(8.2), Inches(0.58),
                question, font_size=Pt(15), bold=True, color=AZUL_ESCURO)
    add_textbox(s, Inches(1.38), y + Inches(0.68), Inches(8.2), Inches(0.72),
                detail, font_size=Pt(13), color=RGBColor(0x55, 0x55, 0x55))
    add_rect(s, Inches(9.7), y + Inches(0.25), Inches(3.0), Inches(1.0),
             fill_color=AZUL_CLARO)
    y += Inches(1.75)

add_notes(s, """O artigo é estruturado em torno de três questões de pesquisa.
A RQ1 é a principal: qual combinação de embedding e modelo de deep learning produz os melhores resultados?
A RQ2 verifica se os resultados são generalizáveis — ou seja, se uma combinação que funciona bem no GCC também funciona no Firefox.
A RQ3 investiga se usar tanto o título quanto a descrição do bug report melhora o desempenho em relação a usar apenas um deles.
Essas questões guiam toda a estrutura experimental do artigo.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 7 — Datasets
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Datasets", "Conjuntos de dados utilizados")
footer_bar(s)

datasets = [
    ("GCC", "GNU Compiler Collection", VERDE),
    ("Eclipse JDT", "Java Development Tools", AZUL_MEDIO),
    ("Mozilla Firefox", "Navegador web open-source", RGBColor(0xE6, 0x7E, 0x22)),
]

dx = Inches(0.4)
for name, full, clr in datasets:
    add_rect(s, dx, Inches(1.3), Inches(3.9), Inches(4.5),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_rect(s, dx, Inches(1.3), Inches(3.9), Inches(0.5), fill_color=clr)
    add_textbox(s, dx + Inches(0.1), Inches(1.33), Inches(3.7), Inches(0.44),
                name, font_size=Pt(16), bold=True, color=BRANCO)
    add_textbox(s, dx + Inches(0.1), Inches(1.82), Inches(3.7), Inches(0.35),
                full, font_size=Pt(12), color=RGBColor(0x55, 0x55, 0x55))
    dx += Inches(4.45)

# Estrutura de cada registro
add_rect(s, Inches(0.4), Inches(5.95), Inches(12.5), Inches(0.05),
         fill_color=AZUL_CLARO)
add_textbox(s, Inches(0.4), Inches(6.0), Inches(12.5), Inches(0.35),
            "Cada registro contém:   Bug ID  •  Summary (título)  •  Description (texto longo)  •  Assignee (desenvolvedor — variável alvo)  •  Status",
            font_size=Pt(13), color=RGBColor(0x44, 0x44, 0x44), align=PP_ALIGN.CENTER)

# Tabela de resumo
headers_d = ["Dataset", "Registros", "Desenvolvedores únicos", "Disponível no repositório"]
rows_d = [
    ["GCC",          "2.103",  "82",  "Sim (gcc_data.csv.zip)"],
    ["Eclipse JDT",  "~10 k",  "~72", "Sim (jdt_data.csv.zip)"],
    ["Mozilla Firefox", "~26 k", "~88", "Sim (sun_firefox.csv.zip)"],
]
add_table(s, Inches(0.4), Inches(1.9), Inches(12.5), headers_d, rows_d,
          font_size=Pt(13),
          col_widths=[Inches(2.5), Inches(2.0), Inches(3.2), Inches(4.8)])

add_notes(s, """O artigo utiliza três datasets clássicos da literatura de atribuição de bugs.
GCC é o menor, com cerca de 2.100 registros e 82 desenvolvedores únicos.
Eclipse JDT e Firefox são maiores, com cerca de 10 mil e 26 mil registros respectivamente.
Um ponto muito positivo: todos os três datasets estão disponíveis diretamente no repositório GitHub dos autores como arquivos CSV compactados — nenhum download externo é necessário.
Cada registro tem cinco campos: o identificador do bug, o título, a descrição, o desenvolvedor atribuído e o status de resolução.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 8 — Embeddings
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Word Embeddings", "Como representar texto para modelos de ML?")
footer_bar(s)

add_textbox(s, Inches(0.4), Inches(1.22), Inches(12.5), Inches(0.42),
            "Embedding = transformação de palavras em vetores numéricos que capturam seu significado",
            font_size=Pt(15), italic=True, color=AZUL_MEDIO)

embeddings = [
    ("Word2Vec", "Estático", "Cada palavra → vetor fixo.\nTreinado no GoogleNews (3,6 GB).", AZUL_ESCURO),
    ("GloVe", "Estático", "Baseado em co-ocorrência global.\nVersão 840B 300d (2,2 GB).", AZUL_MEDIO),
    ("ELMo", "Contextual", "Vetor muda conforme o contexto.\nUsa LSTM bidirecional (350 MB).", VERDE),
    ("BERT", "Contextual", "Transformers bidirecional.\nBert-base-uncased (440 MB).", RGBColor(0xC0, 0x39, 0x2B)),
    ("NextBug", "Especializado", "Treinado especificamente em\nbug reports (DeepSIM).", RGBColor(0x7D, 0x3C, 0x98)),
]

ex = Inches(0.4)
for name, tipo, desc, clr in embeddings:
    add_rect(s, ex, Inches(1.8), Inches(2.35), Inches(4.6),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_rect(s, ex, Inches(1.8), Inches(2.35), Inches(0.42), fill_color=clr)
    add_textbox(s, ex + Inches(0.08), Inches(1.82), Inches(2.2), Inches(0.38),
                name, font_size=Pt(14), bold=True, color=BRANCO)
    badge_clr = RGBColor(0xE8, 0xF8, 0xF5) if tipo == "Contextual" else RGBColor(0xEA, 0xF2, 0xFB)
    add_rect(s, ex + Inches(0.08), Inches(2.32), Inches(2.2), Inches(0.3),
             fill_color=badge_clr, line_color=clr, line_width=Pt(0.5))
    add_textbox(s, ex + Inches(0.08), Inches(2.32), Inches(2.2), Inches(0.3),
                tipo, font_size=Pt(11), color=clr, align=PP_ALIGN.CENTER)
    add_textbox(s, ex + Inches(0.08), Inches(2.72), Inches(2.22), Inches(1.55),
                desc, font_size=Pt(12), color=TEXTO_ESCURO)
    ex += Inches(2.52)

add_notes(s, """Antes de alimentar o texto para um modelo de machine learning, precisamos convertê-lo em números.
Os embeddings fazem isso: transformam palavras em vetores numéricos que capturam o significado semântico.
O artigo compara cinco técnicas: Word2Vec e GloVe são estáticos — cada palavra tem sempre o mesmo vetor.
ELMo e BERT são contextuais — o vetor de uma palavra muda dependendo da frase em que ela aparece, o que é mais poderoso.
NextBug é um embedding especializado, treinado especificamente em bug reports, o que teoricamente o tornaria mais adequado para essa tarefa.
Um problema prático: esses modelos são grandes — Word2Vec ocupa 3,6 GB, GloVe 2,2 GB — o que dificulta a reprodução.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 9 — Modelos
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Modelos de Classificação", "Arquiteturas de deep learning avaliadas")
footer_bar(s)

models = [
    ("MLP", "Multilayer\nPerceptron", "Rede totalmente conectada.\nBaseline simples."),
    ("TextCNN", "Convolutional\nNeural Network", "Filtros convolucionais\nsobre o texto."),
    ("LSTM", "Long Short-Term\nMemory", "Processa a sequência\nde palavras."),
    ("LSTM-Att", "LSTM com\nAtenção", "LSTM + mecanismo\nde atenção."),
    ("Bi-LSTM", "Bi-direcional\nLSTM", "Lê o texto nos\ndois sentidos."),
    ("Bi-LSTM-Att", "Bi-LSTM com\nAtenção", "Bi-LSTM + atenção.\nMelhor resultado."),
]

ex = Inches(0.3)
for i, (short, full, desc) in enumerate(models):
    clr = AZUL_ESCURO if i < 2 else (AZUL_MEDIO if i < 4 else VERDE)
    if i == 5:
        clr = RGBColor(0xC0, 0x39, 0x2B)
    add_rect(s, ex, Inches(1.3), Inches(2.1), Inches(4.6),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_rect(s, ex, Inches(1.3), Inches(2.1), Inches(0.5), fill_color=clr)
    add_textbox(s, ex + Inches(0.05), Inches(1.33), Inches(2.0), Inches(0.44),
                short, font_size=Pt(14), bold=True, color=BRANCO)
    add_textbox(s, ex + Inches(0.08), Inches(1.86), Inches(1.95), Inches(0.6),
                full, font_size=Pt(11), bold=True, color=clr)
    add_textbox(s, ex + Inches(0.08), Inches(2.5), Inches(1.95), Inches(1.35),
                desc, font_size=Pt(12), color=TEXTO_ESCURO)
    ex += Inches(2.18)

add_rect(s, Inches(0.3), Inches(6.1), Inches(12.7), Inches(0.75),
         fill_color=AZUL_CLARO)
add_textbox(s, Inches(0.5), Inches(6.15), Inches(12.3), Inches(0.65),
            "Total avaliado: 5 embeddings × 7 modelos (incluindo Naive Bayes) = 35 combinações únicas",
            font_size=Pt(14), bold=True, color=AZUL_ESCURO, align=PP_ALIGN.CENTER)

add_notes(s, """O artigo avalia seis arquiteturas de deep learning mais o Naive Bayes como baseline.
MLP é a rede mais simples: camadas totalmente conectadas.
TextCNN usa filtros convolucionais que capturam padrões locais no texto — similar a como funciona para imagens.
LSTM e suas variantes processam o texto como uma sequência, o que é natural para linguagem.
A versão bidirecional lê o texto da esquerda para direita e da direita para esquerda, capturando contexto dos dois lados.
O mecanismo de atenção permite que o modelo 'preste atenção' nas partes mais relevantes do texto.
Ao combinar com os 5 embeddings, chegamos a 35 configurações diferentes avaliadas no artigo.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 10 — Metodologia Original
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Metodologia Original", "Pipeline proposto pelo artigo")
footer_bar(s)

steps = [
    ("1\nBug\nReport", AZUL_ESCURO, "Texto do\nbug report\n(summary +\ndescription)"),
    ("2\nPré-\nprocesso", AZUL_MEDIO, "Remoção de\nstopwords,\nlimpeza e\ntokenização"),
    ("3\nEmbedding", RGBColor(0x1A, 0x79, 0x6B), "Word2Vec /\nGloVe / ELMo\n/ BERT /\nNextBug"),
    ("4\nModelo\nDL", RGBColor(0xC0, 0x39, 0x2B), "TextCNN,\nLSTM,\nBi-LSTM,\nMLP..."),
    ("5\nDesenv.\nPrevisto", VERDE, "Ranking de\ndesenvolv.\npor\nprobabilidade"),
]

px = Inches(0.3)
for i, (label, clr, detail) in enumerate(steps):
    add_rect(s, px, Inches(1.55), Inches(2.35), Inches(1.65),
             fill_color=clr)
    add_textbox(s, px, Inches(1.55), Inches(2.35), Inches(1.65),
                label, font_size=Pt(14), bold=True, color=BRANCO, align=PP_ALIGN.CENTER)
    add_rect(s, px, Inches(3.3), Inches(2.35), Inches(1.45),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_textbox(s, px + Inches(0.08), Inches(3.35), Inches(2.2), Inches(1.35),
                detail, font_size=Pt(12), color=TEXTO_ESCURO, align=PP_ALIGN.CENTER)
    if i < 4:
        add_textbox(s, px + Inches(2.38), Inches(2.1), Inches(0.35), Inches(0.8),
                    "→", font_size=Pt(24), bold=True, color=AZUL_MEDIO,
                    align=PP_ALIGN.CENTER)
    px += Inches(2.57)

# Avaliação
add_rect(s, Inches(0.3), Inches(5.0), Inches(12.7), Inches(1.2),
         fill_color=AZUL_CLARO)
add_textbox(s, Inches(0.5), Inches(5.05), Inches(12.3), Inches(0.4),
            "Avaliação: 10-fold Cross-Validation   |   Métricas: Top-1, Top-5, Top-10 Accuracy e MRR",
            font_size=Pt(14), bold=True, color=AZUL_ESCURO, align=PP_ALIGN.CENTER)
add_textbox(s, Inches(0.5), Inches(5.5), Inches(12.3), Inches(0.6),
            "Análise estatística: Cliff's Delta (tamanho de efeito) + Teste de Wilcoxon (significância)",
            font_size=Pt(13), color=TEXTO_ESCURO, align=PP_ALIGN.CENTER)

add_notes(s, """O pipeline do artigo tem cinco etapas principais.
Primeiro, o texto do bug report é carregado — tanto o título (summary) quanto a descrição.
Em seguida, o texto passa por pré-processamento: remoção de stopwords, caracteres especiais e tokenização.
O texto limpo é então convertido em vetores numéricos usando um dos cinco embeddings avaliados.
Esses vetores alimentam um dos modelos de deep learning, que produz um ranking de desenvolvedores por probabilidade.
A avaliação usa validação cruzada de 10 folds, e as conclusões sobre diferenças entre modelos são validadas com testes estatísticos.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 11 — Métricas de Avaliação
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Métricas de Avaliação", "Como medir a qualidade das predições?")
footer_bar(s)

metrics_info = [
    ("Top-1\nAccuracy", AZUL_ESCURO,
     "O modelo acertou o desenvolvedor na 1ª sugestão?\n\nTop-1 = 0.52  →  acerta 1 em cada 2 bugs"),
    ("Top-5\nAccuracy", AZUL_MEDIO,
     "O correto está entre as 5 primeiras sugestões?\n\nTop-5 = 0.75  →  acerta 3 em cada 4 bugs"),
    ("Top-10\nAccuracy", VERDE,
     "O correto está entre as 10 primeiras sugestões?\n\nTop-10 = 0.85  →  acerta 5 em cada 6 bugs"),
    ("MRR", RGBColor(0xC0, 0x39, 0x2B),
     "Posição média do correto na lista ordenada.\nMRR = média de 1/posição\n\nMRR = 0.63  →  correto ~na posição 1.6"),
]

mx = Inches(0.3)
for label, clr, desc in metrics_info:
    add_rect(s, mx, Inches(1.3), Inches(2.95), Inches(4.8),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_rect(s, mx, Inches(1.3), Inches(2.95), Inches(0.62), fill_color=clr)
    add_textbox(s, mx, Inches(1.3), Inches(2.95), Inches(0.62),
                label, font_size=Pt(14), bold=True, color=BRANCO, align=PP_ALIGN.CENTER)
    add_textbox(s, mx + Inches(0.1), Inches(2.0), Inches(2.75), Inches(3.0),
                desc, font_size=Pt(13), color=TEXTO_ESCURO)
    mx += Inches(3.1)

add_textbox(s, Inches(0.3), Inches(6.15), Inches(12.7), Inches(0.38),
            "Validação: 10-fold Cross-Validation estratificada — mesma proporção de classes em cada fold",
            font_size=Pt(13), italic=True, color=RGBColor(0x55, 0x55, 0x55),
            align=PP_ALIGN.CENTER)

add_notes(s, """As métricas usadas são padrão em sistemas de recomendação e recuperação de informação.
Top-1 é a mais exigente: o modelo precisa acertar o desenvolvedor exato na primeira posição.
Top-5 e Top-10 são mais tolerantes: o correto precisa aparecer entre os k primeiros — útil em sistemas onde um humano escolhe entre as sugestões.
MRR mede a posição média do correto na lista. Um MRR de 0.63 significa que, em média, o desenvolvedor correto aparece próximo da segunda posição.
A validação cruzada de 10 folds garante que os resultados não dependam de uma divisão específica dos dados.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 12 — Resultados Principais do Artigo
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Resultados Principais", "O que o artigo concluiu?")
footer_bar(s)

findings = [
    (VERDE,     "ELMo foi o embedding mais eficaz",
                "Embeddings contextuais (ELMo, BERT) superaram\nconsistentemente os estáticos (Word2Vec, GloVe, NextBug)"),
    (AZUL_MEDIO, "Bi-LSTM com Atenção foi o melhor modelo",
                "Captura relações bidirecionais e pondera\nas partes mais relevantes do texto"),
    (RGBColor(0xE6, 0x7E, 0x22), "Summary + Description melhora os resultados",
                "Usar os dois campos textuais do bug report\naumenta o desempenho em todos os datasets (RQ3)"),
    (RGBColor(0x7D, 0x3C, 0x98), "Resultados variam entre datasets (RQ2)",
                "O melhor modelo em GCC não é necessariamente\no melhor em Firefox — contexto importa"),
]

y = Inches(1.3)
for clr, title, detail in findings:
    add_rect(s, Inches(0.4), y, Inches(12.5), Inches(1.2),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_rect(s, Inches(0.4), y, Inches(0.15), Inches(1.2), fill_color=clr)
    add_textbox(s, Inches(0.65), y + Inches(0.1), Inches(12.0), Inches(0.42),
                title, font_size=Pt(15), bold=True, color=clr)
    add_textbox(s, Inches(0.65), y + Inches(0.52), Inches(12.0), Inches(0.6),
                detail, font_size=Pt(13), color=TEXTO_ESCURO)
    y += Inches(1.3)

add_notes(s, """Os principais resultados do artigo são quatro.
Primeiro: ELMo foi o embedding mais eficaz na maioria dos experimentos — embeddings contextuais superam os estáticos.
Segundo: Bi-LSTM com atenção foi o melhor modelo geral, atingindo Top-1 de aproximadamente 0.62 no dataset JDT.
Terceiro: combinar o título e a descrição do bug report melhora o desempenho — a RQ3 confirma que ambos os campos são informativos.
Quarto: os resultados não são completamente generalizáveis — o melhor modelo em um dataset não é sempre o melhor nos demais.
Essa última observação é importante: soluções de atribuição de bugs podem precisar ser ajustadas por projeto.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 13 — Artefatos Disponíveis
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Artefatos Disponíveis", "O que os autores disponibilizaram?")
footer_bar(s)

add_rect(s, Inches(0.4), Inches(1.22), Inches(12.5), Inches(0.5),
         fill_color=AZUL_ESCURO)
add_textbox(s, Inches(0.6), Inches(1.28), Inches(12.1), Inches(0.38),
            "github.com/AI4BA/dl4ba",
            font_size=Pt(16), bold=True, color=AMARELO_ACC, align=PP_ALIGN.CENTER)

artifact_cols = [
    ("Datasets", [
        "gcc_data.csv.zip (2.103 registros)",
        "jdt_data.csv.zip",
        "sun_firefox.csv.zip",
        "Sem necessidade de cadastro",
    ], VERDE),
    ("Código-fonte", [
        "models.py — TextCNN, LSTM, Bi-LSTM, MLP",
        "train.py — script CLI de treinamento",
        "utils.py — pré-processamento + embeddings",
        "parameters.json — hiperparâmetros",
    ], AZUL_MEDIO),
    ("Resultados", [
        "res/rq1/ — resultados por modelo",
        "res/rq2/ — médias por dataset",
        "res/rq3/ — ablação summary vs desc",
        "Testes de Wilcoxon e Cliff's Delta",
    ], RGBColor(0xC0, 0x39, 0x2B)),
]

ax = Inches(0.4)
for title, items, clr in artifact_cols:
    add_rect(s, ax, Inches(1.82), Inches(4.05), Inches(4.9),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(1))
    add_rect(s, ax, Inches(1.82), Inches(4.05), Inches(0.48), fill_color=clr)
    add_textbox(s, ax + Inches(0.08), Inches(1.85), Inches(3.9), Inches(0.42),
                title, font_size=Pt(14), bold=True, color=BRANCO)
    yi = Inches(2.4)
    for item in items:
        add_textbox(s, ax + Inches(0.12), yi, Inches(3.82), Inches(0.45),
                    "✓  " + item, font_size=Pt(12), color=TEXTO_ESCURO)
        yi += Inches(0.48)
    ax += Inches(4.3)

add_notes(s, """O repositório do artigo é bem estruturado e contém três categorias de artefatos.
Os datasets estão todos disponíveis como arquivos CSV compactados — basta fazer o download e descompactar.
O código-fonte inclui as implementações dos modelos, o script de treinamento com interface de linha de comando, e as funções de pré-processamento.
Os resultados pré-computados são uma adição valiosa: mesmo sem rodar todos os experimentos, é possível verificar as conclusões do artigo consultando as planilhas em res/.
Esse nível de disponibilidade de artefatos é um diferencial positivo do artigo.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 14 — Plano de Replicação
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Plano de Replicação", "Por que uma reprodução simplificada?")
footer_bar(s)

# Coluna esquerda: obstáculos
add_rect(s, Inches(0.4), Inches(1.25), Inches(6.0), Inches(5.55),
         fill_color=BRANCO, line_color=RGBColor(0xFF, 0x80, 0x80), line_width=Pt(1))
add_rect(s, Inches(0.4), Inches(1.25), Inches(6.0), Inches(0.45),
         fill_color=VERMELHO)
add_textbox(s, Inches(0.5), Inches(1.27), Inches(5.8), Inches(0.41),
            "Obstáculos para reprodução completa", font_size=Pt(14),
            bold=True, color=BRANCO)
obstacles = [
    "PyTorch 1.12.1 é incompatível com Python ≥ 3.10",
    "Embeddings externos somam > 7 GB de download",
    "GPU RTX 3090 necessária para execução eficiente",
    "Caminhos de arquivos hard-coded no código-fonte",
    "Ausência de script de download automatizado",
    "README sem exemplos de comandos executáveis",
]
yi = Inches(1.82)
for obs in obstacles:
    add_textbox(s, Inches(0.55), yi, Inches(5.7), Inches(0.45),
                "✗  " + obs, font_size=Pt(12.5), color=VERMELHO)
    yi += Inches(0.48)

# Coluna direita: solução
add_rect(s, Inches(6.85), Inches(1.25), Inches(6.05), Inches(5.55),
         fill_color=BRANCO, line_color=RGBColor(0x80, 0xC0, 0x80), line_width=Pt(1))
add_rect(s, Inches(6.85), Inches(1.25), Inches(6.05), Inches(0.45),
         fill_color=VERDE)
add_textbox(s, Inches(6.95), Inches(1.27), Inches(5.85), Inches(0.41),
            "Reprodução simplificada adotada", font_size=Pt(14),
            bold=True, color=BRANCO)
solutions = [
    "Dataset: GCC (disponível no repositório)",
    "Embedding: TF-IDF (sem download externo)",
    "Modelo: Regressão Logística",
    "Execução: 100% em CPU (~30 segundos)",
    "Validação: 10-fold CV (igual ao artigo)",
    "Métricas: Top-1, Top-5, Top-10, MRR",
]
yi = Inches(1.82)
for sol in solutions:
    add_textbox(s, Inches(7.0), yi, Inches(5.75), Inches(0.45),
                "✓  " + sol, font_size=Pt(12.5), color=VERDE)
    yi += Inches(0.48)

add_notes(s, """Para reproduzir o artigo integralmente seria necessário: criar um ambiente Python 3.9, baixar mais de 7 GB de modelos pré-treinados, e ter acesso a uma GPU RTX 3090.
Essas restrições tornaram a reprodução completa inviável dentro do prazo e com os recursos disponíveis.
A solução foi uma reprodução simplificada que mantém os elementos essenciais do pipeline: o mesmo dataset, as mesmas métricas e o mesmo protocolo de validação cruzada.
A diferença é no modelo: em vez de deep learning com embeddings pesados, usamos TF-IDF com Regressão Logística, que roda em qualquer computador em menos de um minuto.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 15 — Reprodução Simplificada
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Reprodução Simplificada", "Configuração do experimento executado")
footer_bar(s)

# Bloco de configuração
cfg_items = [
    ("Dataset", "GCC  (gcc_data.csv — baixado do repositório original)"),
    ("Amostras", "1.987 registros após filtro de classes com < 10 amostras"),
    ("Classes", "36 desenvolvedores únicos (de 82 no total)"),
    ("Features", "Summary + Description concatenados e limpos"),
    ("Embedding", "TF-IDF  (max_features=20.000, ngram=(1,2), sublinear_tf=True)"),
    ("Modelo", "Regressão Logística multinomial  (C=1.0, solver=lbfgs)"),
    ("Validação", "10-fold Stratified Cross-Validation  (random_seed=1234)"),
    ("Hardware", "CPU — sem GPU — ~30 segundos de execução"),
]

y = Inches(1.3)
for label, value in cfg_items:
    add_rect(s, Inches(0.4), y, Inches(2.8), Inches(0.48), fill_color=AZUL_ESCURO)
    add_textbox(s, Inches(0.45), y + Inches(0.04), Inches(2.7), Inches(0.40),
                label, font_size=Pt(13), bold=True, color=BRANCO)
    add_rect(s, Inches(3.25), y, Inches(9.75), Inches(0.48),
             fill_color=BRANCO, line_color=AZUL_CLARO, line_width=Pt(0.8))
    add_textbox(s, Inches(3.35), y + Inches(0.04), Inches(9.55), Inches(0.40),
                value, font_size=Pt(13), color=TEXTO_ESCURO)
    y += Inches(0.56)

add_notes(s, """Aqui está a configuração exata do experimento que executei.
O dataset GCC tem 2.103 registros, mas após filtrar desenvolvedores com menos de 10 bugs ficamos com 1.987 amostras e 36 classes.
Essa filtragem é necessária para que a validação cruzada de 10 folds funcione — cada classe precisa ter amostras suficientes em cada fold.
O TF-IDF transforma o texto em vetores de frequência de termos, considerando unigramas e bigramas — pares de palavras consecutivas.
A Regressão Logística multinomial é um classificador linear clássico que produz probabilidades para cada classe, permitindo calcular o ranking de desenvolvedores.
Todo o experimento roda em cerca de 30 segundos em qualquer computador com Python e scikit-learn.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 16 — Resultados da Reprodução
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Resultados da Reprodução", "Métricas obtidas no experimento executado")
footer_bar(s)

headers_r = ["Métrica", "Média (10 folds)", "Desvio Padrão", "Interpretação"]
rows_r = [
    ["Top-1 Accuracy", "0.5229", "±0.0219", "1ª sugestão correta em 52% dos bugs"],
    ["Top-5 Accuracy", "0.7504", "±0.0127", "Correto entre os 5 primeiros em 75%"],
    ["Top-10 Accuracy", "0.8541", "±0.0142", "Correto entre os 10 primeiros em 85%"],
    ["MRR", "0.6279", "±0.0153", "Correto ~na posição 1.6 da lista"],
]
add_table(s, Inches(0.4), Inches(1.3), Inches(12.5), headers_r, rows_r,
          font_size=Pt(14),
          col_widths=[Inches(2.6), Inches(2.2), Inches(2.0), Inches(5.7)])

# Resultados por fold
add_textbox(s, Inches(0.4), Inches(3.75), Inches(12.5), Inches(0.38),
            "Resultados por fold (10-fold CV):",
            font_size=Pt(13), bold=True, color=AZUL_ESCURO)

fold_data = "Fold 1: 0.5226  |  Fold 2: 0.5025  |  Fold 3: 0.5528  |  Fold 4: 0.5327  |  Fold 5: 0.5126\n" \
            "Fold 6: 0.5427  |  Fold 7: 0.5377  |  Fold 8: 0.4899  |  Fold 9: 0.4899  |  Fold 10: 0.5455"
add_rect(s, Inches(0.4), Inches(4.18), Inches(12.5), Inches(0.95),
         fill_color=AZUL_CLARO)
add_textbox(s, Inches(0.55), Inches(4.22), Inches(12.2), Inches(0.87),
            fold_data, font_size=Pt(12.5), color=AZUL_ESCURO, align=PP_ALIGN.CENTER)

# Destaques
highlights = [
    ("Desvio padrão < 0.022 em todas as métricas", VERDE,
     "O modelo é estável — resultados não dependem do fold"),
    ("Execução: 28.8 segundos em CPU", AZUL_MEDIO,
     "Experimento reproduzível em qualquer computador"),
]
hx = Inches(0.4)
for ht, hclr, hdesc in highlights:
    add_rect(s, hx, Inches(5.28), Inches(6.2), Inches(0.95),
             fill_color=BRANCO, line_color=hclr, line_width=Pt(1.5))
    add_textbox(s, hx + Inches(0.1), Inches(5.32), Inches(6.0), Inches(0.38),
                ht, font_size=Pt(13), bold=True, color=hclr)
    add_textbox(s, hx + Inches(0.1), Inches(5.68), Inches(6.0), Inches(0.48),
                hdesc, font_size=Pt(12), color=TEXTO_ESCURO)
    hx += Inches(6.5)

add_notes(s, """Esses são os resultados reais obtidos ao executar o script de reprodução.
Top-1 de 0.52 significa que o modelo indica o desenvolvedor correto em mais da metade dos casos — na primeira sugestão.
Top-5 de 0.75 indica que 3 em cada 4 bugs têm o responsável correto entre as 5 primeiras sugestões.
MRR de 0.63 mostra que, em média, o correto aparece muito próximo do topo da lista.
O desvio padrão baixo em todos os folds indica que o modelo é estável — não depende de uma divisão específica dos dados.
Todo o experimento rodou em menos de 30 segundos numa CPU comum.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 17 — Comparação com o Artigo
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, CINZA_FUNDO)
header_bar(s, "Comparação com o Artigo Original", "Análise das diferenças nos resultados")
footer_bar(s)

headers_c = ["Métrica", "Artigo (TextCNN + Word2Vec, GCC)", "Reprodução (TF-IDF + LR, GCC)"]
rows_c = [
    ["Top-1 Accuracy", "~0.41", "0.5229"],
    ["Top-5 Accuracy", "~0.63", "0.7504"],
    ["Top-10 Accuracy", "~0.72", "0.8541"],
    ["MRR", "~0.49", "0.6279"],
    ["Classes avaliadas", "82 (todos)", "36 (filtrado ≥ 10 amostras)"],
    ["Tempo de execução", "Horas (GPU)", "~30 s (CPU)"],
]
add_table(s, Inches(0.4), Inches(1.3), Inches(12.5), headers_c, rows_c,
          font_size=Pt(13),
          col_widths=[Inches(2.8), Inches(4.4), Inches(5.3)])

# Nota de cautela
add_rect(s, Inches(0.4), Inches(5.0), Inches(12.5), Inches(1.35),
         fill_color=RGBColor(0xFF, 0xF3, 0xCD), line_color=AMARELO_ACC, line_width=Pt(1.5))
add_textbox(s, Inches(0.55), Inches(5.05), Inches(12.1), Inches(0.42),
            "Atenção — esta comparação deve ser interpretada com cautela:",
            font_size=Pt(14), bold=True, color=RGBColor(0x85, 0x64, 0x04))
add_textbox(s, Inches(0.55), Inches(5.48), Inches(12.1), Inches(0.75),
            "Os valores reproduzidos são numericamente superiores não por TF-IDF superar deep learning,\n"
            "mas porque a reprodução usa 36 classes (problema mais fácil) em vez das 82 do artigo original.",
            font_size=Pt(13), color=RGBColor(0x6D, 0x4C, 0x41))

add_notes(s, """Esta tabela compara os resultados da reprodução com os valores reportados pelo artigo para a configuração TextCNN + Word2Vec no dataset GCC.
Os valores da reprodução são numericamente superiores, mas isso não significa que TF-IDF supera deep learning.
A explicação é simples: o artigo avalia todos os 82 desenvolvedores do dataset, enquanto nossa reprodução filtra para apenas 36 — aqueles com pelo menos 10 amostras.
Com menos classes, o problema é mais fácil de classificar corretamente.
Outro ponto: o artigo relata que ELMo + Bi-LSTM com atenção atinge Top-1 de 0.62 no JDT, que provavelmente superaria nossa reprodução se testado nas mesmas condições.
O que podemos concluir é que o pipeline funciona e os resultados são da ordem de grandeza esperada.""")

# ─────────────────────────────────────────────────────────────────────────────
# SLIDE 18 — Conclusão
# ─────────────────────────────────────────────────────────────────────────────
s = blank_slide(prs)
fill_bg(s, AZUL_ESCURO)

# Faixa de acento
add_rect(s, 0, 0, SLIDE_W, Inches(0.18), fill_color=AMARELO_ACC)

add_textbox(s, Inches(0.5), Inches(0.3), Inches(12.3), Inches(0.65),
            "Conclusão", font_size=Pt(30), bold=True, color=BRANCO,
            align=PP_ALIGN.LEFT)
add_rect(s, Inches(0.5), Inches(0.95), Inches(2.5), Inches(0.05),
         fill_color=AMARELO_ACC)

conclusion_items = [
    (VERDE,  "Artefatos de qualidade",
             "Datasets, código e resultados pré-computados disponíveis no GitHub"),
    (AZUL_CLARO, "Reprodução parcialmente bem-sucedida",
             "O pipeline foi executado de ponta a ponta com resultados coerentes"),
    (AMARELO_ACC, "Modelo simplificado foi competitivo",
             "TF-IDF + LR atingiu Top-1 = 0.52 e MRR = 0.63 em 36 classes, em ~30 s"),
    (RGBColor(0xFF, 0x80, 0x80), "Desafio principal",
             "PyTorch desatualizado e embeddings externos de >7 GB sem automação"),
    (RGBColor(0xA8, 0xD8, 0xEA), "Lição aprendida",
             "Reprodutibilidade melhora com: ambiente dockerizado, scripts de download\ne exemplos de comandos no README"),
]

y = Inches(1.15)
for clr, title, detail in conclusion_items:
    add_rect(s, Inches(0.4), y, Inches(0.12), Inches(1.0), fill_color=clr)
    add_textbox(s, Inches(0.65), y + Inches(0.05), Inches(12.0), Inches(0.42),
                title, font_size=Pt(15), bold=True, color=clr)
    add_textbox(s, Inches(0.65), y + Inches(0.47), Inches(12.0), Inches(0.48),
                detail, font_size=Pt(13), color=RGBColor(0xCC, 0xCC, 0xCC))
    y += Inches(1.12)

add_rect(s, 0, SLIDE_H - Inches(0.32), SLIDE_W, Inches(0.32),
         fill_color=RGBColor(0x0D, 0x1B, 0x3E))
add_textbox(s, Inches(0.3), SLIDE_H - Inches(0.30), Inches(12.7), Inches(0.28),
            "Wang et al. | JSS 2024 | Reprodução — Pedro Mesquita Maia",
            font_size=Pt(9), color=RGBColor(0xBB, 0xCC, 0xDD),
            align=PP_ALIGN.LEFT)

add_notes(s, """Para concluir, o artigo disponibiliza artefatos de boa qualidade: datasets, código e resultados pré-computados — o que facilitou a verificação das conclusões mesmo sem reprodução completa.
A reprodução simplificada foi bem-sucedida: o pipeline rodou de ponta a ponta em menos de 30 segundos, com resultados coerentes com o problema.
O principal desafio foi a incompatibilidade do ambiente: PyTorch 1.12.1 com Python 3.13, e a ausência de automação para download dos embeddings.
A lição principal é que reprodutibilidade não depende apenas do código estar disponível — ela requer documentação clara, ambiente isolado e scripts de setup automatizados.
Fico à disposição para perguntas. Obrigado.""")

# ── Salvar ───────────────────────────────────────────────────────────────────
import os
out_path = os.path.join(os.path.dirname(__file__), "apresentacao_reproducao_dl4ba.pptx")
prs.save(out_path)
print(f"Salvo em: {out_path}")
print(f"Slides criados: {len(prs.slides)}")
print("Speaker notes: sim (todos os slides)")

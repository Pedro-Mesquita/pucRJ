/* Pedro Mesquita Maia 2312664 3WC*/

#include "converteutf832.h"

int verifyFile(FILE *arquivo_entrada, FILE *arquivo_saida);

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida)
{
    int goodFile = verifyFile(arquivo_entrada, arquivo_saida);

    if (goodFile != 0)
    {
        return -1;
    }

    unsigned char utf8Char;
    unsigned int utf32Char;
    unsigned char BOM[4] = {0xFF, 0xFE, 0x00, 0x00};

    if (fwrite(BOM, 1, 4, arquivo_saida) != 4)
    {
        fprintf(stderr, "Erro ao escrever o BOM\n");
        return -1;
    }

    while (fread(&utf8Char, 1, 1, arquivo_entrada) == 1)
    {
        if (utf8Char <= 0x7F)
        {
            utf32Char = utf8Char;
        }
        else if ((utf8Char & 0xE0) == 0xC0)
        {
            unsigned char utf8Char2;
            if (fread(&utf8Char2, 1, 1, arquivo_entrada) != 1)
            {
                fprintf(stderr, "Erro ao ler o segundo byte UTF-8\n");
                return -1;
            }
            utf32Char = ((utf8Char & 0x1F) << 6) | (utf8Char2 & 0x3F);
        }
        else if ((utf8Char & 0xF0) == 0xE0)
        {
            unsigned char utf8Char2, utf8Char3;
            if (fread(&utf8Char2, 1, 1, arquivo_entrada) != 1 || fread(&utf8Char3, 1, 1, arquivo_entrada) != 1)
            {
                fprintf(stderr, "Erro ao ler o segundo ou terceiro byte UTF-8\n");
                return -1;
            }
            utf32Char = ((utf8Char & 0x0F) << 12) | ((utf8Char2 & 0x3F) << 6) | (utf8Char3 & 0x3F);
        }
        else if ((utf8Char & 0xF8) == 0xF0)
        {
            unsigned char utf8Char2, utf8Char3, utf8Char4;
            if (fread(&utf8Char2, 1, 1, arquivo_entrada) != 1 || fread(&utf8Char3, 1, 1, arquivo_entrada) != 1 || fread(&utf8Char4, 1, 1, arquivo_entrada) != 1)
            {
                fprintf(stderr, "Erro ao ler o segundo, terceiro ou quarto byte UTF-8\n");
                return -1;
            }
            utf32Char = ((utf8Char & 0x07) << 18) | ((utf8Char2 & 0x3F) << 12) | ((utf8Char3 & 0x3F) << 6) | (utf8Char4 & 0x3F);
        }

        unsigned char utf32Bytes[4];
        utf32Bytes[0] = utf32Char & 0xFF;
        utf32Bytes[1] = (utf32Char >> 8) & 0xFF;
        utf32Bytes[2] = (utf32Char >> 16) & 0xFF;
        utf32Bytes[3] = (utf32Char >> 24) & 0xFF;

        if (fwrite(utf32Bytes, 1, 4, arquivo_saida) != 4)
        {
            fprintf(stderr, "Erro ao escrever o caractere UTF-32\n");
            return -1;
        }
    }

    return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida)
{
    int goodFile = verifyFile(arquivo_entrada, arquivo_saida);

    if (goodFile != 0)
    {
        return -1;
    }

    unsigned int utf32Char;
    unsigned char BOM[4];

    if (fread(BOM, 1, 4, arquivo_entrada) != 4)
    {
        fprintf(stderr, "Erro ao ler o BOM\n");
        return -1;
    }

    int isLittleEndian = 0;
    if (BOM[0] == 0xFF && BOM[1] == 0xFE && BOM[2] == 0x00 && BOM[3] == 0x00)
    {
        isLittleEndian = 1;
    }
    else if (BOM[0] == 0x00 && BOM[1] == 0x00 && BOM[2] == 0xFE && BOM[3] == 0xFF)
    {
        isLittleEndian = 0;
    }
    else
    {
        fprintf(stderr, "BOM inválido\n");
        return -1;
    }

    unsigned char bytes[4];
    while (fread(bytes, 1, 4, arquivo_entrada) == 4)
    {
        if (isLittleEndian)
        {
            utf32Char = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
        }
        else
        {
            utf32Char = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
        }

        if (utf32Char <= 0x7F)
        {
            fputc(utf32Char, arquivo_saida);
        }
        else if (utf32Char <= 0x7FF)
        {
            fputc(0xC0 | (utf32Char >> 6), arquivo_saida);
            fputc(0x80 | (utf32Char & 0x3F), arquivo_saida);
        }
        else if (utf32Char <= 0xFFFF)
        {
            fputc(0xE0 | (utf32Char >> 12), arquivo_saida);
            fputc(0x80 | ((utf32Char >> 6) & 0x3F), arquivo_saida);
            fputc(0x80 | (utf32Char & 0x3F), arquivo_saida);
        }
        else if (utf32Char <= 0x10FFFF)
        {
            fputc(0xF0 | (utf32Char >> 18), arquivo_saida);
            fputc(0x80 | ((utf32Char >> 12) & 0x3F), arquivo_saida);
            fputc(0x80 | ((utf32Char >> 6) & 0x3F), arquivo_saida);
            fputc(0x80 | (utf32Char & 0x3F), arquivo_saida);
        }
    }

    return 0;
}

int verifyFile(FILE *arquivo_entrada, FILE *arquivo_saida)
{
    if (arquivo_entrada == NULL)
    {
        fprintf(stderr, "Erro ao tentar abrir o arquivo de entrada\n");
        return -1;
    }

    if (arquivo_saida == NULL)
    {
        fprintf(stderr, "Erro ao criar o arquivo de saída\n");
        return -1;
    }

    return 0;
}
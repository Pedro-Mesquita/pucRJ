#include "converteutf832.h"

int main(void)
{
    // Testando a conversão de UTF-8 para UTF-32
    FILE *arquivo_entrada_utf8 = fopen("utf8_peq.txt", "r");
    FILE *arquivo_saida_utf32 = fopen("convUtf8p32Output.txt", "w");

    int result_utf8_to_utf32 = convUtf8p32(arquivo_entrada_utf8, arquivo_saida_utf32);

    fclose(arquivo_entrada_utf8);
    fclose(arquivo_saida_utf32);

    if (result_utf8_to_utf32 == -1)
    {
        printf("Deu ruim na conversão de UTF-8 para UTF-32\n");
    }
    else
    {
        printf("Deu bom na conversão de UTF-8 para UTF-32\n");
    }

    // Testando a conversão de UTF-32 para UTF-8
    FILE *arquivo_entrada_utf32 = fopen("utf32_peq.txt", "r");
    FILE *arquivo_saida_utf8 = fopen("convUtf32p8Output.txt", "w");

    int result_utf32_to_utf8 = convUtf32p8(arquivo_entrada_utf32, arquivo_saida_utf8);

    fclose(arquivo_entrada_utf32);
    fclose(arquivo_saida_utf8);

    if (result_utf32_to_utf8 == -1)
    {
        printf("Deu ruim na conversão de UTF-32 para UTF-8\n");
    }
    else
    {
        printf("Deu bom na conversão de UTF-32 para UTF-8\n");
    }

    return 0;
}
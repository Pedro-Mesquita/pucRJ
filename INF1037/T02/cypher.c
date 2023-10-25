#include <ctype.h>
#include <string.h>
#include "cypher.h"

void cifra(char *texto, char *chave, int posicao, int cifrar)
{
    if (*texto)
    {
        if (isalpha(*texto))
        {
            char upLow = (isupper(*texto) ? 'A' : 'a');
            int deslocamento = toupper(*(chave + posicao)) - 'A';

            if (cifrar == 0)
            {
                deslocamento = 26 - deslocamento;
            }

            *texto = (*texto - upLow + deslocamento) % 26 + upLow;
            posicao = (posicao + 1) % strlen(chave);
        }

        cifra(texto + 1, chave, posicao, cifrar);
    }
}

ate9 = ['um', 'dois','tres', 'quatro', 'cinco', 'seis', 'sete', 'oito', 'nove']
ate19 = ['dez', 'onze', 'doze', 'treze', 'quatorze', 'quinze', 'dezesseis', 'dezessete', 'dezoito', 'dezenovo']
dezenas = ['vinte', 'trinta', 'quarenta', 'cinquenta', 'sessenta', 'setenta', 'oitenta','noventa']
centena = ['cem', 'duzentos', 'trezentos', 'quatrocentos', 'quinhentos', 'seiscentos','setecentos','oitocentos','novecentos']

def transforma_extenso(numero):
    if numero < 10:
        print(ate9[numero-1])
        return
    if numero < 20:
        print(ate19[numero%10])
        return 
    if numero < 100:
        print(dezenas[(numero//10)-2])
        return transforma_extenso(numero%10)
    if numero < 1000:
        print(centena[(numero//100)-1])
        return transforma_extenso(numero%10)
transforma_extenso(100)


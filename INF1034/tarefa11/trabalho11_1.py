ate9 = ['um', 'dois','tres', 'quatro', 'cinco', 'seis', 'sete', 'oito', 'nove']
ate19 = ['dez', 'onze', 'doze', 'treze', 'quatorze', 'quinze', 'dezesseis', 'dezessete', 'dezoito', 'dezenove']
dezenas = ['vinte', 'trinta', 'quarenta', 'cinquenta', 'sessenta', 'setenta', 'oitenta','noventa']
centena = ['cento', 'duzentos', 'trezentos', 'quatrocentos', 'quinhentos', 'seiscentos','setecentos','oitocentos','novecentos']

def transforma_extenso(numero):
    if numero < 10:
        if numero != 0:
            return ate9[numero-1]
        return ''
    if numero < 20:
        return ate19[numero%10]
    if numero == 100:
        return 'cem'
    if numero < 100:
        if numero%10 !=0:
            return dezenas[(numero//10)-2] + ' e ' + transforma_extenso(numero%10)
        return dezenas[(numero//10)-2]
    if numero < 1000:
        if numero//10%10 != 0:
            return centena[(numero//100)-1] +' e '+ transforma_extenso(numero%100)
        return  centena[(numero//100)-1] +' e '+ transforma_extenso(numero%10)
    
switch = True

while switch:
    numero = int(input("Entre com um númunero inteiro de 1 a 999: "))
    print('Número em extenso: %s'%(transforma_extenso(numero)))
    resposta = input("Deseja pedir um novo valor por extenso? (sim/não)")
    if resposta != 'sim':
        switch = False
        
    

capital_inicial = float(input("Capital inicial: "))
taxa = 5/100
meses = int(input("Meses: "))
capital_final = capital_inicial * (1+taxa)**meses
print('Para o capital inicial de R$%.2f, com taxa de %d%% ao mês, após %d meses, teremos o capital final de R$%.2f' % (capital_inicial, taxa*100, meses, capital_final))
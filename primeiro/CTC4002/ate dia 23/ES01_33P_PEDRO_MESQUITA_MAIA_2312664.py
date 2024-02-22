nlatas = int(input("Número de latas: "))
capacidade = int(input("Capacidade de latas no engradado: "))
valorEng = float(input("Valor do engradado: "))

numEngCheios = nlatas//capacidade
sobraLatas = nLatas%capacidade
valTotal = numEngCheios*valorEng

print("Total de engradados cheios: %d. Total de latas restantes: %d. Valor total dos engradados: %d"%(numEngCheios, sobraLatas, valTotal))
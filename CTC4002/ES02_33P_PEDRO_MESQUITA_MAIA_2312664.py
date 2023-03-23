#ES02_33P,PEDRO_MESQUITA_MAIA_2312664.py
nome_cadidato = input("Digite o seu nome: ")
nota_prova = float(input("Digite a nota da sua prova: "))
nota_curriculo = float(input("Digite a nota do seu currículo: "))
nota_entrevista = float(input("Digite a nota da sua entrevista: "))
nota_cadidato = ((nota_prova*3)+(nota_curriculo*2)+nota_entrevista)/6
print('%s, sua nota é %d'%(nome_cadidato, nota_cadidato))

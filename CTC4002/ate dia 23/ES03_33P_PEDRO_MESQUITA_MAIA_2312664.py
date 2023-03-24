meses = ["janeiro", "fevereiro", "março", "abril", "maio", "junho", "julho", "agosto", "setembro", "outubro", "novembro", "dezembro"]
nome = input("Digite o seu nome: ")
data_de_nascimento = input("Digite a sua data de nascimento no formtato dd/mm/aa: ")
mes_de_nascimento = meses[int(data_de_nascimento[3:5])-1]
print('%s nasceu em %s' %(nome, mes_de_nascimento))
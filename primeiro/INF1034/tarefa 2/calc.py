def jusroscompostos(periodo_em_meses, aporte_regular):
    total = 0
    for x in range(periodo_em_meses):
        total = ((total + aporte_regular)*0.01) + (total + aporte_regular)
    
    print(total)
    
jusroscompostos(12, 1000)

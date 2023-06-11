def alg_euclides_estendido(a, b):
    if a == 0:
        return b, 0, 1
    else:
        mdc, x, y = alg_euclides_estendido(b % a, a)
        return mdc, y - (b // a) * x, x

def co_primo(t):
    for i in range(2,t):
        if alg_euclides_estendido(i, t)[0] == 1:
            lista_primos.append(i)
    return lista_primos

lista_primos = []

print(co_primo(30))
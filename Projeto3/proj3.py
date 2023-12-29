from pulp import LpMaximize, LpProblem, LpVariable, value, PULP_CBC_CMD

brinquedos = []
pacotes_especiais = []

variaveis_brinquedos = []
variaveis_pacotes = []
funcao_objetivo = 0
restricao_1 = 0

# Criação do problema
problema = LpProblem(name="Maximizar_Lucro", sense=LpMaximize)


# Input
n, p, max_brinquedos = [int(i) for i in input().split()]

# Brinquedos
for i in range(n):
    valor, capacidade_producao = [int(i) for i in input().split()]

    # Dicionário para representar o brinquedo
    brinquedo = {
        'valor': valor,
        'capacidade_producao': capacidade_producao,
    }

    aux = LpVariable(name=f"brinquedo_{i}", lowBound=0)
    variaveis_brinquedos.append(aux)
    brinquedos.append(brinquedo)
    funcao_objetivo += (aux * brinquedo['valor'])
    restricao_1 += aux

# Pacotes
for j in range(p):
    x, y, z, lucro = [int(i) for i in input().split()]

    # Dicionário para representar o pacote especial
    pacote_especial = {
        'brinquedos': [x - 1, y - 1, z - 1],
        'lucro': int(lucro)
    }

    aux = LpVariable(name=f"pacote_{j}", lowBound=0)
    variaveis_pacotes.append(aux)
    pacotes_especiais.append(pacote_especial)
    funcao_objetivo += (aux * pacote_especial['lucro'])
    restricao_1 += (aux*3)

# Adição da função objetivo
problema += (funcao_objetivo, "Funcao_Objetivo")

# Adição das restrições
problema += restricao_1 <= max_brinquedos, "Restricao_1"

for i, brinquedo in enumerate(brinquedos):
    restricoes_brinquedo = []

    for j, pacote in enumerate(pacotes_especiais):
        if i in pacote['brinquedos']:
            restricoes_brinquedo.append(variaveis_pacotes[j])

    if restricoes_brinquedo:
        problema += (sum(restricoes_brinquedo) + variaveis_brinquedos[i] <= brinquedo['capacidade_producao'],
                     f"Restricao_{i + 2}")
    else:
        problema += variaveis_brinquedos[i] <= brinquedo['capacidade_producao'], f"Restricao_{i + 2}"

# Resolução do problema
problema.solve(PULP_CBC_CMD(msg=False))

# Exibição dos resultados
print(f"{int(value(problema.objective))}")
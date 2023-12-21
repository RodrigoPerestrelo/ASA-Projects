from pulp import LpMaximize, LpProblem, LpVariable

flag = 1

# Input
with open("input.txt", "r") as ficheiro:
    # Trata primeira linha
    n, p, max_brinquedos = map(int, ficheiro.readline().split())

    # Lista de brinquedos
    brinquedos = []

    # Detalhes para cada brinquedo e adicionar o mesmo à lista
    for i in range(n):
        valor, capacidade_producao = map(int, ficheiro.readline().split())

        # Dicionário para representar o brinquedo
        brinquedo = {
            'valor': valor,
            'capacidade_producao': capacidade_producao
        }

        # Adiciona o dicionário à lista de brinquedos
        brinquedos.append(brinquedo)

    # Lista de pacotes especiais
    pacotes_especiais = []

    # Detalhes para cada pacote especial e adicionar o mesmo à lista
    for j in range(p):
        x, y, z, lucro = map(int, ficheiro.readline().split())

        # Dicionário para representar o pacote especial
        pacote_especial = {
            'brinquedos': [x-1, y-1, z-1],
            'lucro': int(lucro)
        }

        # Adiciona o dicionário à lista de pacotes especiais
        pacotes_especiais.append(pacote_especial)

ficheiro.close()


# Criação do problema
problema = LpProblem(name="Maximizar_Lucro", sense=LpMaximize)

# Criação das variáveis de decisão
variaveis_brinquedos = [LpVariable(name=f"brinquedo_{i}", lowBound=0) for i in range(len(brinquedos))]
variaveis_pacotes = [LpVariable(name=f"pacote_{i}", lowBound=0) for i in range(len(pacotes_especiais))]


"""
# Versao Anterior
a = LpVariable(name="a", lowBound=0)  # x >= 0
b = LpVariable(name="b", lowBound=0)
c = LpVariable(name="c", lowBound=0)
d = LpVariable(name="d", lowBound=0)
e = LpVariable(name="e", lowBound=0)

f = LpVariable(name="f", lowBound=0)
g = LpVariable(name="g", lowBound=0)
"""

# Adição da função objetivo
problema += (sum(variavel * brinquedo['valor'] for variavel, brinquedo in zip(variaveis_brinquedos, brinquedos)) +
             sum(variavel * pacote['lucro'] for variavel, pacote in zip(variaveis_pacotes, pacotes_especiais)),
             "Funcao_Objetivo")

"""
# Versao Anterior
problema += (a * brinquedos[0]['valor'] + b * brinquedos[1]['valor'] + c * brinquedos[2]['valor'] +
             d * brinquedos[3]['valor'] + e * brinquedos[4]['valor'] + f * pacotes_especiais[0]['lucro'] +
             g * pacotes_especiais[1]['lucro'], "Funcao_Objetivo")
"""

# Adição das restrições
problema += sum(variaveis_brinquedos) + sum(variaveis_pacotes) * 3 <= max_brinquedos, "Restricao_1"

# Está MALLLLLL :(
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


"""
# Versao Anterior
problema += a + b + c + d + e + f*3 + g*3 <= max_brinquedos, "Restricao_1"
problema += a + f <= brinquedos[0]['capacidade_producao'], "Restricao_2"
problema += b + g <= brinquedos[1]['capacidade_producao'], "Restricao_3"
problema += c + f + g <= brinquedos[2]['capacidade_producao'], "Restricao_4"
problema += d + g <= brinquedos[3]['capacidade_producao'], "Restricao_5"
problema += e + f <= brinquedos[4]['capacidade_producao'], "Restricao_6"
"""

# Resolve o problema
problema.solve()


"""
# Mais prints de teste

print(f"Valor de brinquedos diferentes: {n}")
print(f"Valor de pacotes especiais: {p}")
print(f"Valor de brinquedos máximo: {max_brinquedos}")
print("\n")

for brinquedo in brinquedos:
    print(f"Valor: R${brinquedo['valor']:.2f}")
    print(f"Capacidade de Produção: {brinquedo['capacidade_producao']} unidades por dia\n")
for pacote_especial in pacotes_especiais:
    print("Pacote Especial:")
    print(f"Produto 1: {pacote_especial['brinquedos']}")
    print(f"Lucro: R${pacote_especial['lucro']:.2f}\n")
"""

# Exibindo os resultados
print(f"Status: {problema.status}")
print(f"Lucro Máximo: {problema.objective.value()}")

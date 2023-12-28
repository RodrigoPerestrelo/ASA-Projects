from pulp import LpMaximize, LpProblem, LpVariable

flag = 1

# Input
p_linha = input()
n, p, max_brinquedos = map(int, p_linha.split())

# Lista de brinquedos
brinquedos = []

# Detalhes para cada brinquedo e adicionar o mesmo à lista
for i in range(n):
    entrada_brinquedo = input()
    valor, capacidade_producao = map(int, entrada_brinquedo.split())

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
    entrada_pacote = input()
    x, y, z, lucro = map(int, entrada_pacote.split())

    # Dicionário para representar o pacote especial
    pacote_especial = {
        'brinquedos': [x - 1, y - 1, z - 1],
        'lucro': int(lucro)
    }

    # Adiciona o dicionário à lista de pacotes especiais
    pacotes_especiais.append(pacote_especial)

# Criação do problema
problema = LpProblem(name="Maximizar_Lucro", sense=LpMaximize)

# Criação das variáveis de decisão
variaveis_brinquedos = [LpVariable(name=f"brinquedo_{i}", lowBound=0) for i in range(len(brinquedos))]
variaveis_pacotes = [LpVariable(name=f"pacote_{i}", lowBound=0) for i in range(len(pacotes_especiais))]

# Adição da função objetivo
problema += (sum(variavel * brinquedo['valor'] for variavel, brinquedo in zip(variaveis_brinquedos, brinquedos)) +
             sum(variavel * pacote['lucro'] for variavel, pacote in zip(variaveis_pacotes, pacotes_especiais)),
             "Funcao_Objetivo")

# Adição das restrições
problema += sum(variaveis_brinquedos) + sum(variaveis_pacotes) * 3 <= max_brinquedos, "Restricao_1"

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

# Resolve o problema
problema.solve()

# Exibindo os resultados
print(f"{problema.objective.value()}")

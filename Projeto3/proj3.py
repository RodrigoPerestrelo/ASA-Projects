from pulp import LpMaximize, LpProblem, LpVariable, value, GLPK, LpInteger, lpSum

# Criação do problema
problema = LpProblem(name="Maximizar_Lucro", sense=LpMaximize)

# Input
n, p, max_brinquedos = map(int, input().split())

brinquedos = [None] * n
restriction_1 = [None] * n
objective_function = [None] * n
capacidade_producao_lista =  [None] * n
vetor_pacotes = [None] * n

# Brinquedos
for i in range(n):
    valor, capacidade_producao = map(int, input().split())
    aux = LpVariable(name=f"brinquedo_{i}", lowBound=0, upBound=capacidade_producao, cat=LpInteger)

    # Dicionário para representar o brinquedo
    capacidade_producao_lista[i] = capacidade_producao
    brinquedos[i] = aux
    objective_function[i] = aux * valor
    restriction_1[i] = aux

# Pacotes
for j in range(p):
    x, y, z, lucro = map(int, input().split())
    b1 = capacidade_producao_lista[x-1]
    b2 = capacidade_producao_lista[y-1]      
    b3 = capacidade_producao_lista[z-1]      
    up = min(b1, b2, b3)
    aux = LpVariable(name=f"pacote_{j}", lowBound=0, upBound=up, cat=LpInteger)

    if vetor_pacotes[x-1] != None:
        vetor_pacotes[x-1].append(aux)
    else:
        vetor_pacotes[x-1] = [aux]
    if vetor_pacotes[y-1] != None:
        vetor_pacotes[y-1].append(aux)
    else:
        vetor_pacotes[y-1] = [aux]
    if vetor_pacotes[z-1] != None:
        vetor_pacotes[z-1].append(aux)
    else:
        vetor_pacotes[z-1] = [aux]

    objective_function[j] += (aux * lucro)
    restriction_1[j] += aux*3

# Adição da função objetivo
problema += lpSum(objective_function)

# Adição das restrições
problema += lpSum(restriction_1) <= max_brinquedos, "Restricao_1"

for i in range(n):
    if(vetor_pacotes[i] != None):
        problema += lpSum(vetor_pacotes[i]) + brinquedos[i] <= capacidade_producao_lista[i], f"Restricao_{i + 2}"

# Resolução do problema
problema.solve(GLPK(msg=0))

# Exibição dos resultados
print(int(value(problema.objective)))
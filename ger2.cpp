#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Grafo {
    int vertices;
    vector<vector<int> > adj;

public:
    Grafo(int v) : vertices(v), adj(v) {}

    void adicionarAresta(int arestas) {
    for (int i = 0; i < arestas; i++) {
        int u, v;
        printf("Digite a aresta %d: ", i + 1);
        scanf("%d %d", &u, &v);

        // Verifica se os índices estão dentro dos limites
        if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
            adj[u].push_back(v);
        } else {
            printf("Índices inválidos!\n");
            i--;  // Para compensar a entrada inválida
        }
    }
}

    void dfsIterativa(int inicio, vector<int>& temposFim) {
        vector<bool> visitado(vertices, false);
        stack<pair<int, int>> pilha;  // Usamos um par (vértice, tempo de fim)

        for (int i = 0; i < vertices; ++i) {
            if (!visitado[i]) {
                pilha.push({i, 0});

                while (!pilha.empty()) {
                    int atual = pilha.top().first;
                    int& tempoFim = pilha.top().second;

                    if (tempoFim == 0) {
                        // Primeira vez que visitamos o vértice
                        printf("Visitando vértice 1ª vez %d\n", atual);
                        visitado[atual] = true;
                    }

                    bool explorado = true;

                    for (int vizinho : adj[atual]) {
                        if (!visitado[vizinho]) {
                            pilha.push({vizinho, 0});
                            explorado = false;
                            break;
                        }
                    }

                    if (explorado) {
                        // Vértice totalmente explorado, adiciona o tempo de fim
                        temposFim.push_back(atual);
                        pilha.pop();
                    } else {
                        // Incrementa o tempo de fim para a próxima iteração
                        ++tempoFim;
                    }
                }
            }
        }
    }


    void segundaDFS(int inicio, vector<bool>& visitado, vector<int>& arestasPorComponente, int& arestasNoCaminho) {
        // falta um if algures que eu n estou a comoreender onde pq sou nabo
        stack<int> pilha;
        pilha.push(inicio);

        while (!pilha.empty()) {
            int v = pilha.top();
            pilha.pop();

            if (!visitado[v]) {
                visitado[v] = true;

                for (int vizinho : adj[v]) {
                    // Verifica se a aresta é direcionada para o vizinho
                    if (!visitado[vizinho]) {
                        pilha.push(vizinho);

                        // Incrementa o número de arestas no componente atual
                        arestasPorComponente[v] += arestasPorComponente[v];
                    }
                }

                // Atualiza o número total de arestas no caminho
                arestasNoCaminho = max(arestasNoCaminho, arestasPorComponente[v] + int(adj[v].size()));
            }
        }
    }



    int maiorCaminho() {
        // Realiza a primeira DFS para obter os tempos de fim
        vector<int> temposFim;
        dfsIterativa(0, temposFim);

        // Inicializa a segunda DFS
        vector<bool> visitado(vertices, false);
        vector<int> arestasPorComponente(vertices, 0);
        int arestasNoCaminho = 0;

        // Realiza a segunda DFS em ordem decrescente de tempos de fim
        for (int i = temposFim.size() - 1; i >= 0; --i) {
            int componente = temposFim[i];
            if (!visitado[componente]) {
                segundaDFS(componente, visitado, arestasPorComponente, arestasNoCaminho);
            }
        }

        return arestasNoCaminho;
    }
};

int main() {
    int num_arestas, num_vertices;

    printf("Digite o número de vértices: ");
    scanf("%d", &num_vertices);
    Grafo grafo(num_vertices);

    printf("Digite o número de arestas: ");
    scanf("%d", &num_arestas);

    grafo.adicionarAresta(num_arestas);

    int resultado = grafo.maiorCaminho();


    vector<int> temposFim;
    grafo.dfsIterativa(0, temposFim);
    printf("Tempos de fim em ordem crescente: ");
    for (int tempo : temposFim) {
        printf("%d ", tempo);
    }
    printf("\n");

    printf("Número de arestas no maior caminho: %d\n", resultado);

    return 0;
}
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Grafo {
    int vertices;
    vector<vector<int>> adj;

public:
    Grafo(int v) : vertices(v), adj(v) {}

    void adicionarAresta(int u, int v) {
        adj[u].push_back(v);
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
                        cout << "Visitando vértice " << atual << endl;
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
};

int main() {
    Grafo grafo(6);
    
    grafo.adicionarAresta(0, 1);
    grafo.adicionarAresta(0, 2);
    grafo.adicionarAresta(1, 3);
    grafo.adicionarAresta(2, 4);
    grafo.adicionarAresta(3, 5);
    
    vector<int> temposFim;
    grafo.dfsIterativa(0, temposFim);
    
    cout << "Tempos de fim em ordem crescente: ";
    for (int tempo : temposFim) {
        cout << tempo << " ";
    }
    cout << endl;

    return 0;
}

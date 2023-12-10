#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

using namespace std;

class Grafo {
public:
    int vertices;
    vector<vector<int>> listaAdj;

    Grafo(int V) : vertices(V), listaAdj(V) {}

    void adicionarAresta(int origem, int destino) {
        listaAdj[origem].push_back(destino);
    }

    /* Visita os vértices da lista de adjacências do vertice dado como arg */
    void preencherOrdem(int vertice, vector<bool>& visitados, stack<int>& pilha) {
        visitados[vertice] = true;

        for (int vizinho : listaAdj[vertice]) {
            if (!visitados[vizinho]) {
                preencherOrdem(vizinho, visitados, pilha);
            }
        }

        pilha.push(vertice);
    }

    /* Faz a DFS no vertice dado */
    void DFS(int vertice, vector<bool>& visitados) {
        visitados[vertice] = true;
        cout << vertice << " ";

        for (int vizinho : listaAdj[vertice]) {
            if (!visitados[vizinho]) {
                DFS(vizinho, visitados);
            }
        }
    }

    void imprimirComponentesFortementeConectados() {
        stack<int> pilha;
        vector<bool> visitados(vertices, false);

        /* Usado para preencher a pilha, depois deste loop a pilha terá 
        no topo os vertices com tempo de fim maior */
        for (int i = 0; i < vertices; ++i) {
            if (!visitados[i]) {
                preencherOrdem(i, visitados, pilha);
            }
        }

        /* Obtem o grafo transposto para fazer a DFS através da fila da pilha */
        Grafo grafoTransposto = obterGrafoTransposto();

        /* Reinicia todos os valores a false */
        fill(visitados.begin(), visitados.end(), false);

        /* Faz a DFS pelos elementos da pilha, ou seja, por ordem de maior tempo de fim */
        while (!pilha.empty()) {
            int vertice = pilha.top();
            pilha.pop();

            if (!visitados[vertice]) {
                grafoTransposto.DFS(vertice, visitados);
                cout <<endl;
            }
        }
    }

    Grafo BuildGscc(const Grafo& G, const vector<unordered_set<int>>& C1) {
        Grafo Gscc(C1.size());
        int C = C1.size();
        vector<int> A(C, 0);

        for (const auto& C_scc : C1) {
            for (int u : C_scc) {
                for (int v : G.listaAdj[u]) {
                    if (C_scc.count(v) == 0 && A[v] == 0) {
                        Gscc.adicionarAresta(u, v);
                        A[v] = 1;
                    }
                }
            }

            for (int u : C_scc) {
                for (int v : Gscc.listaAdj[u]) {
                    A[v] = 0;
                }
            }
        }

        return Gscc;
    }

private:
    Grafo obterGrafoTransposto() {
        Grafo grafoTransposto(vertices);

        for (int i = 0; i < vertices; ++i) {
            for (int vizinho : listaAdj[i]) {
                grafoTransposto.adicionarAresta(vizinho, i);
            }
        }

        return grafoTransposto;
    }
};

int main() {
    Grafo grafo(5);
    Grafo grafo2(3);

    grafo.adicionarAresta(0, 2);
    grafo.adicionarAresta(2, 1);
    grafo.adicionarAresta(1, 0);
    grafo.adicionarAresta(0, 3);
    grafo.adicionarAresta(3, 4);

    cout << "Componentes Fortemente Conectados:" << endl;
    grafo.imprimirComponentesFortementeConectados();

    vector<int> primeiro = {0,1,2};
    vector<int> segundo = {3};
    vector<int> terceiro = {4};
    
    vector<unordered_set<int>> SCCs = {primeiro, segundo, terceiro};
    grafo2 = grafo.BuildGscc(grafo, SCCs);

    return 0;
}

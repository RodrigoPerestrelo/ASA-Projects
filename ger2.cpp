#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Grafo {
public:
    int vertices;
    vector<vector<int>> listaAdj;

    Grafo(int V) : vertices(V), listaAdj(V) {}

    void adicionarAresta(int origem, int destino) {
        listaAdj[origem].push_back(destino);
    }

     void imprimirGrafo() {
        for (int i = 0; i < vertices; ++i) {
            cout << "Vértice " << i << ": ";
            for (int j = 0; j < listaAdj[i].size(); ++j) {
                cout << listaAdj[i][j];
                if (j < listaAdj[i].size() - 1) {
                    cout << " -> ";
                }
            }
            cout << endl;
        }
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

    void DFS(int vertice, vector<bool>& visitados, int index, vector<int>& SCC_Indexs) {
        SCC_Indexs[vertice] = index;
        visitados[vertice] = true;
        cout << vertice << " ";

        
        for (int vizinho : listaAdj[vertice]) {
            if (!visitados[vizinho]) {
                DFS(vizinho, visitados, index, SCC_Indexs);
            }
        }
    }

    void imprimirComponentesFortementeConectados(vector<int>& SCC_Indexs) {
        stack<int> pilha;
        vector<bool> visitados(vertices, false);
        int index = 0;

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
                // É AQUI, fazes pushback num vetor de vetores:
                // vector<vector<int>>
                //o de dentro: vector<int> é feito na DFS, o de fora é dar pushback do que foi obtido apartir da DFS
                grafoTransposto.DFS(vertice, visitados, index, SCC_Indexs);
                index++;
                cout <<endl;
            }
        }
    }

    Grafo BuildGscc(const Grafo& GivenGraph, const vector<vector<int>>& SCCs_Vector, vector<int> SCC_Indexs) {
        Grafo SCC_Graph(SCCs_Vector.size());
        int SCC_Graph_Size = SCCs_Vector.size();
        vector<int> A(SCC_Graph_Size, 0);

        for (const vector<int>& SCC : SCCs_Vector) {
            for (int vertice : SCC) {
                for (int vizinho : GivenGraph.listaAdj[vertice]) {
                    printf("-%d %d-\n", vertice, vizinho);
                    if ((SCC_Indexs[vertice] != SCC_Indexs[vizinho]) && A[SCC_Indexs[vizinho]] == 0) {
                        SCC_Graph.adicionarAresta(SCC_Indexs[vertice], SCC_Indexs[vizinho]);
                        A[SCC_Indexs[vizinho]] = 1;
                    }
                }

                for (int vertice : SCC) {
                    for (int vizinho : GivenGraph.listaAdj[vertice]) {
                        A[SCC_Indexs[vizinho]] = 0;
                    }
                }
            }
        }

    return SCC_Graph;
    }

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

    grafo.adicionarAresta(0, 2);
    grafo.adicionarAresta(2, 1);
    grafo.adicionarAresta(1, 0);
    grafo.adicionarAresta(0, 3);
    grafo.adicionarAresta(3, 4);
    
    vector<int> SCC_Indexs = vector<int>(5, 0);
    grafo.imprimirComponentesFortementeConectados(SCC_Indexs);


    //FAZER: TENTAR METER A FUNÇÃO IMPRIMIRCOMPONENTESFORTEMENTECONECTADOS FAZER OS VETORES QUE ESTÃO ABAIXO
    //MANDAR UM vector<vector<int>> e fazer com que essa função altere de acordo com o que for preciso
    //TEM COMENTÁRIOS NO SITIO QUE É PRA FAZER
    vector<int> primeiro = {0,1,2};
    vector<int> segundo = {3};
    vector<int> terceiro = {4};
    vector<vector<int>> SCCs = {primeiro, segundo, terceiro};

    // Isto tá a funcionar já
    // Falta fazer com que aqueles prints dos SCCs passem a ser pushbacks num vetor de SCCs
    // para fazer estes vetores que eu meti à mão, o primeiro, segundo e o terceiro.
    Grafo grafo2 = grafo.BuildGscc(grafo, SCCs, SCC_Indexs);
    grafo2.imprimirGrafo();

    return 0;
}

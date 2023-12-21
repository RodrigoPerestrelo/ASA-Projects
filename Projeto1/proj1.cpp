#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void initializeMatrix(
    vector<vector<int>>& matrix,
    int max_lines,
    int max_columns,
    int nPieces);

int cuttingMarble(
    vector<vector<int>>& matrix,
    int max_lines,
    int max_columns
);

// Função principal
int main() {
    int max_lines, max_columns, nPieces;
    scanf("%d %d %d", &max_lines, &max_columns, &nPieces);

    vector<vector<int>> matrix(max_lines + 1, vector<int>(max_columns + 1, 0));
    
    initializeMatrix(matrix, max_lines, max_columns, nPieces);
    printf("%d\n", cuttingMarble(matrix ,max_lines, max_columns));

    return 0;
}

// Função que inicializa a matriz com os valores de input das peças
void initializeMatrix(
    vector<vector<int>>& matrix,
    int max_lines,
    int max_columns,
    int nPieces
) {
    int line, column, value;
    for (int i = 0; i < nPieces; i++) {
        scanf("%d %d %d", &line, &column, &value);
        if (line <= max_lines && column <= max_columns) {
            if (matrix[line][column] < value) matrix[line][column] = value;
        }
        if (column <= max_lines && line <= max_columns) {
            if (matrix[column][line] < value) matrix[column][line] = value;
        }
    }
}

// Função que calcula o valor máximo possível usando corte de peças 2D com programação dinâmica
int cuttingMarble(
    vector<vector<int>>& matrix,
    int max_lines,
    int max_columns
) {

    for (int i = 1; i <= max_lines; ++i) {
        for (int j = 1; j <= max_columns; ++j) {
            int maxRes = 0;
            for (int k = 1; k < i; k++) {
                maxRes = max(maxRes, matrix[i-k][j] + matrix[k][j]);
            }
            for (int k = 1; k < j; k++) {
                maxRes = max(maxRes, matrix[i][j-k] + matrix[i][k]);
            }
            maxRes = max(maxRes, matrix[i][j]);
            matrix[i][j] = maxRes;
        }
    }
    return matrix[max_lines][max_columns];
}
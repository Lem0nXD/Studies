//Метод зворотного розміщення елементів
//Distance Matrix is calculated with Floyd Warshall's function
#include <iostream>
#include <algorithm>

using namespace std;

const int MAX_VERTICES = 5;
const int INF = 1e9;

void printMatrix(int(&matrix)[MAX_VERTICES][MAX_VERTICES], string label)
{
    cout << label << ":\n";
    for (int i = 0; i < MAX_VERTICES; i++)
    {
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void floydWarshall(int (&distanceMatrix)[MAX_VERTICES][MAX_VERTICES]) 
{
    for (int k = 0; k < MAX_VERTICES; k++) 
    {
        for (int i = 0; i < MAX_VERTICES; i++) 
        {
            for (int j = 0; j < MAX_VERTICES; j++) 
            {
                if (distanceMatrix[i][k] != INF && distanceMatrix[k][j] != INF) 
                {
                    distanceMatrix[i][j] = min(distanceMatrix[i][j], distanceMatrix[i][k] + distanceMatrix[k][j]);
                }
            }
        }
    }
}

void sumRows(int (&matrix)[MAX_VERTICES][MAX_VERTICES], int(&sum_arr)[MAX_VERTICES])
{
    for (int i = 0; i < MAX_VERTICES; i++)
    {
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            sum_arr[i] = sum_arr[i] + matrix[i][j];
        }
    }
}

int sumOfMatrix(int(&matrix)[MAX_VERTICES][MAX_VERTICES])
{
    int sum = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            sum += matrix[i][j];
        }
    }
    return sum;
}

int calculateEfficiency(int(&matrix1)[MAX_VERTICES][MAX_VERTICES], int(&matrix2)[MAX_VERTICES][MAX_VERTICES])
{
    short int answ = 0;
    int tempMatrix[MAX_VERTICES][MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++)
    {
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            tempMatrix[i][j] = matrix1[i][j] * matrix2[i][j];
        }
    }
    answ = (sumOfMatrix(tempMatrix)) / 2;
    printMatrix(tempMatrix, "Adjacency x Distance Matrix");
    return answ;
}

int main() 
{
    int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES] = { 0 };
    int distanceMatrix[MAX_VERTICES][MAX_VERTICES];
    int newDistanceMatrix[MAX_VERTICES][MAX_VERTICES];
    int adjacencyRowSumArr[MAX_VERTICES] = { 0 };
    int distanceRowSumArr[MAX_VERTICES] = { 0 };
    int productMatrix[MAX_VERTICES][MAX_VERTICES];
    int newVerticesPositionArr[MAX_VERTICES] = { 0 };
    double answ1, answ2, E;

    int u, v, w;

    int w_arr[4] = { 0 };

    cout << "Enter u,v (adjacencyMatrix):\n";
    for (int i = 0; i < MAX_VERTICES; i++) 
    {
        cin >> u >> v;
        if (u <= 0 || v <= 0 || u > MAX_VERTICES || v > MAX_VERTICES) 
        {
            cout << "Invalid input, out of bounds!" << endl;
            continue;
        }
        adjacencyMatrix[u - 1][v - 1]++;
        adjacencyMatrix[v - 1][u - 1]++;
    }

    for (int i = 0; i < MAX_VERTICES; i++)
    {
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            distanceMatrix[i][j] = (i == j) ? 0 : INF;
        }
    }

    cout << "Enter u,v,w (distanceMatrix):\n";
    for (int i = 0; i < 4; i++) 
    {
        cin >> u >> v >> w;
        if (u <= 0 || v <= 0 || u > MAX_VERTICES || v > MAX_VERTICES) 
        {
            cout << "Invalid input, out of bounds!" << endl;
            continue;
        }
        distanceMatrix[u - 1][v - 1] = distanceMatrix[v - 1][u - 1] = w;
        w_arr[i] = w;
    }

    floydWarshall(distanceMatrix);

    printMatrix(adjacencyMatrix, "Adjacency matrix");

    /*
    1 2
    2 3
    2 3
    1 5
    4 5
    */

    printMatrix(distanceMatrix, "Distance matrix");

    /*
    1 2 1
    2 3 1
    3 4 1
    4 5 2
    */

    sumRows(adjacencyMatrix, adjacencyRowSumArr);
    sumRows(distanceMatrix, distanceRowSumArr);

    cout << "Adjacency Row Sum Arr: ";
    for (int x : adjacencyRowSumArr)
        cout << x << " ";
    cout << endl;

    cout << "Distance Row Sum Arr: ";
    for (int x : distanceRowSumArr)
        cout << x << " ";
    cout << endl;

    answ1 = calculateEfficiency(adjacencyMatrix, distanceMatrix);

    for (int i = 0; i < MAX_VERTICES; i++)
    {
        int max_A = 0;
        int min_D = 0;
        max_A = distance(adjacencyRowSumArr, max_element(adjacencyRowSumArr, adjacencyRowSumArr + MAX_VERTICES));
        min_D = distance(distanceRowSumArr, min_element(distanceRowSumArr, distanceRowSumArr + MAX_VERTICES));
        newVerticesPositionArr[min_D] = max_A + 1;
        adjacencyRowSumArr[max_A] = 0;
        distanceRowSumArr[min_D] = 99;
    }

    cout << "New Vertices Positions: ";
    for (int x : newVerticesPositionArr)
        cout << x << " ";
    cout << endl;

    for (int i = 0; i < MAX_VERTICES; i++)
    {
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            newDistanceMatrix[i][j] = (i == j) ? 0 : INF;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        int u = newVerticesPositionArr[i];
        int v = newVerticesPositionArr[i + 1];
        newDistanceMatrix[u - 1][v - 1] = newDistanceMatrix[v - 1][u - 1] = w_arr[i];
    }

    floydWarshall(newDistanceMatrix);

    printMatrix(newDistanceMatrix, "New distance matrix");

    answ2 = calculateEfficiency(adjacencyMatrix, newDistanceMatrix);

    cout << "Before reverse placement of elements: " << answ1 << endl;
    cout << "After reverse placement of elements: " << answ2 << endl;

    E = ((answ1 - answ2) / answ1) * 100;
    cout << "E = " << E << "%";
    return 0;
}

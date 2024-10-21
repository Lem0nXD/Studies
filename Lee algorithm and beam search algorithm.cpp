#include <iostream>
#include <map>
#include <unordered_map>
#include <queue>

const int w = 9, h = 11, layerIndex = 2;
const int MAX_VERTICES = 7;
void initializePositionMatrix(char positionMatrix[layerIndex][w][h], int layerIndex);
void inputPositionMatrix(char positionMatrix[layerIndex][w][h], std::map<char, std::pair<int, int>>& elementCoordinates);
void inputAdjacencyMatrix(int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::unordered_map<char, int>& elementMap);
void printPositionMatrix(char positionMatrix[layerIndex][w][h], int layerIndex);
void printAdjacencyMatrix(int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::unordered_map<char, int>& elementMap);
char findMaxConnectedElement(int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::unordered_map<char, int>& elementMap);

int main()
{

    char positionMatrix[layerIndex][w][h];
    int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES] = 
    {
        //0
        {0,0,0,0,0,1,1},
        {0,0,2,0,1,0,1},
        {0,2,0,0,1,0,0},
        {0,0,0,0,0,2,2},
        {0,1,1,0,0,2,0},
        {1,0,0,2,2,0,0},
        {1,1,0,2,0,0,0}
    };

    std::map<char, std::pair<int, int>> elementCoordinates;
    std::unordered_map<char, int> elementMap;
    std::unordered_map<char, int> unplacedElementMap;

    elementCoordinates['A'] = { 2,1 };
    elementCoordinates['B'] = { 5,1 };
    elementCoordinates['C'] = { 2,4 };
    elementCoordinates['D'] = { 7,8 };
    elementCoordinates['E'] = { 1,7 };
    elementCoordinates['F'] = { 6,4 };
    elementCoordinates['H'] = { 3,9 };

    initializePositionMatrix(positionMatrix, 0);
   /*
   A 2 1
   B 5 1
   C 2 4
   F 6 4
   E 1 7
   D 7 8
   H 3 9
   X -1 -1
   */

    //inputPositionMatrix(positionMatrix, elementCoordinates);


    int count = 0;
    for (const auto& element : elementCoordinates)
    {
        char key = element.first;
        int x = element.second.first;
        int y = element.second.second;

        positionMatrix[0][x][y] = key;
        elementMap.insert({key,count});
        count++;
    }
    
    unplacedElementMap = elementMap;
    /*
    H D 2
    H A 1
    D F 2
    F A 1
    F E 2
    E B 1
    E C 1
    C B 2
    B H 1
    X X 0
    */

    //inputAdjacencyMatrix(adjacencyMatrix, elementMap);
    
    printPositionMatrix(positionMatrix, 0);
    printAdjacencyMatrix(adjacencyMatrix, elementMap);

    char maxElement = findMaxConnectedElement(adjacencyMatrix, elementMap);

    printPositionMatrix(positionMatrix, 0);

}

void initializePositionMatrix(char positionMatrix[layerIndex][w][h], int layerIndex)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            positionMatrix[layerIndex][j][i] = '.';
        }
    }
}

void inputPositionMatrix(char positionMatrix[layerIndex][w][h], std::map<char, std::pair<int, int>>& elementCoordinates)
{
    std::cout << "Enter position matrix (elem, position_x, position_y):\n";
    std::cout << "Enter 'X -1 -1' to finish input.\n";

    while (true)
    {
        char letter;
        int width, height;
        std::cin >> letter >> width >> height;
        if (letter == 'X' && width == -1 || height == -1)
        {
            break;
        }
        else if (width < -1 || height < -1
            || width > w || height > h)
        {
            std::cout << "Invalid input, out of bounds!" << std::endl;
            continue;
        }
        elementCoordinates[letter].first = width;
        elementCoordinates[letter].second = height;
        positionMatrix[0][width][height] = letter;
    }
}

void inputAdjacencyMatrix(int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::unordered_map<char, int>& elementMap)
{
    std::cout << "Enter adjacency connections (elem1 elem2 number_of_connections):\n";
    std::cout << "Enter 'X X 0' to finish input.\n";

    while (true)
    {
        char elem1, elem2;
        int connections;
        std::cin >> elem1 >> elem2 >> connections;

        if (elem1 == 'X' && elem2 == 'X' && connections == 0)
        {
            break;
        }

        if (elementMap.find(elem1) == elementMap.end() || elementMap.find(elem2) == elementMap.end())
        {
            std::cout << "Invalid input elements!" << std::endl;
            continue;
        }

        int index1 = elementMap[elem1];
        int index2 = elementMap[elem2];

        adjacencyMatrix[index1][index2] = connections;
        adjacencyMatrix[index2][index1] = connections;
    }
}

void printPositionMatrix(char positionMatrix[layerIndex][w][h], int layerIndex)
{
    printf("Printing positionMatrix [%d]... \n", layerIndex);
    for (int j = 0; j < h; j++)
    {
        std::cout << "| ";
        for (int k = 0; k < w; k++)
        {
            std::cout << positionMatrix[layerIndex][k][j] << " ";
        }
        std::cout << "|" << std::endl;
    }
}

void printAdjacencyMatrix(int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::unordered_map<char, int>& elementMap)
{
    // Reverse the unordered_map to print the matrix with element names
    std::unordered_map<int, char> reverseMap;
    for (const auto& pair : elementMap)
    {
        reverseMap[pair.second] = pair.first;
    }

    std::cout << "Adjacency Matrix:" << std::endl;
    std::cout << "  ";
    for (int i = 0; i < MAX_VERTICES; i++)
    {
        std::cout << reverseMap[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < MAX_VERTICES; i++)
    {
        std::cout << reverseMap[i] << " ";
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            std::cout << adjacencyMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

char findMaxConnectedElement(int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::unordered_map<char, int>& elementMap)
{
    char maxElement = '\0';
    int maxConnections = -1;

    for (const auto& pair : elementMap)
    {
        int currentConnections = 0;
        int index = pair.second;

        for (int j = 0; j < MAX_VERTICES; j++)
        {
            currentConnections += adjacencyMatrix[index][j];
        }

        if (currentConnections > maxConnections)
        {
            maxConnections = currentConnections;
            maxElement = pair.first;
        }
    }

    return maxElement;
}

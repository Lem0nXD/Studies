#include <iostream>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>

const int w = 9, h = 11, layerIndex = 3;
const int MAX_VERTICES = 7;
void initializePositionMatrix(char positionMatrix[layerIndex][w][h]);
void inputPositionMatrix(char positionMatrix[layerIndex][w][h], std::map<char, std::pair<int, int>>& elementCoordinates);
void inputAdjacencyMatrix(int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::unordered_map<char, int>& elementMap);
void printPositionMatrix(char positionMatrix[layerIndex][w][h], int layerIndex);
void printAdjacencyMatrix(int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::unordered_map<char, int>& elementMap);

char findMaxConnectedElement(int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::unordered_map<char, int>& elementMap);
int leeAlgorithm(char positionMatrix[layerIndex][w][h], int layerIndex, std::map<char, std::pair<int, int>>& elementCoordinates, char start, char target, char pathSymbol);
void leeConnectElements(char positionMatrix[layerIndex][w][h], int layerIndex, int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::map<char, std::pair<int, int>>& elementCoordinates, std::unordered_map<char, int> elementMap);

std::vector<std::pair<int, int>> getDirectionsA(const std::pair<int, int>& A, const std::pair<int, int>& B, int rayNumber);
std::vector<std::pair<int, int>> getDirectionsB(const std::pair<int, int>& A, const std::pair<int, int>& B, int rayNumber);
int traceBackPath(const std::vector<std::vector<std::pair<int, int>>>& prev, std::pair<int, int> start);
void rayRestorePath(char positionMatrix[layerIndex][w][h], int layer, std::vector<std::vector<std::pair<int, int>>>& prev, std::pair<int, int> start, char marker);
bool processRay(char positionMatrix[layerIndex][w][h], int layer,
    std::queue<std::pair<int, int>>& queue,
    std::vector<std::vector<bool>>& visited,
    std::vector<std::vector<std::pair<int, int>>>& prev,
    const std::vector<std::pair<int, int>>& directions,
    const std::vector<std::vector<bool>>& otherVisited,
    std::pair<int, int>& meetPoint);
int rayAlgorithm(char positionMatrix[layerIndex][w][h], int layer, std::map<char, std::pair<int, int>>& elementCoordinates, char start, char target, char pathSymbol);
void rayConnectElements(char positionMatrix[layerIndex][w][h], int layer, int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::map<char, std::pair<int, int>>& elementCoordinates, std::unordered_map<char, int> elementMap);



int main()
{

    char positionMatrix[layerIndex][w][h];
    char positionMatrixRay[layerIndex][w][h];
    int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES] =
    {
        0
        /*{0,0,0,0,0,1,1},
        {0,0,2,0,1,0,1},
        {0,2,0,0,1,0,0},
        {0,0,0,0,0,2,2},
        {0,1,1,0,0,2,0},
        {1,0,0,2,2,0,0},
        {1,1,0,2,0,0,0}*/
    };


    std::map<char, std::pair<int, int>> elementCoordinates;
    std::unordered_map<char, int> elementMap;

    elementCoordinates['A'] = { 2,1 };
    elementCoordinates['B'] = { 5,1 };
    elementCoordinates['C'] = { 2,4 };
    elementCoordinates['D'] = { 7,8 };
    elementCoordinates['E'] = { 1,7 };
    elementCoordinates['F'] = { 6,4 };
    elementCoordinates['H'] = { 3,9 };

    initializePositionMatrix(positionMatrix);
    initializePositionMatrix(positionMatrixRay);
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

    inputPositionMatrix(positionMatrix, elementCoordinates);


    int count = 0;
    for (const auto& element : elementCoordinates)
    {
        char key = element.first;
        int x = element.second.first;
        int y = element.second.second;

        //positionMatrix[0][x][y] = key;
        positionMatrix[1][x][y] = key;
        positionMatrixRay[0][x][y] = key;
        positionMatrixRay[1][x][y] = key;
        positionMatrixRay[2][x][y] = key;
        elementMap.insert({ key,count });
        count++;
    }

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

    inputAdjacencyMatrix(adjacencyMatrix, elementMap);

    printPositionMatrix(positionMatrix, 0);
    printAdjacencyMatrix(adjacencyMatrix, elementMap);

    std::cout << "\n//////////////\n";
    std::cout << "Lee algorithm:\n";
    std::cout << "//////////////\n";
    leeConnectElements(positionMatrix, 0, adjacencyMatrix, elementCoordinates, elementMap);

    std::cout << "\n//////////////////////\n";
    std::cout << "Ray search algorithm:\n";
    std::cout << "//////////////////////\n";
    printPositionMatrix(positionMatrixRay, 0);
    rayConnectElements(positionMatrixRay, 0, adjacencyMatrix, elementCoordinates, elementMap);
}

void initializePositionMatrix(char positionMatrix[layerIndex][w][h])
{
    for (int k = 0; k < layerIndex; k++)
    {
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                positionMatrix[k][j][i] = '.';
            }
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

int leeAlgorithm(char positionMatrix[layerIndex][w][h], int layer, std::map<char, std::pair<int, int>>& elementCoordinates, char start, char target, char pathSymbol)
{
    const int dx[] = { -1, 1, 0, 0 };
    const int dy[] = { 0, 0, -1, 1 };

    int startX = elementCoordinates[start].first;
    int startY = elementCoordinates[start].second;
    int targetX = elementCoordinates[target].first;
    int targetY = elementCoordinates[target].second;

    int distanceMatrix[w][h];
    for (auto& row : distanceMatrix)
        for (auto& col : row)
            col = -1;

    std::vector<std::vector<std::pair<int, int>>> prevPositions(w, std::vector<std::pair<int, int>>(h, { -1, -1 }));

    std::queue<std::pair<int, int>> q;

    distanceMatrix[startX][startY] = 0;
    q.push({ startX,startY });

    while (!q.empty())
    {
        auto [x, y] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (((newX >= 0 && newX < w && newY >= 0 && newY < h) && (positionMatrix[layer][newX][newY] == '.' || positionMatrix[layer][newX][newY] == target)) && distanceMatrix[newX][newY] == -1)
            {
                distanceMatrix[newX][newY] = distanceMatrix[x][y] + 1;
                prevPositions[newX][newY] = { x,y };
                q.push({ newX,newY });

                if (newX == targetX && newY == targetY)
                {
                    int pathX = targetX, pathY = targetY;
                    while (pathX != startX || pathY != startY)
                    {
                        if (pathX != targetX || pathY != targetY)
                        {
                            positionMatrix[layer][pathX][pathY] = pathSymbol;
                        }
                        std::tie(pathX, pathY) = prevPositions[pathX][pathY];
                    }
                    return distanceMatrix[newX][newY];
                }
            }
        }
    }
    return -1;
}

void leeConnectElements(char positionMatrix[layerIndex][w][h], int layer, int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::map<char, std::pair<int, int>>& elementCoordinates, std::unordered_map<char, int> elementMap)
{
    int leeSumOfLengths = 0;
    int pathNum = 49; //'1'

    std::set<std::pair<char, char>> connectedPairs;

    while (!elementMap.empty())
    {
        char maxElement = findMaxConnectedElement(adjacencyMatrix, elementMap);
        if (maxElement == '\0')
        {
            break;
        }

        int maxIndex = elementMap[maxElement];
        bool connected = false;
        for (const auto& pair : elementMap)
        {
            char otherElement = pair.first;
            int otherIndex = pair.second;

            if (adjacencyMatrix[maxIndex][otherIndex] > 0 && connectedPairs.find({ maxElement, otherElement }) == connectedPairs.end())
            {
                std::cout << "Connecting " << maxElement << " with " << otherElement << "...\n";
                int result = -1;
                for (layer = 0; layer < layerIndex; layer++)
                {
                    result = leeAlgorithm(positionMatrix, layer, elementCoordinates, maxElement, otherElement, pathNum);
                    if (result != -1)
                    {
                        std::cout << "Length of shortest path: " << result << std::endl;
                        printPositionMatrix(positionMatrix, layer);

                        connectedPairs.insert({ maxElement, otherElement });
                        connectedPairs.insert({ otherElement, maxElement });

                        connected = true;

                        leeSumOfLengths += result * adjacencyMatrix[maxIndex][otherIndex];
                        pathNum++;
                        break;
                    }
                    else
                    {
                        std::cout << "No path for " << maxElement << " with " << otherElement << "!\n";
                        std::cout << "Trying layer [" << layer + 1 << "]...\n";
                    }
                }
            }
        }

        if (!connected)
        {
            elementMap.erase(maxElement);
        }
    }
    std::cout << "Sum of all Lee length * number of connections: " << leeSumOfLengths << std::endl;
}

void rayRestorePath(char positionMatrix[layerIndex][w][h], int layer, std::vector<std::vector<std::pair<int, int>>>& prev, std::pair<int, int> start, char marker)
{
    while (prev[start.first][start.second] != std::make_pair(-1, -1)) {
        positionMatrix[layer][start.first][start.second] = marker;
        start = prev[start.first][start.second];
    }
}

std::vector<std::pair<int, int>> getDirectionsA(const std::pair<int, int>& A, const std::pair<int, int>& B, int rayNumber) 
{
    bool isRight = A.second < B.second;
    bool isBelow = A.first < B.first;

    if (rayNumber == 1) 
    {
        return isBelow ? std::vector<std::pair<int, int>>{{1, 0}, { 0, isRight ? 1 : -1 }}   // Вниз, вправо або вліво
        : std::vector<std::pair<int, int>>{ {-1, 0}, {0, isRight ? 1 : -1} };  // Вгору, вправо або вліво
    }
    else 
    {
        return isBelow ? std::vector<std::pair<int, int>>{{0, isRight ? 1 : -1}, { 1, 0 }}   // Вправо/вліво, вниз
        : std::vector<std::pair<int, int>>{ {0, isRight ? 1 : -1}, {-1, 0} }; // Вправо/вліво, вгору
    }
}

std::vector<std::pair<int, int>> getDirectionsB(const std::pair<int, int>& A, const std::pair<int, int>& B, int rayNumber) 
{
    bool isRight = A.second < B.second;
    bool isBelow = A.first < B.first;

    if (rayNumber == 1) {
        return isBelow ? std::vector<std::pair<int, int>>{{-1, 0}, { 0, isRight ? -1 : 1 }}   // Вгору, вліво або вправо
        : std::vector<std::pair<int, int>>{ {1, 0}, {0, isRight ? -1 : 1} };   // Вниз, вліво або вправо
    }
    else 
    {
        return isBelow ? std::vector<std::pair<int, int>>{{0, isRight ? -1 : 1}, { -1, 0 }}   // Вліво/вправо, вгору
        : std::vector<std::pair<int, int>>{ {0, isRight ? -1 : 1}, {1, 0} };   // Вліво/вправо, вниз
    }
}

int traceBackPath(const std::vector<std::vector<std::pair<int, int>>>& prev, std::pair<int, int> start) 
{
    int length = 0;
    while (prev[start.first][start.second] != std::make_pair(-1, -1)) 
    {
        length++;
        start = prev[start.first][start.second];
    }
    return length;
}

bool processRay(char positionMatrix[layerIndex][w][h], int layer,
    std::queue<std::pair<int, int>>& queue, 
    std::vector<std::vector<bool>>& visited,
    std::vector<std::vector<std::pair<int, int>>>& prev,
    const std::vector<std::pair<int, int>>& directions,
    const std::vector<std::vector<bool>>& otherVisited,
    std::pair<int, int>& meetPoint)
{
    if (!queue.empty()) 
    {
        auto [x, y] = queue.front();
        queue.pop();

        for (const auto& dir : directions) 
        {
            int newX = x + dir.first;
            int newY = y + dir.second;

            if (newX >= 0 && newX < w && newY >= 0 && newY < h && positionMatrix[layer][newX][newY] == '.' && !visited[newX][newY]) 
            {
                visited[newX][newY] = true;
                queue.push({ newX, newY });
                prev[newX][newY] = { x, y };

                if (otherVisited[newX][newY]) 
                {
                    meetPoint = { newX, newY };
                    return true;
                }
            }
        }
    }
    return false;
}

int rayAlgorithm(char positionMatrix[layerIndex][w][h], int layer,
    std::map<char, std::pair<int, int>>& elementCoordinates,
    char start, char target, char pathSymbol)
{
    std::pair<int, int> A = elementCoordinates[start];
    std::pair<int, int> B = elementCoordinates[target];

    std::queue<std::pair<int, int>> queueA, queueB;
    queueA.push(A);
    queueB.push(B);

    std::vector<std::vector<bool>> visitedA(w, std::vector<bool>(h, false));
    std::vector<std::vector<bool>> visitedB(w, std::vector<bool>(h, false));

    std::vector<std::vector<std::pair<int, int>>> prevA(w, std::vector<std::pair<int, int>>(h, { -1, -1 }));
    std::vector<std::vector<std::pair<int, int>>> prevB(w, std::vector<std::pair<int, int>>(h, { -1, -1 }));

    visitedA[A.first][A.second] = true;
    visitedB[B.first][B.second] = true;

    std::vector<std::pair<int, int>> directionsA1 = getDirectionsA(A, B, 1);
    std::vector<std::pair<int, int>> directionsA2 = getDirectionsA(A, B, 2);
    std::vector<std::pair<int, int>> directionsB1 = getDirectionsB(A, B, 1);
    std::vector<std::pair<int, int>> directionsB2 = getDirectionsB(A, B, 2);

    std::pair<int, int> meetPoint;

    while (!queueA.empty() && !queueB.empty()) 
    {
        if (processRay(positionMatrix, layer, queueA, visitedA, prevA, directionsA1, visitedB, meetPoint) ||
            processRay(positionMatrix, layer, queueA, visitedA, prevA, directionsA2, visitedB, meetPoint) ||
            processRay(positionMatrix, layer, queueB, visitedB, prevB, directionsB1, visitedA, meetPoint) ||
            processRay(positionMatrix, layer, queueB, visitedB, prevB, directionsB2, visitedA, meetPoint))
        {
            rayRestorePath(positionMatrix, layer, prevA, meetPoint, pathSymbol);
            rayRestorePath(positionMatrix, layer, prevB, meetPoint, pathSymbol);

            return traceBackPath(prevA, meetPoint) + traceBackPath(prevB, meetPoint);
        }
    }

    return -1;
}

void rayConnectElements(char positionMatrix[layerIndex][w][h], int layer, int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::map<char, std::pair<int, int>>& elementCoordinates, std::unordered_map<char, int> elementMap)
{
    int leeSumOfLengths = 0;
    int pathNum = 49; //'1'

    std::set<std::pair<char, char>> connectedPairs;

    while (!elementMap.empty())
    {
        char maxElement = findMaxConnectedElement(adjacencyMatrix, elementMap);
        if (maxElement == '\0')
        {
            break;
        }

        int maxIndex = elementMap[maxElement];
        bool connected = false;
        for (const auto& pair : elementMap)
        {
            char otherElement = pair.first;
            int otherIndex = pair.second;

            if (adjacencyMatrix[maxIndex][otherIndex] > 0 && connectedPairs.find({ maxElement, otherElement }) == connectedPairs.end())
            {
                std::cout << "Connecting " << maxElement << " with " << otherElement << "...\n";
                int result = -1;
                for (layer = 0; layer < layerIndex; layer++)
                {
                    result = rayAlgorithm(positionMatrix, layer, elementCoordinates, maxElement, otherElement, pathNum);
                    if (result != -1)
                    {
                        std::cout << "Length of shortest path: " << result << std::endl;
                        printPositionMatrix(positionMatrix, layer);

                        connectedPairs.insert({ maxElement, otherElement });
                        connectedPairs.insert({ otherElement, maxElement });

                        connected = true;

                        leeSumOfLengths += result * adjacencyMatrix[maxIndex][otherIndex];
                        pathNum++;
                        break;
                    }
                    else
                    {
                        std::cout << "No path for " << maxElement << " with " << otherElement << "!\n";
                        std::cout << "Trying layer [" << layer + 1 << "]...\n";
                    }
                }
            }
        }

        if (!connected)
        {
            elementMap.erase(maxElement);
        }
    }
    std::cout << "Sum of all Ray length * number of connections: " << leeSumOfLengths << std::endl;
}

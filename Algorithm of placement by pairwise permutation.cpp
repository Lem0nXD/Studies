//Алгоритм розміщення методом попарної перестановки
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

const int MAX_VERTICES = 12;

std::vector <int> fixedPositions(12, -1);
std::unordered_set<int> fixedPositionsSet;
short int fixedPositionsIndex = 0;

void printMatrix(int(&matrix)[MAX_VERTICES][MAX_VERTICES], std::string label);
void updateConnections(const int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::vector<std::pair<int, int>>& amountOfElementConnections);
int selectNextElement(const std::vector<std::pair<int, int>>& connections);
std::pair<int, int> findBestPlace(const std::vector<std::vector<int>>& positionMatrix, int element, const int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES]);
void computeDistanceMatrix(const std::vector<std::vector<int>>& positionMatrix, int distanceMatrix[MAX_VERTICES][MAX_VERTICES]);
int calculateEfficiency(int(&matrix1)[MAX_VERTICES][MAX_VERTICES], int(&matrix2)[MAX_VERTICES][MAX_VERTICES]);

int main()
{
	int w, h, u, v;
	double answ1, answ2, E;
	std::vector <std::vector<int>> positionMatrix;
	int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES] = { 0 };
	int distanceMatrix[MAX_VERTICES][MAX_VERTICES] = { 0 };
	std::vector <std::pair<int, int>> amountOfElementConnections(12, { -1,-1 });
	std::vector <std::vector<int>> randomPositionMatrix{
		/*{1,2,0,0},
		{0,0,11,10},
		{8,6,12,3},
		{9,4,0,0},
		{7,5,0,0}*/ //Розміщення елементів за ручним обчисленнями
		{1,2,5,7},
		{4,8,9,10},
		{11,6,12,3},
		{0,0,0,0},
		{0,0,0,0}
	};
	int randomDistanceMatrix[MAX_VERTICES][MAX_VERTICES] = { 0 };


	std::cout << "Enter w and h of position matrix" << std::endl;
	std::cin >> w >> h;

	positionMatrix.resize(h, std::vector<int>(w));

	/*
	1 0 0 0
	0 0 0 0
	0 6 0 3
	0 0 0 0
	0 0 0 0
	*/

	std::cout << "Enter position matrix" << std::endl;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			std::cin >> positionMatrix[i][j];
		}
	}

	/*
	1 2
	1 2
	1 2
	2 3
	2 10
	3 10
	3 5
	10 11
	11 12
	11 12
	5 4
	5 7
	5 7
	7 4
	7 9
	9 8
	9 8
	8 6
	6 12
	6 4
	6 4
	6 4
	*/

	std::cout << "Enter u,v (adjacencyMatrix)\nEnter -1 -1 to finish!\n";
	while (true)
	{
		std::cin >> u >> v;
		if (u == -1 || v == -1)
		{
			break;
		}
		else if (u == 0 || v == 0 || u > MAX_VERTICES || v > MAX_VERTICES)
		{
			std::cout << "Invalid input, out of bounds!" << std::endl;
			continue;
		}
		adjacencyMatrix[u - 1][v - 1]++;
		adjacencyMatrix[v - 1][u - 1]++;
	}

	printMatrix(adjacencyMatrix, "Adjacency matrix:\n");

	std::cout << "Enter fixed positions\nEnter -1 to finish!\n";
	while (true)
	{
		std::cin >> u;
		if (u == -1)
		{
			break;
		}
		else if (u <= 0 || u > MAX_VERTICES)
		{
			std::cout << "Invalid input, out of bounds!" << std::endl;
			continue;
		}
		fixedPositions[u - 1] = (u - 1);
		fixedPositionsSet.insert(u - 1);
		fixedPositionsIndex++;
	}

	std::cout << "Fixed positions: ";
	for (int& x : fixedPositions)
		std::cout << x << " ";
	std::cout << std::endl;

	updateConnections(adjacencyMatrix, amountOfElementConnections);

	for (int i = 0; i < MAX_VERTICES; i++)
	{
		if (fixedPositionsSet.find(i) == fixedPositionsSet.end())
		{
			std::cout << "[" << (i + 1) << "] " << amountOfElementConnections[i].first << " " << amountOfElementConnections[i].second << std::endl;
		}
	}

	while (true) {
		int nextElement = selectNextElement(amountOfElementConnections);
		if (nextElement == -1)
		{
			std::cout << "All elements are placed!" << std::endl;
			break;
		}

		std::pair<int, int> bestPosition = findBestPlace(positionMatrix, nextElement, adjacencyMatrix);
		if (bestPosition.first == -1 && bestPosition.second == -1)
		{
			std::cout << "No available positions left!" << std::endl;
			break;
		}

		positionMatrix[bestPosition.first][bestPosition.second] = nextElement + 1;
		fixedPositions[nextElement] = (nextElement);
		fixedPositionsSet.insert(nextElement);

		std::cout << "Fixed positions: ";
		for (int& x : fixedPositions)
			std::cout << x << " ";
		std::cout << std::endl;

		std::cout << "Placed element " << (nextElement + 1) << " at position (" << bestPosition.first << ", " << bestPosition.second << ")" << std::endl;
		updateConnections(adjacencyMatrix, amountOfElementConnections);
	}

	std::cout << "Final placement:" << std::endl;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			std::cout << positionMatrix[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "Random placement:" << std::endl;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			std::cout << randomPositionMatrix[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	computeDistanceMatrix(positionMatrix, distanceMatrix);
	computeDistanceMatrix(randomPositionMatrix, randomDistanceMatrix);

	answ1 = calculateEfficiency(adjacencyMatrix, randomDistanceMatrix);
	std::cout << "Before pairwise placement of elements: " << answ1 << std::endl;
	answ2 = calculateEfficiency(adjacencyMatrix, distanceMatrix);
	std::cout << "After pairwise placement of elements: " << answ2 << std::endl;

	E = ((answ1 - answ2) / answ1) * 100;
	std::cout << "E = " << E << "%";
	return 0;
}

void printMatrix(int(&matrix)[MAX_VERTICES][MAX_VERTICES], std::string label)
{
	std::cout << label << ":\n";
	for (int i = 0; i < MAX_VERTICES; i++)
	{
		for (int j = 0; j < MAX_VERTICES; j++)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void updateConnections(const int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES], std::vector<std::pair<int, int>>& amountOfElementConnections) {
	for (int i = 0; i < MAX_VERTICES; i++)
	{
		if (fixedPositionsSet.find(i) == fixedPositionsSet.end())
		{
			amountOfElementConnections[i].first = 0;
			amountOfElementConnections[i].second = 0;
			for (int j = 0; j < MAX_VERTICES; j++)
			{
				if (fixedPositionsSet.find(j) != fixedPositionsSet.end())
				{
					amountOfElementConnections[i].first += adjacencyMatrix[i][j];
				}
				else
				{
					amountOfElementConnections[i].second += adjacencyMatrix[i][j];
				}
			}
		}
	}
}

int selectNextElement(const std::vector<std::pair<int, int>>& connections)
{
	int bestIndex = -1;
	int maxFixedConnections = -1;
	int minUnfixedConnections = std::numeric_limits<int>::max();

	for (int i = 0; i < MAX_VERTICES; i++)
	{
		if (fixedPositionsSet.find(i) == fixedPositionsSet.end())
		{
			if (connections[i].first > maxFixedConnections ||
				(connections[i].first == maxFixedConnections && connections[i].second < minUnfixedConnections))
			{
				bestIndex = i;
				maxFixedConnections = connections[i].first;
				minUnfixedConnections = connections[i].second;
			}
		}
	}
	return bestIndex;
}

std::pair<int, int> findBestPlace(const std::vector<std::vector<int>>& positionMatrix, int element, const int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES]) {
	std::pair<int, int> bestPlace = { -1, -1 };
	int minDistanceSum = std::numeric_limits<int>::max();

	std::unordered_map<int, std::pair<int, int>> fixedElementPositions;
	for (int i = 0; i < positionMatrix.size(); i++)
	{
		for (int j = 0; j < positionMatrix[i].size(); j++)
		{
			if (positionMatrix[i][j] != 0)
			{
				fixedElementPositions[positionMatrix[i][j]] = { i, j };
			}
		}
	}

	for (int i = 0; i < positionMatrix.size(); i++)
	{
		for (int j = 0; j < positionMatrix[i].size(); j++)
		{
			if (positionMatrix[i][j] == 0)
			{
				int currentDistanceSum = 0;
				for (const auto& [fixedElement, pos] : fixedElementPositions)
				{
					if (adjacencyMatrix[element][fixedElement - 1] > 0)
					{
						currentDistanceSum += abs(pos.first - i) + abs(pos.second - j);
					}
				}

				if (currentDistanceSum < minDistanceSum)
				{
					minDistanceSum = currentDistanceSum;
					bestPlace = { i, j };
				}
			}
		}
	}

	return bestPlace;
}


void computeDistanceMatrix(const std::vector<std::vector<int>>& positionMatrix, int distanceMatrix[MAX_VERTICES][MAX_VERTICES])
{
	std::unordered_map<int, std::pair<int, int>> elementPositions;

	for (int i = 0; i < positionMatrix.size(); i++)
	{
		for (int j = 0; j < positionMatrix[i].size(); j++)
		{
			if (positionMatrix[i][j] != 0)
			{
				elementPositions[positionMatrix[i][j]] = { i, j };
			}
		}
	}

	for (int i = 0; i < MAX_VERTICES; i++)
	{
		for (int j = 0; j < MAX_VERTICES; j++)
		{
			if (i != j)
			{
				auto it1 = elementPositions.find(i + 1);
				auto it2 = elementPositions.find(j + 1);

				if (it1 != elementPositions.end() && it2 != elementPositions.end())
				{
					const auto& pos1 = it1->second;
					const auto& pos2 = it2->second;
					distanceMatrix[i][j] = abs(pos1.first - pos2.first) + abs(pos1.second - pos2.second);
				}
				else
				{
					distanceMatrix[i][j] = 0;
				}
			}
			else
			{
				distanceMatrix[i][j] = 0;
			}
		}
	}
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
	int sum = 0;
	for (int i = 0; i < MAX_VERTICES; i++) {
		for (int j = 0; j < MAX_VERTICES; j++) {
			sum += tempMatrix[i][j];
		}
	}
	answ = (sum / 2);
	return answ;
}

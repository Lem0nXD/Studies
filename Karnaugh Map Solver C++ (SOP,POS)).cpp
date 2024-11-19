//Karnaugh Map Solver C++
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

const int MapLayout[4][4] = {
    {0, 1, 3, 2},
    {4, 5, 7, 6},
    {12, 13, 15, 14},
    {8, 9, 11, 10}
};

string toBinary(int num, int bits);
void displayKMap(const vector<string>& kmap, int rows, int cols);
void displayMapLayout();
void displayOptimalGroups(vector<vector<int>>& optimalGroups);
void placeMintermsInKMap(vector<string>& kmap, const vector<int>& minterms, int rows, int cols);
vector<vector<int>> findGroups(const vector<string>& kmap, int rows, int cols, string value);
vector<vector<int>> selectOptimalGroups(vector<vector<int>>& groups, int rows, int cols);
vector<string> groupsToSOP(const vector<vector<int>>& groups, int variables);
vector<string> groupsToPOS(const vector<vector<int>>& groups, int variables);

int main() 
{
    vector<int> minterms;
    cout << "Enter the minterms (separated by space, end with -1): ";
    int term;
    while (cin >> term && term != -1) 
    {
        minterms.push_back(term);
    }

    int variables = 4;
    int rows = 4, cols = 4;
    int numCells = rows * cols;

    vector<string> kmap(numCells, "0");
    placeMintermsInKMap(kmap, minterms, rows, cols);

    displayKMap(kmap, rows, cols);
    displayMapLayout();
    
    auto allGroupsSOP = findGroups(kmap, rows, cols, "1");
    auto allGroupsPOS = findGroups(kmap, rows, cols, "0");
    auto optimalGroupsSOP = selectOptimalGroups(allGroupsSOP, rows, cols);
    auto optimalGroupsPOS = selectOptimalGroups(allGroupsPOS, rows, cols);

    displayOptimalGroups(optimalGroupsSOP);
    auto simplifiedSOP = groupsToSOP(optimalGroupsSOP, variables);

    cout << "\nSimplified SOP:\n";
    for (const auto& term : simplifiedSOP) 
    {
        cout << term << " + ";
    }
    cout << "\b\b \n";

    displayOptimalGroups(optimalGroupsPOS);
    auto simplifiedPOS = groupsToPOS(optimalGroupsPOS, variables);

    cout << "\nSimplified POS:\n";
    for (const auto& term : simplifiedPOS)
    {
        cout << term << " + ";
    }
    cout << "\b\b \n";
    return 0;
}

string toBinary(int num, int bits)
{
    string binary(bits, '0');
    for (int i = bits - 1; i >= 0; i--)
    {
        if (num & (1 << i))
            binary[bits - 1 - i] = '1';
    }
    return binary;
}

void displayKMap(const vector<string>& kmap, int rows, int cols)
{
    cout << "\nK-map:\n";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            cout << setw(2) << kmap[i * cols + j] << " ";

        cout << endl;
    }
}

void displayMapLayout()
{
    cout << "\nMap Layout\n";
    for (auto& x : MapLayout)
    {
        for (int k : x)
        {
            std::cout << setw(2) << k << ' ';
        }
        std::cout << '\n';
    }
}

void displayOptimalGroups(vector<vector<int>>& optimalGroups)
{
    cout << "\nOptimal Groups:\n";
    for (int i = 0; i < optimalGroups.size(); i++)
    {
        cout << "Group " << i + 1 << ": ";
        for (int idx : optimalGroups[i])
        {
            cout << idx << " ";
        }
        cout << endl;
    }
}

void placeMintermsInKMap(vector<string>& kmap, const vector<int>& minterms, int rows, int cols)
{
    for (int minterm : minterms)
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (MapLayout[i][j] == minterm)
                    kmap[i * cols + j] = "1";
}

vector<vector<int>> findGroups(const vector<string>& kmap, int rows, int cols, string value)
{
    vector<vector<int>> groups;
    vector<pair<int, int>> groupSizes = { {4, 4}, {4, 2}, {2, 4}, {4, 1}, {1, 4}, {2, 2}, {2, 1}, {1, 2}, {1, 1} };

    for (auto [height, width] : groupSizes)
    {
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                bool validGroup = true;
                vector<int> group;
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        int nr = (r + i) % rows;
                        int nc = (c + j) % cols;
                        int index = nr * cols + nc;

                        if (kmap[index] != value)
                        {
                            validGroup = false;
                            break;
                        }
                        group.push_back(index);
                    }
                    if (!validGroup) break;
                }
                if (validGroup)
                {
                    groups.push_back(group);
                }
            }
        }
    }
    return groups;
}

vector<vector<int>> selectOptimalGroups(vector<vector<int>>& groups, int rows, int cols)
{
    vector<bool> visited(rows * cols, false);
    vector<vector<int>> selectedGroups;

    vector<int> kmapToLayout(rows * cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            kmapToLayout[i * cols + j] = MapLayout[i][j];

    sort(groups.begin(), groups.end(), [](const vector<int>& a, const vector<int>& b) {
        return a.size() > b.size();
        });

    for (const auto& group : groups)
    {
        bool canUse = false;
        for (int idx : group)
        {
            if (!visited[idx])
            {
                canUse = true;
                break;
            }
        }

        if (canUse)
        {
            vector<int> translatedGroup;
            for (int idx : group)
            {
                translatedGroup.push_back(kmapToLayout[idx]);
                visited[idx] = true;
            }
            sort(translatedGroup.begin(), translatedGroup.end(), less<int>());
            selectedGroups.push_back(translatedGroup);
        }
    }
    return selectedGroups;
}

vector<string> groupsToSOP(const vector<vector<int>>& groups, int variables)
{
    vector<string> result;

    for (const auto& group : groups)
    {
        string term(variables, '-');
        term = toBinary(group[0], variables);

        for (int j = 1; j < group.size(); j++)
        {
            string binary = toBinary(group[j], variables);

            for (int i = 0; i < variables; i++)
            {
                if (term[i] != binary[i])
                {
                    term[i] = '-';
                }
            }
        }

        string simplifiedTerm;
        for (int i = 0; i < variables; i++)
        {
            if (term[i] == '1')
            {
                simplifiedTerm += (char)('A' + i);
            }
            else if (term[i] == '0')
            {
                simplifiedTerm += (char)('A' + i);
                simplifiedTerm += "'";
            }
        }
        result.push_back(simplifiedTerm);
    }
    return result;
}

vector<string> groupsToPOS(const vector<vector<int>>& groups, int variables)
{
    vector<string> result;

    for (const auto& group : groups)
    {
        string term(variables, '-');
        term = toBinary(group[0], variables);

        for (int j = 1; j < group.size(); j++)
        {
            string binary = toBinary(group[j], variables);

            for (int i = 0; i < variables; i++)
            {
                if (term[i] != binary[i])
                {
                    term[i] = '-';
                }
            }
        }
        string simplifiedTerm;
        bool isFirst = true;
        for (int i = 0; i < variables; i++)
        {
            if (term[i] == '1')
            {
                if (!isFirst)
                {
                    simplifiedTerm += "+";
                }
                simplifiedTerm += (char)('A' + i);
                simplifiedTerm += "'";
                isFirst = false;
            }
            else if (term[i] == '0')
            {
                if (!isFirst)
                {
                    simplifiedTerm += "+";
                }
                simplifiedTerm += (char)('A' + i);
                isFirst = false;
            }
        }
        result.push_back("(" + simplifiedTerm + ")");
    }

    if (!result.empty())
    {
        string finalResult = result[0];
        for (size_t i = 1; i < result.size(); i++)
        {
            finalResult += " * " + result[i];
        }
        result.clear();
        result.push_back(finalResult);
    }

    return result;
}

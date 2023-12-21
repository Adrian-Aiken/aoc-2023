#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int64_t INFLATION_SIZE = 1000000 - 1; // -1 to account for existing single row
vector<int64_t> expandRows;
vector<int64_t> expandCols;

pair<int64_t, int64_t> getExpandedCoords(int x, int y)
{
    pair<int64_t, int64_t> expanded = pair<int64_t, int64_t>(x, y);

    for (auto row: expandRows)
    {
        if (x >= row) expanded.first += INFLATION_SIZE;
    }

    for (auto col: expandCols)
    {
        if (y >= col) expanded.second += INFLATION_SIZE;
    }

    return expanded;
}

int main()
{
    string line;
    ifstream input;
    vector<string> universe;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);
        universe.push_back(line);
    }

    // Add rows
    for (int i = 0; i < universe.size(); i++)
    {
        if (universe[i].find("#") == string::npos)
        {
            expandRows.push_back(i);
        }
    }

    // Add columns
    for (int j = 0; j < universe[0].size(); j++)
    {
        bool isEmpty = true;
        for (int i = 0; i < universe.size(); i++)
        {
            if (universe[i][j] == '#')
            {
                isEmpty = false;
                break;
            }
        }

        if (isEmpty)
        {
            expandCols.push_back(j);
        }
    }

    // Find and mark galaxies
    vector<pair<int64_t, int64_t>> galaxies;
    for (int x = 0; x < universe.size(); x++)
    {
        for (int y = 0; y < universe[x].size(); y++)
        {
            if (universe[x][y] == '#') galaxies.push_back(getExpandedCoords(x, y));
        }
    }

    // Get paired distances
    int64_t distanceSum = 0;
    for (int i = 0; i < galaxies.size(); i++)
    {
        for (int j = i+1; j < galaxies.size(); j++)
        {
            distanceSum += abs(galaxies[i].first - galaxies[j].first) + abs(galaxies[i].second - galaxies[j].second);
        }
    }
    
    cout << endl << "Output: " << distanceSum << endl;
}
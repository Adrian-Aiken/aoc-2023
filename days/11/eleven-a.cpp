#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    string line;
    ifstream input;
    vector<string> universe;

    input.open("sample.txt");

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
            universe.insert(universe.begin()+i, universe[i]);
            i++;
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

        if (!isEmpty) continue;

        for (int i = 0; i < universe.size(); i++)
        {
            universe[i].insert(universe[i].begin()+j, '.');
        }
        j++;
    }

    // Find and mark galaxies
    vector<pair<int, int>> galaxies;
    for (int x = 0; x < universe.size(); x++)
    {
        for (int y = 0; y < universe[x].size(); y++)
        {
            if (universe[x][y] == '#') galaxies.push_back(pair<int, int>(x, y));
        }
    }

    // Get paired distances
    uint64_t distanceSum = 0;
    for (int i = 0; i < galaxies.size(); i++)
    {
        for (int j = i+1; j < galaxies.size(); j++)
        {
            distanceSum += abs(galaxies[i].first - galaxies[j].first) + abs(galaxies[i].second - galaxies[j].second);
        }
    }
    
    cout << endl << "Output: " << distanceSum << endl;
}
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <numeric>

using namespace std;

int main()
{
    string line, directions;
    ifstream input;
    map<string, pair<string, string>> wasteland;
    vector<string> paths;

    input.open("sample.txt");

    getline(input, line);
    directions = line;
    getline(input, line); // Blankline

    while (!input.eof())
    {
        getline(input, line);

        wasteland[line.substr(0, 3)] = pair<string, string>(line.substr(7, 3), line.substr(12, 3));

        if (line[2] == 'A') paths.push_back(line.substr(0,3));
    }

    int64_t pathNum = 1, totalSteps = 1;
    for (auto curNode: paths)
    {
        int64_t numSteps = 0;
        while (curNode[2] != 'Z')
        {
            curNode = directions[numSteps++ % directions.length()] == 'L' ?
                wasteland[curNode].first :
                wasteland[curNode].second;
        }

        totalSteps = std::lcm(totalSteps, numSteps);
        cout << pathNum++ << ": " << numSteps << " steps" << endl;
    }

    cout << endl << "Total Num Steps: " << totalSteps << endl;
}
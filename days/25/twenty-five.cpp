#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <time.h>

using namespace std;

struct Edge {
    int u, v;
};

vector<string> split(string s, string delim)
{
    vector<string> output;
    while (s.find(delim) != string::npos)
    {
        size_t pos = s.find(delim);
        output.push_back(s.substr(0, pos));
        s.erase(0, pos + delim.length());
    }
    output.push_back(s);
    return output;
}

int getRand(int max) { return rand() % max; }

int main()
{
    string line;
    ifstream input;
    map<string, vector<string>> grid;
    vector<string> allNodes;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);
        auto parts = split(line, ": ");
        string left = parts[0];
        auto right = split(parts[1], " ");

        for (string s : right)
        {
            grid[s].push_back(left);
            grid[left].push_back(s);
        }
    }

    for (auto pair : grid) allNodes.push_back(pair.first);

    srand(time(NULL));
    map<string, vector<string>> karger;
    do {
        karger = grid;

        while (karger.size() > 2) {
            // Pick random node + neighbor
            auto leftIt = karger.begin();
            advance(leftIt, rand() % karger.size());
            auto leftId = leftIt->first;
            auto rightId = karger[leftId][rand() % karger[leftId].size()];
            
            auto left = karger[leftId], right = karger[rightId];
            
            // Erase node graph and eachother's edges
            karger.erase(leftId);
            karger.erase(rightId);
            left.erase(remove(left.begin(), left.end(), rightId), left.end());
            right.erase(remove(right.begin(), right.end(), leftId), right.end());

            // Generate & insert merged nodes; update other nodes
            vector<string> combined = left;
            combined.insert(combined.end(), right.begin(), right.end());
            string newId = leftId + "|" + rightId;
            for (auto a = karger.begin(); a != karger.end(); a++)
            {
                for (auto b = a->second.begin(); b != a->second.end(); b++)
                {
                    if (*b == leftId || *b == rightId) *b = newId;
                }
            }
            karger[newId] = combined;
        }
    }
    while (karger.begin()->second.size() != 3);

    auto leftGroup = split(karger.begin()->first, "|");
    auto rightGroup = split(karger.begin()->second.front(), "|");
    
    cout << endl << "Output: " << leftGroup.size() * rightGroup.size() << endl;
}
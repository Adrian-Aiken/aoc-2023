#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

using namespace std;

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

int main()
{
    string line;
    ifstream input;
    map<int, int> cardCounts;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);

        auto prefix = split(line, ": ");

        int cardNum = stoi(prefix[0].substr(5));
        cardCounts[cardNum]++;

        auto numSets = split(prefix[1], " | ");
        auto winningNums = split(numSets[0], " ");
        auto gameNums = split(numSets[1], " ");

        int wins = 0;
        for (string gameNum : gameNums)
        {
            if (gameNum == "") continue;
            if (find(winningNums.begin(), winningNums.end(), gameNum) != winningNums.end())
            {
                wins++;
            } 
        }

        for (int i = 1; i <= wins; i++)
        {
            cardCounts[cardNum+i] += cardCounts[cardNum];
        }

        cout << prefix[0] << ": " << setw(7) << cardCounts[cardNum] << " copies, " << setw(2) << wins << " wins" << endl;
    }
    
    int sum = 0;
    for (auto cardCount : cardCounts) sum += cardCount.second;
    cout << endl << "Output: " << sum << endl;
}
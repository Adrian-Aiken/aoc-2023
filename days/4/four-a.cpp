#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
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
    int sum;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);

        auto prefix = split(line, ": ");
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

        int score = (int)pow(2, wins-1);
        sum += score;
        cout << prefix[0] << ": " << wins << " wins, " << score << " points" << endl;
    }
    
    cout << endl << "Output: " << sum << endl;
}
#include <iostream>
#include <fstream>
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

    vector<int> raceTimes, raceDistances;

    input.open("sample.txt");

    getline(input, line);
    auto lineParse = split(line.substr(9), " ");
    for (string s: lineParse)
    {
        if (s == "") continue;
        raceTimes.push_back(stoi(s));
    }

    getline(input, line);
    lineParse = split(line.substr(9), " ");
    for (string s: lineParse)
    {
        if (s == "") continue;
        raceDistances.push_back(stoi(s));
    }

    int winProd = 1;
    for (int i = 0; i < raceTimes.size(); i++)
    {        
        //cout << "Race " << i << ":" << endl;
        int winCount = 0;
        for (int holdTime = 1; holdTime < raceTimes[i]; holdTime++)
        {
            int distance = holdTime * (raceTimes[i] - holdTime);
            //cout << "t: " << holdTime << ", d: " << distance << endl;

            if (distance > raceDistances[i]) winCount++;
        }
        winProd *= winCount;
        //cout << endl;
    }
    
    cout << endl << "Output: " << winProd << endl;
}
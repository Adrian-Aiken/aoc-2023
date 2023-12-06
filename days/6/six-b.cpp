#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    string line;
    ifstream input;
    uint64_t raceTime = 0, target = 0;

    input.open("sample.txt");

    getline(input, line);
    for (char c: line.substr(9))
    {
        if (c == ' ') continue;
        raceTime *= 10;
        raceTime += c - '0';
    }

    getline(input, line);
    for (char c: line.substr(9))
    {
        if (c == ' ') continue;
        target *= 10;
        target += c - '0';
    }

    uint64_t holdTime;
    for (holdTime = 0; holdTime < raceTime; holdTime++)
    {
        uint64_t distance = holdTime * (raceTime - holdTime);
        if (distance > target) break;
    }

    uint64_t waysToWin = raceTime - (holdTime * 2) + 1;

    cout << endl << "Output: " << waysToWin << endl;
}
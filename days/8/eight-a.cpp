#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

int main()
{
    string line, directions;
    ifstream input;
    map<string, pair<string, string>> wasteland;

    input.open("sample.txt");

    getline(input, line);
    directions = line;
    getline(input, line); // Blankline

    while (!input.eof())
    {
        getline(input, line);

        wasteland[line.substr(0, 3)] = pair<string, string>(line.substr(7, 3), line.substr(12, 3));
    }

    string curNode = "AAA";
    int numSteps = 0;

    while (curNode != "ZZZ")
    {
        curNode = directions[numSteps++ % directions.length()] == 'L' ?
            wasteland[curNode].first :
            wasteland[curNode].second;
    }
    
    cout << endl << "Num Steps: " << numSteps << endl;
}
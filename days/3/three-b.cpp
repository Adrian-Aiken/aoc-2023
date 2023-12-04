#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct NumPos {
    int number, line, start, length;
    char symbol;
};

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

int main()
{
    string line;
    ifstream input;
    vector<string> schematic;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);
        schematic.push_back(line);
    }

    // Pass 1: Find number runs
    vector<NumPos*> numbers;
    for (int i = 0; i < schematic.size(); i++)
    {
        for (int j = 0; j < schematic[i].size(); j++)
        {
            if (isDigit(schematic[i][j]))
            {
                NumPos* numberRun = new NumPos { 0, i, j, 0, ' ' };
                while (j <= schematic[i].size() && isDigit(schematic[i][j]))
                {
                    numberRun->number = (numberRun->number * 10) + (schematic[i][j] - '0');
                    numberRun->length++;
                    j++;
                }
                numbers.push_back(numberRun);
            }
        }
    }

    // Pass 2: Identify symbols
    int schematicHeight = schematic.size();
    int schematicWidth = schematic[0].length();
    map<pair<int, int>, vector<NumPos*>> gears;
    for (NumPos* partNum : numbers)
    {
        int startLine = max(partNum->line-1, 0);
        int endLine = min(partNum->line+1, schematicHeight-1);
        int startRun = max(partNum->start-1, 0);
        int endRun = min(partNum->start + partNum->length, schematicWidth-1);

        for (int x = startLine; x <= endLine; x++)
        {
            for (int y = startRun; y <= endRun; y++)
            {
                if (schematic[x][y] == '*')
                {
                    partNum->symbol = schematic[x][y];
                    gears[pair<int, int>(x, y)].push_back(partNum);
                }
            }
        }
    }

    // Print out:
    int sum = 0;
    for (auto val : gears)
    {
        if (val.second.size() <= 1) continue;

        cout << val.second[0]->number << " * " << val.second[1]->number << endl;
        sum += val.second[0]->number * val.second[1]->number;
    }
    
    cout << endl << "Output: " << sum << endl;
}
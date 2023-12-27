#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool isSymmetricRow(vector<string> field, int row)
{
    for (int i = 0; i < field.size(); i++)
    {
        int lower = row + i + 1;
        int upper = row - i;

        if (upper < 0 || lower >= field.size()) break;

        if (field[lower] != field[upper]) return false;
    }

    return true;
}

bool isSymmetricColumn(vector<string> field, int col)
{
    for (int i = 0; i < field[0].size(); i++)
    {
        int lower = col + i + 1;
        int upper = col - i;

        if (upper < 0 || lower >= field[0].size()) break;

        for (auto row : field)
        {
            if (row[lower] != row[upper]) return false;
        }
    }

    return true;
}

int main()
{
    string line;
    ifstream input;
    vector<string> buff;
    vector<vector<string>> island;
    uint64_t sum = 0;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);
        
        if (line == "")
        {
            island.push_back(buff);
            buff = vector<string>();
        }
        else
        {
            buff.push_back(line);
        }
    }
    island.push_back(buff);

    for (auto field : island)
    {

        // Check rows
        bool done = false;
        for (int i = 0; i < field.size() - 1; i++)
        {
            if (isSymmetricRow(field, i))
            {
                done = true;
                sum += (i+1) * 100;
                break;
            }
        }

        // Check columns
        if (done) continue;
        for (int i = 0; i < field[0].size() - 1; i++)
        {
            if (isSymmetricColumn(field, i))
            {
                done = true;
                sum += i+1;
                break;
            }
        }
    }
    
    cout << "Output: " << sum << endl;
}
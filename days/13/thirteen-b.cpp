#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int symmetricRowDiff(vector<string> field, int row)
{
    int count = 0;
    for (int i = 0; i < field.size(); i++)
    {
        int lower = row + i + 1;
        int upper = row - i;

        if (upper < 0 || lower >= field.size()) break;

        for (int j = 0; j < field[i].size(); j++)
        {
            if (field[lower][j] != field[upper][j]) count++;
        }
    }

    return count;
}

int symmetricColumnDiff(vector<string> field, int col)
{
    int count = 0;
    for (int i = 0; i < field[0].size(); i++)
    {
        int lower = col + i + 1;
        int upper = col - i;

        if (upper < 0 || lower >= field[0].size()) break;

        for (auto row : field)
        {
            if (row[lower] != row[upper]) count++;
        }
    }

    return count;
}

int main()
{
    string line;
    ifstream input;
    vector<string> buff;
    vector<vector<string>> island;
    uint64_t sum = 0;

    input.open("input.txt");

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
        for (auto s: field) cout << s << endl;

        // Check rows
        bool done = false;
        for (int i = 0; i < field.size() - 1; i++)
        {
            if (symmetricRowDiff(field, i) == 1)
            {
                cout << "Row " << i+1 << endl << endl;
                done = true;
                sum += (i+1) * 100;
                break;
            }
        }

        // Check columns
        if (done) continue;
        for (int i = 0; i < field[0].size() - 1; i++)
        {
            if (symmetricColumnDiff(field, i) == 1)
            {
                cout << "Col " << i+1 << endl << endl;
                done = true;
                sum += i+1;
                break;
            }
        }
    }
    
    cout << "Output: " << sum << endl;
}
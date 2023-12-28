#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool SlideNorth(vector<string>* floor)
{
    bool changes = false;

    //  Note: Skip row 0
    for (int x = 1; x < floor->size(); x++)
    {
        for (int y = 0; y < floor->at(x).size(); y++)
        {
            if (floor->at(x)[y] == 'O' && floor->at(x-1)[y] == '.')
            {
                floor->at(x)[y] = '.';
                floor->at(x-1)[y] = 'O';
                changes = true;
            }
        }
    }

    return changes;
}

int main()
{
    string line;
    ifstream input;
    vector<string>* floor = new vector<string>();

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);
        floor->push_back(line);
    }

    // Slide north
    while (SlideNorth(floor));

    uint64_t load = 0;
    for (int i = 0; i < floor->size(); i++)
    {
        for (auto c : floor->at(i))
        {
            if (c == 'O') load += floor->size() - i;
        }
    }
    
    cout << endl << "Output: " << load << endl;
}
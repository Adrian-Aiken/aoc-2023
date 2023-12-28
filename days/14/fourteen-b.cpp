#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define CYCLE_COUNT 1000000000

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

bool SlideSouth(vector<string>* floor)
{
    bool changes = false;

    //  Note: Skip last row
    for (int x = floor->size() - 2; x >= 0; x--)
    {
        for (int y = 0; y < floor->at(x).size(); y++)
        {
            if (floor->at(x)[y] == 'O' && floor->at(x+1)[y] == '.')
            {
                floor->at(x)[y] = '.';
                floor->at(x+1)[y] = 'O';
                changes = true;
            }
        }
    }

    return changes;
}

bool SlideWest(vector<string>* floor)
{
    bool changes = false;

    for (int y = 1; y < floor->at(0).size(); y++)
    {
        for (int x = 0; x < floor->size(); x++)
        {
            if (floor->at(x)[y] == 'O' && floor->at(x)[y-1] == '.')
            {
                floor->at(x)[y] = '.';
                floor->at(x)[y-1] = 'O';
                changes = true;
            }
        }
    }

    return changes;
}

bool SlideEast(vector<string>* floor)
{
    bool changes = false;

    for (int y = floor->at(0).length() - 2; y >=0; y--)
    {
        for (int x = 0; x < floor->size(); x++)
        {
            if (floor->at(x)[y] == 'O' && floor->at(x)[y+1] == '.')
            {
                floor->at(x)[y] = '.';
                floor->at(x)[y+1] = 'O';
                changes = true;
            }
        }
    }

    return changes;
}

void Cycle(vector<string>* floor)
{
    while (SlideNorth(floor));
    while (SlideWest(floor));
    while (SlideSouth(floor));
    while (SlideEast(floor));
}

bool FloorEqual(vector<string> left, vector<string> right)
{
    if (left.size() != right.size()) return false;

    for (int i = 0; i < left.size(); i++)
    {
        if (left[i] != right[i]) return false;
    }

    return true;
}

int main()
{
    string line;
    ifstream input;
    vector<string>* floor = new vector<string>();
    vector<vector<string>> states;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);
        floor->push_back(line);
    }

    while (true) {
        states.push_back(vector<string>(*floor));

        Cycle(floor);

        for (int i = 0; i < states.size(); i++)
        {
            if (FloorEqual(states[i], *floor))
            {
                cout << "Cycle from " << i << " to " << states.size() << endl;

                int cycleSize = states.size() - i;
                uint64_t finalStateIndex = (CYCLE_COUNT - i) % cycleSize;
                finalStateIndex += i;

                vector<string> finalState = states[finalStateIndex];

                // Calculate stress
                uint64_t load = 0;
                for (int i = 0; i < finalState.size(); i++)
                {
                    for (auto c : finalState[i])
                    {
                        if (c == 'O') load += finalState.size() - i;
                    }
                }
                
                cout << endl << "Output: " << load << endl;
                return 0;
            }
        }
    }
}
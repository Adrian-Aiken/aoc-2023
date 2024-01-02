#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

const int MAX_STEPS = 64;

struct Step { int x, y, steps; };

int main()
{
    string line;
    ifstream input;
    vector<string> field;
    vector<vector<int>> distances;
    queue<Step> steps;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);
        field.push_back(line);
        distances.push_back(vector<int>(line.length(), -1));

        if (line.find('S') != string::npos)
        {
            steps.push(Step{ (int)(field.size() - 1), (int)(line.find('S')), 0 });
        }
    }

    while (!steps.empty())
    {
        auto step = steps.front(); steps.pop();

        // Bounds check, rock check, duplicate check
        if (step.x < 0 || step.x >= field.size() || step.y < 0 || step.y >= field[0].size()) continue;
        if (field[step.x][step.y] == '#') continue;
        if (distances[step.x][step.y] >= 0) continue;

        distances[step.x][step.y] = step.steps;

        steps.push(Step{ step.x-1, step.y, step.steps+1 });
        steps.push(Step{ step.x+1, step.y, step.steps+1 });
        steps.push(Step{ step.x, step.y-1, step.steps+1 });
        steps.push(Step{ step.x, step.y+1, step.steps+1 });
    }

    int count = 0;
    for (auto dv : distances) {
        for (auto d : dv) {
            if (d >= 0 && d <= MAX_STEPS && d % 2 == 0) {
                count++;
            }
        }
    }
    
    cout << "Output: " << count << endl;
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

const int MAX_STEPS = 26501365;

struct Step { int x, y, steps; };

vector<vector<int>> getDistanceMap(vector<string> field, int x, int y)
{
    queue<Step> steps;
    vector<vector<int>> distances;
    for (auto s : field) distances.push_back(vector<int>(s.size(), -1));

    steps.push(Step{ x, y, 0 });
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

    return distances;
}

uint64_t countValidSteps(vector<vector<int>> distances, int max, int modulo, int mTarget)
{
    uint64_t count = 0;
    for (auto dv : distances) {
        for (auto d : dv) {
            if (d >= 0 && d <= max && d%modulo == mTarget) count++;
        }
    }
    return count;
}

int main()
{
    string line;
    ifstream input;
    vector<string> field;
    int startX, startY;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);
        field.push_back(line);

        if (line.find('S') != string::npos)
        {
            startX = field.size() - 1;
            startY = line.find('S');
        }
    }

    for (int j = 0; j < field.size(); j++)
    {
        field[j] = field[j] + field[j] + field[j] + field[j] + field[j];
    }
    int n = field.size();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < n; j++) field.push_back(field[j]);
    }
    
    startX += 2 * 131;
    startY += 2 * 131;
    auto map = getDistanceMap(field, startX, startY);
   
    uint64_t r0 = countValidSteps(map, 65, 2, 1);
    uint64_t r1 = countValidSteps(map, 65 + 131, 2, 1);
    uint64_t r2 = countValidSteps(map, 65 + 131 + 131, 2, 1);

    uint64_t b0 = r0;
    uint64_t b1 = r1 - r0;
    uint64_t b2 = r2 - r1;

    uint64_t x = 202300;

    uint64_t result = ((b0 + b1 * x + (x * (x - 1) / 2) * (b2 - b1)));

    cout << "Output: " << result << endl;
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <tuple>
#include <set>

using namespace std;

struct StepState
{
    int x, y, heat, straights;
    char lastDir;
};

struct CompareState {
    bool operator()(StepState const& s1, StepState const& s2) { return s1.heat > s2.heat; }
};

char turnLeft(char dir) {
    switch (dir)
    {
        case '^': return '<';
        case '<': return 'v';
        case 'v': return '>';
        case '>': return '^';
        default : return ' ';
    }
}

char turnRight(char dir) {
    switch (dir)
    {
        case '^': return '>';
        case '>': return 'v';
        case 'v': return '<';
        case '<': return '^';
        default : return ' ';
    }
}

int stepX(int x, char dir)
{
    switch (dir)
    {
        case '^': return x-1;
        case 'v': return x+1;
        default : return x;
    }
}

int stepY(int y, char dir)
{
    switch (dir)
    {
        case '<': return y-1;
        case '>': return y+1;
        default : return y;
    }
}

bool outOfBounds(vector<string> map, int x, int y) { return x < 0 || x >= map.size() || y < 0 || y >= map[0].size(); }

bool hasVisited(vector<pair<int, int>> path, int x, int y)
{
    for (auto block : path)
    {
        if (block.first == x && block.second == y) return true;
    }

    return false;
}

int main()
{
    string line;
    ifstream input;
    vector<string> map;
    priority_queue<StepState, vector<StepState>, CompareState> queue;
    set<tuple<int, int, int, char>> visited;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);
        map.push_back(line);
    }

    // Populate initial two dirs
    queue.push(StepState{0, 1, map[0][1] - '0', 1, '>' });
    queue.push(StepState{1, 0, map[1][0] - '0', 1, 'v' });

    while (!queue.empty())
    {
        auto state = queue.top(); queue.pop();

        if (visited.find({state.x, state.y, state.straights, state.lastDir}) != visited.end())
            continue;
        visited.insert({state.x, state.y, state.straights, state.lastDir});

        if (outOfBounds(map, state.x, state.y)) break;

        if (state.x == map.size() - 1 && state.y == map[0].size() -1) {
            cout << "Final Heat: " << state.heat << endl;
            return 0;
        }

        if (state.straights < 3)
        {
            int sx = stepX(state.x, state.lastDir);
            int sy = stepY(state.y, state.lastDir);
            if (!outOfBounds(map, sx, sy))
            {
                StepState newState = StepState{ sx, sy, state.heat + (map[sx][sy] - '0'), state.straights + 1, state.lastDir };
                queue.push(newState);
            }
        }
    
        char leftDir = turnLeft(state.lastDir);
        int lx = stepX(state.x, leftDir);
        int ly = stepY(state.y, leftDir);
        if (!outOfBounds(map, lx, ly)) {
            StepState newState = StepState{ lx, ly, state.heat + (map[lx][ly] - '0'), 1, leftDir };
            queue.push(newState);
        }

        char rightDir = turnRight(state.lastDir);
        int rx = stepX(state.x, rightDir);
        int ry = stepY(state.y, rightDir);
        if (!outOfBounds(map, rx, ry)) {
            StepState newState = StepState{ rx, ry, state.heat + (map[rx][ry] - '0'),  1, rightDir };
            queue.push(newState);
        }
    }
    
    cout << endl << "!!! FOUND END !!! " << endl;
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

struct BeamStep
{
    int x, y;
    char dir;
};

struct Dir
{
    bool u, d, l, r;
    bool energized() { return u || d || l || r; }
};

int main()
{
    string line;
    ifstream input;
    vector<string> room;
    vector<vector<Dir>> energized;
    queue<BeamStep> queue;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);
        room.push_back(line);
        energized.push_back(vector<Dir>(line.length()));
    }

    queue.push(BeamStep{ 0, 0, 'r' });

    while (!queue.empty())
    {
        auto step = queue.front(); queue.pop();

        // Ignore if it would go off of the grid
        if (step.x < 0 || step.x >= room.size() || step.y < 0 || step.y >= room[0].size()) continue;

        // Ignore if already check this dir, otherwise mark
        switch (step.dir)
        {
            case 'u': if (energized[step.x][step.y].u) continue; energized[step.x][step.y].u = true; break;
            case 'd': if (energized[step.x][step.y].d) continue; energized[step.x][step.y].d = true; break;
            case 'l': if (energized[step.x][step.y].l) continue; energized[step.x][step.y].l = true; break;
            case 'r': if (energized[step.x][step.y].r) continue; energized[step.x][step.y].r = true; break;
        }

        int nextX = step.x, nextY = step.y;
        char nextDir = step.dir;
        switch (room[step.x][step.y])
        {
        emptySpace:
            case '.':
                switch (step.dir)
                {
                    case 'u': nextX -= 1; break;
                    case 'd': nextX += 1; break;
                    case 'l': nextY -= 1; break;
                    case 'r': nextY += 1; break;
                }
                queue.push(BeamStep{ nextX, nextY, step.dir });
                continue;

            case '\\':
                switch (step.dir)
                {
                    case 'u': nextDir = 'l'; nextY -= 1; break;
                    case 'd': nextDir = 'r'; nextY += 1; break;
                    case 'l': nextDir = 'u'; nextX -= 1; break;
                    case 'r': nextDir = 'd'; nextX += 1; break;
                }
                queue.push(BeamStep{ nextX, nextY, nextDir });
                continue;

            case '/':
                switch (step.dir)
                {
                    case 'u': nextDir = 'r'; nextY += 1; break;
                    case 'd': nextDir = 'l'; nextY -= 1; break;
                    case 'l': nextDir = 'd'; nextX += 1; break;
                    case 'r': nextDir = 'u'; nextX -= 1; break;
                }
                queue.push(BeamStep{ nextX, nextY, nextDir });
                continue;

            case '-':
                switch (step.dir)
                {
                    case 'u':
                    case 'd':
                        queue.push(BeamStep{ nextX, nextY-1, 'l' });
                        queue.push(BeamStep{ nextX, nextY+1, 'r' });
                        continue;
                    case 'l':
                    case 'r':
                        goto emptySpace;
                }

            case '|':
             switch (step.dir)
             {
                case 'u':
                case 'd':
                    goto emptySpace;
                case 'l':
                case 'r':
                    queue.push(BeamStep{ nextX-1, nextY, 'u' });
                    queue.push(BeamStep{ nextX+1, nextY, 'd' });
                    continue;
             }
        }
    }

    // Count energized tiles
    int count = 0;
    for (auto row: energized) {
        for (auto tile: row)
        {
            if (tile.energized()) count++;
        }
    }
    
    cout << endl << "Output: " << count << endl;
}
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

#define MAX_X 10
#define MAX_Y 10
#define MAX_Z 305

//#define MAX_X 3
//#define MAX_Y 3
//#define MAX_Z 10

using namespace std;

struct Coord { int x, y, z; };
struct Brick {
    int id;
    vector<Coord> spaces;
};

int grid[MAX_X][MAX_Y][MAX_Z];

vector<string> split(string s, string delim)
{
    vector<string> output;
    while (s.find(delim) != string::npos)
    {
        size_t pos = s.find(delim);
        output.push_back(s.substr(0, pos));
        s.erase(0, pos + delim.length());
    }
    output.push_back(s);
    return output;
}

int overSingle (Coord s) { return grid[s.x][s.y][s.z+1]; }
int underSingle(Coord s) { return grid[s.x][s.y][s.z-1]; }

Coord parseCoord(string s)
{
    auto p = split(s, ",");
    return Coord{stoi(p[0]), stoi(p[1]), stoi(p[2])};
}

bool canFall(Brick b)
{
    if (b.spaces[0].z == 0) return false;
    if (b.spaces.size() > 1 && b.spaces[0].z != b.spaces[1].z)
    {
        return underSingle(b.spaces[0]) < 0;
    }

    for (auto c : b.spaces) {
        if (underSingle(c) >= 0) {
            return false;
        }
    }

    return true;
}


set<int> supporting(Brick b)
{
    set<int> toReturn;
    if (b.spaces.size() > 1 && b.spaces[0].z != b.spaces[1].z)
    {
        int supporting = overSingle(b.spaces.back());
        if (supporting >= 0) toReturn.insert(supporting);
        return toReturn;
    }

    for (auto c : b.spaces) {
        int supporting = overSingle(c);
        if (supporting >= 0) toReturn.insert(supporting);
    }

    return toReturn;
}

set<int> supportedBy(Brick b)
{
    set<int> toReturn;
    if (b.spaces[0].z == 0) return toReturn;
    if (b.spaces.size() > 1 && b.spaces[0].z != b.spaces[1].z)
    {
        int supporting = underSingle(b.spaces[0]);
        if (supporting >= 0) toReturn.insert(supporting);
        return toReturn;
    }

    for (auto c : b.spaces) {
        int supporting = underSingle(c);
        if (supporting >= 0) toReturn.insert(supporting);
    }

    return toReturn;
}

void printGridX()
{
    for (int x = 0; x < MAX_X; x++) cout << x;
    cout << endl;
    
    for (int z = MAX_Z-1; z >= 0; z--)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            char out = '.';
            for (int y = 0; y < MAX_Y; y++)
            {
                if (grid[x][y][z] >= 0) out = 'A' + (grid[x][y][z] % 26);
            }
            cout << out;
        }
        cout << ' ' << z << endl;
    }
    cout << endl;
}

void printGridY()
{
    for (int y = 0; y < MAX_Y; y++) cout << y;
    cout << endl;
    
    for (int z = MAX_Z-1; z >= 0; z--)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            char out = '.';
            for (int x = 0; x < MAX_X; x++)
            {
                if (grid[x][y][z] >= 0) out = 'A' + (grid[x][y][z] % 26);
            }
            cout << out;
        }
        cout << ' ' << z << endl;
    }
    cout << endl;
}

int main()
{
    string line;
    ifstream input;
    vector<Brick> bricks, bricksCopy;

    for (int x = 0; x < MAX_X; x++) {
        for (int y = 0; y < MAX_Y; y++) {  
            for (int z = 0; z < MAX_Z; z++) {
                grid[x][y][z] = -1;
            }
        }
    }

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);
        vector<string> brickParts = split(line, "~");

        Coord start = parseCoord(brickParts[0]);
        Coord end = parseCoord(brickParts[1]);

        if (start.x > end.x || start.y > end.y || start.z > end.z) swap(start, end);

        Brick newBrick = Brick{ (int)bricks.size(), vector<Coord>() };
        for (int x = start.x; x <= end.x; x++) {
            for (int y = start.y; y <= end.y; y++) {
                for (int z = start.z; z <= end.z; z++) {
                    newBrick.spaces.push_back(Coord{x, y, z});
                    grid[x][y][z] = newBrick.id;
                }
            }
        }

        bricks.push_back(newBrick);
    }

    // Main falling loop
    bool movedBricks = true;
    while (movedBricks)
    { 
        movedBricks = false;

        for (int bi = 0; bi < bricks.size(); bi++)
        {
            if (canFall(bricks[bi]))
            {
                for (int i = 0; i < bricks[bi].spaces.size(); i++)
                {
                    auto c = bricks[bi].spaces[i];
                    grid[c.x][c.y][c.z] = -1;
                    grid[c.x][c.y][c.z-1] = bricks[bi].id;
                    bricks[bi].spaces[i].z--;
                }
                movedBricks = true;
            }
        }
    }

    // Check for disintegration
    set<int> shouldDestroy;
    for (auto brick: bricks)
    {
        bool canDestroy = false;
        auto supportedList = supporting(brick);

        for (auto supported : supportedList)
        {
            auto sby = supportedBy(bricks[supported]);
            if (sby.size() == 1)
            {
                shouldDestroy.insert(brick.id);
                break;
            }
        }
    }
    
    uint64_t totalFall = 0;
    for (int destroyId : shouldDestroy)
    {
        set<int> moved;
        queue<int> toCheck;

        moved.insert(destroyId);
        for (int supported : supporting(bricks[destroyId])) toCheck.push(supported);

        while (!toCheck.empty())
        {
            int checkId = toCheck.front(); toCheck.pop();
            
            auto belowToCheck = supportedBy(bricks[checkId]);
            int stillSupporting = belowToCheck.size();
            for (int checkBelow : belowToCheck)
            {
                if (moved.find(checkBelow) != moved.end()) stillSupporting--;
            }

            if (stillSupporting == 0)
            {
                moved.insert(checkId);
                for (int s : supporting(bricks[checkId])) toCheck.push(s);
            }
        }


        //cout << (char)('A' + destroyId % 26)<< " caused " << moved.size()-1 << " to fall" << endl;
        totalFall += moved.size() - 1;
    }
    
    cout << endl << "Output: " << totalFall << endl;
}
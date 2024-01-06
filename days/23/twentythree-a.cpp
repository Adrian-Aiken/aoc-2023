#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <map>

#define Coord pair<int, int>

using namespace std;

struct Path {
    vector<Coord> steps;
    Coord start;
    int x, y;
};
struct Node { int x, y, length; set<Coord> seen; };

bool compareNodes(Node left, Node right) { return left.x == right.x && left.y == right.y && left.length == right.length; }
bool sortNodes(Node left, Node right) { return left.x < right.x || left.y < right.y || left.length < right.length; }

void printTrail(vector<string> grounds, vector<Coord> maxSteps)
{
    for (int x = 0; x < grounds.size(); x++)
    {
        for (int y = 0; y < grounds.size(); y++)
        {
            if (find(maxSteps.begin(), maxSteps.end(), Coord{x, y}) != maxSteps.end())
            {
                cout << '*';
            } else {
                cout << grounds[x][y];
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool getPathCount(vector<string> grounds, int x, int y)
{
    if (x == 0 || x == grounds.size() - 1) return true;

    int pathCount = 0;
    
    if (grounds[x-1][y] != '#') pathCount++;
    if (grounds[x+1][y] != '#') pathCount++;
    if (grounds[x][y+1] != '#') pathCount++;
    if (grounds[x][y-1] != '#') pathCount++;

    return pathCount != 2;
}

int main()
{
    string line;
    ifstream input;
    vector<string> grounds;
    queue<Coord> intersecitons;
    set<Coord> seenIntersections;
    map<Coord, vector<Node>> segments;

    int maxLen = 0, finished = 0;

    input.open("input.txt");

    while (!input.eof())
    {
        getline(input, line);
        grounds.push_back(line);
    }

    intersecitons.push(Coord{0, 1});

    while (!intersecitons.empty())
    {
        auto intersect = intersecitons.front(); intersecitons.pop();

        if (seenIntersections.find(intersect) != seenIntersections.end()) continue;
        seenIntersections.insert(intersect);
        
        if (intersect.first == grounds.size() - 1) continue;

        queue<Path> routes;        
        Path firstPath{ vector<Coord>(), intersect, intersect.first, intersect.second };
        if (intersect.first == 0)
        {
            firstPath.steps.push_back(Coord{0, 1});
            firstPath.x = 1;
        }
        
        routes.push(firstPath);
    
        while (!routes.empty())
        {
            auto trail = routes.front(); routes.pop();

            trail.steps.push_back(Coord{trail.x, trail.y});

            if (getPathCount(grounds, trail.x, trail.y) && !(intersect.first == trail.x && intersect.second == trail.y))
            {
                segments[intersect].push_back(Node{trail.x, trail.y, (int)trail.steps.size()-1});
                intersecitons.push(Coord{trail.x, trail.y});
                continue;
            }

            // North
            if ((grounds[trail.x-1][trail.y] == '.' || grounds[trail.x-1][trail.y] == '^')
                && find(trail.steps.begin(), trail.steps.end(), Coord{trail.x-1, trail.y}) == trail.steps.end())
            {
                Path nextTrail { vector<Coord>(trail.steps), trail.start, trail.x-1, trail.y };
                if (grounds[trail.x-1][trail.y] == '^')
                {
                    nextTrail.steps.push_back(Coord{nextTrail.x, nextTrail.y});
                    nextTrail.x--;
                }
                routes.push(nextTrail);
            }

            // South
            if ((grounds[trail.x+1][trail.y] == '.' || grounds[trail.x+1][trail.y] == 'v')
                && find(trail.steps.begin(), trail.steps.end(), Coord{trail.x+1, trail.y}) == trail.steps.end())
            {
                Path nextTrail { vector<Coord>(trail.steps), trail.start, trail.x+1, trail.y };
                if (grounds[trail.x+1][trail.y] == 'v')
                {
                    nextTrail.steps.push_back(Coord{nextTrail.x, nextTrail.y});
                    nextTrail.x++;
                }
                routes.push(nextTrail);
            }

            // East
            if ((grounds[trail.x][trail.y+1] == '.' || grounds[trail.x][trail.y+1] == '>')
                && find(trail.steps.begin(), trail.steps.end(), Coord{trail.x, trail.y+1}) == trail.steps.end())
            {
                Path nextTrail { vector<Coord>(trail.steps), trail.start, trail.x, trail.y+1 };
                if (grounds[trail.x][trail.y+1] == '>')
                {
                    nextTrail.steps.push_back(Coord{nextTrail.x, nextTrail.y});
                    nextTrail.y++;
                }
                routes.push(nextTrail);
            }

            // West
            if ((grounds[trail.x][trail.y-1] == '.' || grounds[trail.x][trail.y-1] == '<')
                && find(trail.steps.begin(), trail.steps.end(), Coord{trail.x, trail.y-1}) == trail.steps.end())
            {
                Path nextTrail { vector<Coord>(trail.steps), trail.start, trail.x, trail.y-1 };
                if (grounds[trail.x][trail.y-1] == '>')
                {
                    nextTrail.steps.push_back(Coord{nextTrail.x, nextTrail.y});
                    nextTrail.y--;
                }
                routes.push(nextTrail);
            }
        }
    }

    // Proper Breadth First Search
    queue<Node> routeQueue;
    routeQueue.push(Node{0, 1, 0});
    while (!routeQueue.empty())
    {
        auto route = routeQueue.front(); routeQueue.pop();
        route.seen.insert(Coord(route.x, route.y));

        if (route.x == grounds.size() - 1)
        {
            maxLen = max(maxLen, route.length);
            finished++;
            continue;
        }

        for (auto m : segments[Coord{route.x, route.y}])
        {
            if (route.seen.find(Coord(m.x, m.y)) != route.seen.end()) continue;

            routeQueue.push(Node{m.x, m.y, route.length + m.length });
        }
    }

    cout << endl << "Output: " << maxLen << " (" << finished << " finished)" << endl;
}
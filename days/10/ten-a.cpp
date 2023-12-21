#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

string uChars = "|7F";
string dChars = "|LJ";
string lChars = "-LF";
string rChars = "-J7";

struct PipeSeg {
    int x, y, dist;
};

int main()
{
    string line;
    ifstream input;
    vector<string> feild;
    vector<vector<int>> distances;
    queue<PipeSeg> toSearch;
    int maxDist = -1;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);

        if (line.find('S') != string::npos)
        {
            toSearch.push(PipeSeg{(int)feild.size(), (int)line.find('S'), 0});
        }

        feild.push_back(line);
        distances.push_back(vector<int>(line.size(), -1));
    }

    while (!toSearch.empty())
    {
        PipeSeg pipeSeg = toSearch.front(); toSearch.pop();

        if (distances[pipeSeg.x][pipeSeg.y] >= 0) continue;

        distances[pipeSeg.x][pipeSeg.y] = pipeSeg.dist;
        maxDist = max(pipeSeg.dist, maxDist);

        // Up
        if (pipeSeg.x > 0 && uChars.find(feild[pipeSeg.x-1][pipeSeg.y]) != string::npos)
            toSearch.push(PipeSeg{pipeSeg.x-1, pipeSeg.y, pipeSeg.dist+1});
        
        // Down
        if (pipeSeg.x < feild.size()-1 && dChars.find(feild[pipeSeg.x+1][pipeSeg.y]) != string::npos)
            toSearch.push(PipeSeg{pipeSeg.x+1, pipeSeg.y, pipeSeg.dist+1});

        // Left
        if (pipeSeg.y > 0 && lChars.find(feild[pipeSeg.x][pipeSeg.y-1]) != string::npos)
            toSearch.push(PipeSeg{pipeSeg.x, pipeSeg.y-1, pipeSeg.dist+1});

        // Right
        if (pipeSeg.y < feild[0].size()-1 && rChars.find(feild[pipeSeg.x][pipeSeg.y+1]) != string::npos)
            toSearch.push(PipeSeg{pipeSeg.x, pipeSeg.y+1, pipeSeg.dist+1});
    }

    for (int i = 0; i < distances.size(); i++)
    {
        for (int j = 0; j < distances[i].size(); j++)
        {
            if (distances[i][j] == -1) cout << ".";
            else cout << distances[i][j];
        }
        cout << endl;
    }

    cout << endl << "Max distance: " << maxDist;
}
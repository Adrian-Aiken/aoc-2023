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

string emptyChars = ".@";

vector<string> feild;

struct PipeSeg {
    int x, y, dist;
};

bool isEmpty(int x, int y)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (feild[x+i][y+j] != '.') return false; 
        }
    }

    return true;
}

int main()
{
    string line;
    ifstream input;
    vector<vector<int>> distances;
    queue<PipeSeg> toSearch;

    input.open("sample.txt");

    // Parse
    while (!input.eof())
    {
        getline(input, line);

        if (line.find('S') != string::npos)
        {
            toSearch.push(PipeSeg{(int)feild.size() * 3, (int)line.find('S'), 0});
        }

        feild.push_back(line);
    }

    // Zoom in
    vector<string> bigFeild;
    for (int x = 0; x < feild.size(); x++)
    {
        string top, middle, bottom;
        for (int y = 0; y < feild[x].size(); y++)
        {
            switch(feild[x][y])
            {
                case '|': top += ".|."; middle += ".|."; bottom += ".|."; break;
                case '-': top += "..."; middle += "---"; bottom += "..."; break;
                case 'L': top += ".|."; middle += ".L-"; bottom += "..."; break;
                case 'J': top += ".|."; middle += "-J."; bottom += "..."; break;
                case '7': top += "..."; middle += "-7."; bottom += ".|."; break;
                case 'F': top += "..."; middle += ".F-"; bottom += ".|."; break;
                case '.': top += "..."; middle += "..."; bottom += "..."; break;
                case 'S': top += ".|."; middle += "-S-"; bottom += ".|."; break;
            }
        }
        bigFeild.push_back(top);
        bigFeild.push_back(middle);
        bigFeild.push_back(bottom);
        
        distances.push_back(vector<int>(top.size(), -1));
        distances.push_back(vector<int>(top.size(), -1));
        distances.push_back(vector<int>(top.size(), -1));
    }
    feild = bigFeild;

    // Find start
    for (int i = 0; i < feild.size(); i++)
    {
        for (int j = 0; j < feild[0].size(); j++)
        {
            if (feild[i][j] == 'S') toSearch.push(PipeSeg{i, j, 0});
        }
    }

    // Find loop
    while (!toSearch.empty())
    {
        PipeSeg pipeSeg = toSearch.front(); toSearch.pop();

        if (distances[pipeSeg.x][pipeSeg.y] >= 0) continue;

        distances[pipeSeg.x][pipeSeg.y] = pipeSeg.dist;

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

    // Clear extra pipes
    for (int x = 0; x < feild.size(); x++)
    {
        for (int y = 0; y < feild[x].size(); y++)
        {
            if (distances[x][y] == -1) feild[x][y] = '.';
        }
    }

    // Mark 'outside' tiles
    queue<pair<int, int>> outside;
    outside.push(pair<int, int>(0,0));
    while (!outside.empty())
    {
        auto c = outside.front(); outside.pop();

        if ((feild[c.first][c.second]) == ' ') continue;

        feild[c.first][c.second] = ' ';

        // Up
        if (c.first > 0 && emptyChars.find(feild[c.first-1][c.second]) != string::npos)
            outside.push(pair<int, int>(c.first-1, c.second));

        // Down
        if (c.first < feild.size()-1 && emptyChars.find(feild[c.first+1][c.second]) != string::npos)
            outside.push(pair<int, int>(c.first+1, c.second));

        // Left
        if (c.second > 0 && emptyChars.find(feild[c.first][c.second-1]) != string::npos)
            outside.push(pair<int, int>(c.first, c.second-1));

        // Right
        if (c.second < feild[0].size()-1 && emptyChars.find(feild[c.first][c.second+1]) != string::npos)
            outside.push(pair<int, int>(c.first, c.second+1));
    }

    // Final tally
    int count = 0;
    for (int i = 0; i < feild.size(); i += 3)
    {
        for (int j = 0; j < feild[i].size(); j += 3)
        {
            if (isEmpty(i, j)) count++;
        }
    }

    cout << endl << "Inner tiles: " << count << endl;
}
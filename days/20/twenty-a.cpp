#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <sstream>

using namespace std;

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

enum ModuleType { Base, FlipFlop, Conjunction };

struct Pulse { string source, destination; bool isHigh; };
class Module
{
public:
    ModuleType type;

    bool isOn;    
    map<string, bool> lastPulse;
    vector<string> destinations;

    Module(){}
    Module(vector<string> destinations)
    {
        type = Base;
        this->destinations = destinations;
    }

    vector<Pulse> getNextDestinations(Pulse p)
    {
        switch(type)
        {
            case FlipFlop: return getFlipFlopDestinations(p);
            case Conjunction: return getConjunctionDestinations(p);
            default: return getBaseDestinations(p);
        }
    }

private:
    vector<Pulse> getBaseDestinations(Pulse p)
    {
        vector<Pulse> toReturn;
        for (auto d : destinations)
        {
            toReturn.push_back(Pulse{ p.destination, d, p.isHigh });
        }
        return toReturn;
    }
    
    vector<Pulse> getFlipFlopDestinations(Pulse p)
    {
        vector<Pulse> toReturn;
        if (p.isHigh) return toReturn;

        isOn = !isOn;
        for (auto d : destinations)
        {
            toReturn.push_back(Pulse{ p.destination, d, isOn });
        }
        return toReturn;
    }

    vector<Pulse> getConjunctionDestinations(Pulse p)
    {
        vector<Pulse> toReturn;
        lastPulse[p.source] = p.isHigh;

        bool allHigh = true;
        for (auto d : lastPulse) if (!d.second) allHigh = false;

        for (auto d : destinations)
        {
            toReturn.push_back(Pulse{ p.destination, d, !allHigh });
        }
        return toReturn;
    }
};

string hashState(map<string, Module> network)
{
    auto ss = stringstream();

    for (auto p : network)
    {
        ss << ';' << p.first;
        switch (p.second.type)
        {
            case FlipFlop: ss << (p.second.isOn ? '1' : '0'); break;
            case Conjunction:
                for (auto cstate : p.second.lastPulse)
                {
                    ss << (cstate.second ? '1' : '0');
                }
                break;
            default: break;
        }
    }

    return ss.str();
}

int main()
{
    string line;
    ifstream input;
    map<string, Module> network;
    map<string, Module> allConj;

    input.open("sample.txt");

    while (!input.eof())
    {
        getline(input, line);
        auto parts = split(line, " -> ");
        auto destinations = split(parts[1], ", ");

        Module newModule = Module(destinations);
        switch (parts[0][0])
        {
            case '%':
                newModule.type = FlipFlop;
                newModule.isOn = false;             
                network[parts[0].substr(1)] = newModule;
                break;

            case '&':
                newModule.type = Conjunction;
                newModule.lastPulse = map<string, bool>();
                network[parts[0].substr(1)] = newModule;
                allConj[parts[0].substr(1)] = newModule;
                break;

            default:
                network[parts[0]] = newModule;
                break;
        }
    }

    // Pass to mark Conjunction inputs
    for (auto p : network)
    {
        for (auto d : p.second.destinations)
        {
            if (allConj.find(d) != allConj.end())
            {
                network[d].lastPulse[p.first] = false;
            }
        }
    }

    // Queue through system
    uint64_t highPulses = 0, lowPulses = 0;
    for (int i = 0; i < 1000; i++) {
        queue<Pulse> pulses;
        pulses.push(Pulse{ "button", "broadcaster", false });
        while (!pulses.empty())
        {
            auto pulse = pulses.front(); pulses.pop();
            if (pulse.isHigh) highPulses++;
            else lowPulses++;

            auto newPulses = network[pulse.destination].getNextDestinations(pulse);
            for (auto p : newPulses) pulses.push(p);
        }
    }
    
    cout << endl << "Output: " << highPulses * lowPulses << " (low: " << lowPulses << ", high: " << highPulses << ")" << endl;
}
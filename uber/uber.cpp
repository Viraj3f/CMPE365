#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>

#include "CityGraph.hpp"

using std::vector;
using std::string;
using std::cout;
using std::endl;


struct Passenger
{
    int requestTime;
    int startLocation;
    int endLocation;
};

class RequestParser
{
    std::fstream fs;

  public:
    RequestParser(const string &filename) :
        fs(filename)
    {};

    bool parseNext(Passenger& p)
    {
        string line;
        if (!std::getline(fs, line))
        {
            return false;
        }

        std::stringstream ss(line);
        std::getline(ss, line, ',');
        p.requestTime = std::stoi(line);
        std::getline(ss, line, ',');
        p.startLocation = std::stoi(line);
        std::getline(ss, line, ',');
        p.endLocation = std::stoi(line);

        return true;
    }

    void reset()
    {
        fs.seekg(0);
    }
};


int main()
{
    CityGraph<49> graph;
    graph.parseCSV("./data/network.csv");

    //RequestParser requestParser("./requests.csv");
    //graph.getShortestPathsFrom(1);

    /* Passenger p; */
    /* while (requestParser.parseNext(p)) */
    /* { */
    /*     cout << p.requestTime << " " << p.startLocation << " " << p.endLocation << endl; */
    /* } */

    return 0;
}

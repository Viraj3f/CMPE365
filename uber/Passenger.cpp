#include <fstream>

#include "Passenger.hpp"

std::ostream& operator<<(std::ostream& o, Passenger& p)
{
    o << "Id:" << p.id << " ";
    o << "Time:" << p.requestTime << " ";
    o << "Start:" << p.startLocation << " ";
    o << "End:" << p.endLocation;
    return o;
}

void parseRequests(const string& filename, std::queue<Passenger>& q)
{
    std::fstream fs(filename);

    int counter = 0;
    string line;
    while (std::getline(fs, line))
    {
        std::stringstream ss(line);

        std::getline(ss, line, ',');
        int requestTime = std::stoi(line);

        std::getline(ss, line, ',');
        int startLocation = std::stoi(line);

        std::getline(ss, line, ',');
        int endLocation = std::stoi(line);

        q.push(Passenger{requestTime, startLocation, endLocation, counter++});
    }
}

#include <iostream>
#include <queue>
#include <set>
#include <cstring>
#include <assert.h>

#include "CityGraph.hpp"
#include "Driver.hpp"
#include "Passenger.hpp"

using std::string;
using std::cout;
using std::endl;

struct Settings
{
    int numDrivers = 1;
    int maxCommittedTrips = 0;
    bool verbose = true;
};

void argParse(Settings& s, int argc, char** argv)
{
    for (int i = 1; i < argc; i += 2)
    {
        string arg(argv[i]);
        if (arg == "-v")
        {
            int p = atoi(argv[i + 1]);
            s.verbose = p == 1;
        }
        else if (arg == "-n")
        {
            int p = atoi(argv[i + 1]);
            assert(p > 0);
            s.numDrivers = p;
            cout << "Setting num drivers to " << p << endl;
        }
        else if (arg == "-m")
        {
            int p = atoi(argv[i + 1]);
            assert(p >= 0);
            s.maxCommittedTrips = p;
            cout << "Setting max commited trips to " << p << endl;
        }
        else
        {
            cout << "Invalid argument:" << arg << endl;
            exit(-1);
        }
    }
}

int main(int argc, char** argv)
{
    // Parse arguments
    Settings settings;
    argParse(settings, argc, argv);

    // Create graph from parsing network.
    const unsigned int N = 50;
    CityGraph<N> graph;
    graph.parseCSV("./data/network.csv");

    // Parse the passengers.
    std::queue<Passenger> passengers;
    parseRequests("./data/supplementpickups.csv", passengers);

    vector<Driver> drivers;
    for (int i = 0; i < settings.numDrivers; i++)
    {
        drivers.push_back(Driver(i % N));
    }

    int totalWaitingTime = 0;
    while (!passengers.empty())
    {
        Passenger p = passengers.front();
        passengers.pop();

        int minWaitingTime = 1e9;
        int bestTimeToPickup = 1e9;
        Driver* bestDriver = nullptr;
        for (Driver& d : drivers)
        {
            int timeToPickup = graph.shortestDistancesFrom(d.currentNode)[p.startLocation];
            int waitingTime = d.currentTime + timeToPickup > p.requestTime ?
                (d.currentTime + timeToPickup) - p.requestTime : 0;

            if (waitingTime < minWaitingTime)
            {
                bestDriver = &d;
                bestTimeToPickup = timeToPickup;
                minWaitingTime = waitingTime;
            }
        }

        // The distance to the dropoff.
        int timeToDropoff = graph.shortestDistancesFrom(p.startLocation)[p.endLocation];

        // If an uber arrives to early, it needs to "stall" until the passenger is ready.
        int stallTime = std::max(0, p.requestTime - (bestDriver->currentTime + bestTimeToPickup));

        // Update the best driver.
        bestDriver->currentTime += stallTime + bestTimeToPickup + timeToDropoff;
        bestDriver->currentNode = p.endLocation;

        // Keep track of how long the passenger was waiting for.
        totalWaitingTime += minWaitingTime;
    }

    cout << "Total waiting time: " << totalWaitingTime << endl;


    return 0;
}

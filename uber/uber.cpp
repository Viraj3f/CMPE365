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

    // Parse future requests into queue
    std::queue<Passenger> futurePassengers;
    std::queue<Passenger> waitingPassengers;
    parseRequests("./data/supplementpickups.csv", futurePassengers);

    // Create drivers and free drivers list.
    int maxCommittedTrips = settings.maxCommittedTrips;
    vector<Driver> drivers;
    for (int i = 0; i < settings.numDrivers; i++)
    {
        drivers.push_back(Driver(i % N));
    }

    std::set<Driver*> freeDrivers;  // unorderded set is faster, but set is more deterministic.
    for (int i = 0; i < settings.numDrivers; i++)
    {
        freeDrivers.insert(&drivers[i]);
    }

    // Start simulations
    const int maxTime = 1500;
    int totalWaitingTime = 0;
    for (int time = 0; time < maxTime; time++)
    {
        if (futurePassengers.empty() && waitingPassengers.empty())
        {
            break;
        }

        if (settings.verbose)
        {
            cout << "t = " << time << endl;
        }

        while (futurePassengers.size() > 0 &&
               futurePassengers.front().requestTime == time)
        {
            Passenger& p = futurePassengers.front();
            waitingPassengers.push(p);
            if (settings.verbose)
            {
                cout << "Request => " << p << endl;
            }
            futurePassengers.pop();
        }

        while (waitingPassengers.size() > 0 && freeDrivers.size() > 0)
        {
            Passenger& waiting = waitingPassengers.front();
            waitingPassengers.pop();

            // Get distance from startLocation to endLocation
            int distToDropoff = graph.shortestDistancesFrom(waiting.startLocation)[waiting.endLocation];

            int minTotalDist = 1e9;
            int minDistToPickup = 1e9;
            Driver* assignedDriver = nullptr;
            for (Driver* d : freeDrivers)
            {
                int distToPickup =  d->trips.empty() ?
                    graph.shortestDistancesFrom(d->currentNode)[waiting.startLocation] :
                    graph.shortestDistancesFrom(d->trips.back().endLocation)[waiting.startLocation];

                if (distToPickup + d->committedTripLength < minTotalDist)
                {
                    minTotalDist = distToPickup + d->committedTripLength;
                    minDistToPickup = distToPickup;
                    assignedDriver = d;
                }
            }


            if (settings.verbose)
            {
                cout << "Assigning driver " << assignedDriver->id << " to ";
                cout << "passenger " << waiting.id << " with pickup dist ";
                cout << minDistToPickup << " and dropoff dist " << distToDropoff << endl;
            }

            assignedDriver->addPickup(waiting, minDistToPickup);
            assignedDriver->addDropoff(waiting, distToDropoff);
            if (assignedDriver->trips.size() > maxCommittedTrips)
            {
                freeDrivers.erase(assignedDriver);
            }
        }

        for (Driver& d : drivers)
        {
            Trip t;
            if (d.step(t))
            {
                if (t.type == Pickup)
                {
                    int waitingTime = time - t.passenger.requestTime;
                    if (settings.verbose)
                    {
                        cout << "Driver " << d.id << " picked up passenger " << t.passenger.id;
                        cout << " who was waiting for " << waitingTime << " time." << endl;
                    }
                    totalWaitingTime += waitingTime;
                }
                else if (settings.verbose)
                {
                    cout << "Driver " << d.id << " dropped off passenger " << t.passenger.id;
                    cout << "." << endl;
                }
            }

            if (d.trips.size() <= maxCommittedTrips && freeDrivers.count(&d) == 0)
            {
                freeDrivers.insert(&d);
            }
        }
    }

    if (settings.verbose)
    {
        cout << "Total waiting time: " << totalWaitingTime << endl;
    }
    else
    {
        cout << "Total waiting time: " << totalWaitingTime << endl;
    }


    return 0;
}

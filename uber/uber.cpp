#include <iostream>
#include <queue>
#include <unordered_set>

#include "CityGraph.hpp"
#include "Driver.hpp"
#include "Passenger.hpp"

using std::string;
using std::cout;
using std::endl;

int main()
{
    // Create graph from parsing network.
    const unsigned int N = 50;
    CityGraph<N> graph;
    graph.parseCSV("./data/network.csv");

    // Parse future requests into queue
    std::queue<Passenger> futurePassengers;
    std::queue<Passenger> waitingPassengers;
    parseRequests("./data/requests.csv", futurePassengers);


    // Create drivers and free drivers list.
    const int maxComittedTrips = 0;
    vector<Driver> drivers{ Driver(0) };
    std::unordered_set<Driver*> freeDrivers;
    for (Driver& d : drivers)
    {
        freeDrivers.insert(&d);
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

        cout << "t = " << time << endl;
        while (futurePassengers.size() > 0 &&
               futurePassengers.front().requestTime == time)
        {
            Passenger& p = futurePassengers.front();
            waitingPassengers.push(p);
            cout << "Request => " << p << endl;
            futurePassengers.pop();
        }

        while (waitingPassengers.size() > 0 && freeDrivers.size() > 0)
        {
            Passenger& waiting = waitingPassengers.front();
            waitingPassengers.pop();

            // Get distance from startLocation to endLocation
            int distToDropoff = graph.shortestDistancesFrom(waiting.startLocation)[waiting.endLocation];

            int minDistToPickup = 1e9;
            Driver* assignedDriver = nullptr;
            for (Driver* d : freeDrivers)
            {
                int distToPickup =  d->trips.empty() ?
                    graph.shortestDistancesFrom(d->currentNode)[waiting.startLocation] :
                    graph.shortestDistancesFrom(d->trips.back().endLocation)[waiting.startLocation];

                if (distToPickup < minDistToPickup)
                {
                    minDistToPickup = distToPickup;
                    assignedDriver = d;
                }
            }

            cout << "Assigning driver " << assignedDriver->id << " to ";
            cout << "passenger " << waiting.id << " with pickup dist ";
            cout << minDistToPickup << " and dropoff dist " << distToDropoff << endl;

            assignedDriver->addPickup(waiting, minDistToPickup);
            assignedDriver->addDropoff(waiting, distToDropoff);
            if (assignedDriver->trips.size() > maxComittedTrips)
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
                    cout << "Driver " << d.id << " picked up passenger " << t.passenger.id;
                    cout << " who was waiting for " << waitingTime << " time." << endl;
                    totalWaitingTime += waitingTime;
                }
                else
                {
                    cout << "Driver " << d.id << " dropped off passenger " << t.passenger.id;
                    cout << "." << endl;
                }
            }

            if (d.trips.size() <= maxComittedTrips && freeDrivers.count(&d) == 0)
            {
                freeDrivers.insert(&d);
            }
        }
    }

    cout << "Total waiting time: " << totalWaitingTime << endl;

    return 0;
}

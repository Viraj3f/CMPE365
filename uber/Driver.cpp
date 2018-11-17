#include <iostream>

#include "Driver.hpp"

using std::cout;
using std::endl;

static int driverId = 0;

Driver::Driver(int startNode)
{
    id = driverId++;
    currentNode = startNode;
    committedTripLength = 0;
    timeSpentOnCurrentTrip = 0;
}

void Driver::addPickup(Passenger& p, int distance)
{
    trips.push({Pickup, distance, p, p.startLocation});
    committedTripLength += distance;
}

void Driver::addDropoff(Passenger& p, int distance)
{
    trips.push({Dropoff, distance, p, p.endLocation});
    committedTripLength += distance;
}

bool Driver::step(Trip& t)
{
    if (trips.empty())
    {
        return false;
    }
    else if (timeSpentOnCurrentTrip < trips.front().distance)
    {
        timeSpentOnCurrentTrip++;
        return false;
    }
    else
    {
        t = trips.front();
        currentNode = trips.front().endLocation;
        timeSpentOnCurrentTrip = t.type == Pickup ? 1 : 0;
        committedTripLength -= t.distance;
        trips.pop();

        return true;
    }
}

#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <vector>
#include <queue>
#include "Passenger.hpp"

using std::vector;

enum TripType
{
    Pickup,
    Dropoff
};

struct Trip
{
    TripType type;
    int distance;
    Passenger passenger;
    int endLocation;
};

class Driver
{
    private:
    int timeSpentOnCurrentTrip;

    public:
    int id;
    int currentNode;
    std::queue<Trip> trips;
    Driver(int startNode);

    void addPickup(Passenger& p, int distance);
    void addDropoff(Passenger& p, int distance);

    /*
     * Steps one moment in time. Returns 0 if a driver is in the middle of a 
     * trip, or is waiting for a trip. If the driver just picked up a passenger,
     * it will return the time the passenger requested to be picked up.
     */
    bool step(Trip& t);
};

#endif

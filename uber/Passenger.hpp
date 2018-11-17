#ifndef PASSENGER_HPP
#define PASSENGER_HPP

#include <iostream>
#include <queue>
#include <string>
#include <sstream>

using std::string;

struct Passenger
{
    int requestTime;
    int startLocation;
    int endLocation;
    int id;
};

void parseRequests(const string& filename, std::queue<Passenger>& q);
std::ostream& operator<<(std::ostream& o, Passenger& p);

#endif

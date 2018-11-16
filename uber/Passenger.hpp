#ifndef PASSENGER_HPP
#define PASSENGER_HPP

#include <iostream>
#include <queue>
#include <string>
#include <sstream>

using std::string;

struct Passenger
{
    int id;
    int requestTime;
    int startLocation;
    int endLocation;
};

void parseRequests(const string& filename, std::queue<Passenger>& q);

#endif

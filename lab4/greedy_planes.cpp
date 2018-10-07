#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <assert.h>
#include <fstream>

using std::cout;
using std::endl;
using std::vector;


struct PlaneEntry
{
    double startTime;
    double endTime;
};

int numberOfGatesNeeded(vector<PlaneEntry>& planesEntries)
{
    // Sort planes by start time
    std::sort(planesEntries.begin(), planesEntries.end(),
    [](PlaneEntry& a, PlaneEntry& b) -> bool
    {
        return a.startTime < b.startTime;
    });


    // PQ used as a min heap based on the plane entry's end time.
    auto cmp = [](PlaneEntry*a, PlaneEntry*b) { return  a->endTime > b->endTime; };
    std::priority_queue
        <PlaneEntry*,
        std::vector<PlaneEntry*>,
        std::function<bool(PlaneEntry*, PlaneEntry*)>> pq(cmp);

    int numberOfGates = 0;
    for (PlaneEntry& pe : planesEntries)
    {
        if (!pq.empty() && pq.top()->endTime < pe.startTime)
        {
            pq.pop();
        }
        pq.push(&pe);
        numberOfGates = std::max(numberOfGates, (int)pq.size());
    }

    return numberOfGates;
}

void testInputs()
{
    vector<PlaneEntry> v1{{0, 1}, {1.1, 2}, {2.1, 3}};
    int res = numberOfGatesNeeded(v1);
    assert(res == 1);

    vector<PlaneEntry> v2{{0, 1}, {0.5, 2}, {2.1, 3}};
    res = numberOfGatesNeeded(v2);
    assert(res == 2);

    vector<PlaneEntry> v3{{0, 5}, {0, 5}, {0, 5}};
    res = numberOfGatesNeeded(v3);
    assert(res == 3);

    vector<PlaneEntry> v4{{0, 10}, {2, 3.1}, {3, 3.3}, {4, 5}};
    res = numberOfGatesNeeded(v4);
    assert(res == 3);

    vector<PlaneEntry> v5{{0, 10}, {2, 10}, {3, 10}, {4, 10}};
    res = numberOfGatesNeeded(v5);
    assert(res == 4);
}


void part1FirstInputs()
{
    std::ifstream start("start1.csv");
    std::ifstream end("finish1.csv");
    assert(start.is_open() && end.is_open());

    std::vector<PlaneEntry> pe;
    while (!start.eof())
    {
        float startTime;
        float endTime;
        start >> startTime;
        end >> endTime;
        pe.push_back({startTime, endTime});
    }

    start.close();
    end.close();

    cout << numberOfGatesNeeded(pe) << endl;
}

void part1SecondInputs()
{
    std::ifstream start("start2.csv");
    std::ifstream end("finish2.csv");
    assert(start.is_open() && end.is_open());

    std::vector<PlaneEntry> pe;
    while (!start.eof())
    {
        float startTime;
        float endTime;
        start >> startTime;
        end >> endTime;
        pe.push_back({startTime, endTime});
    }

    start.close();
    end.close();

    cout << numberOfGatesNeeded(pe) << endl;
}


int main()
{
    testInputs();
    part1FirstInputs();
    part1SecondInputs();
}

#include <iostream>
#include <vector>
#include <queue>
#include <assert.h>
#include <fstream>
#include <random>


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

void loadFileIntoVector(int i, vector<PlaneEntry>& pe)
{
    std::ifstream start("start" + std::to_string(i) + ".csv");
    std::ifstream end("finish" + std::to_string(i) + ".csv");
    assert(start.is_open() && end.is_open());

    while (!start.eof())
    {
        double startTime;
        double endTime;
        start >> startTime;
        end >> endTime;
        pe.push_back({startTime, endTime});
    }
}


void part1()
{
    for (int i = 1; i <= 2; i++)
    {
        std::vector<PlaneEntry> pe;
        loadFileIntoVector(i, pe);
        cout << "Input " << i << ":" << endl;
        cout << numberOfGatesNeeded(pe) << endl;
    }
}

void part2Early(int i)
{
    // Earliest possible departure strategy
    vector<PlaneEntry> pe;
    loadFileIntoVector(i, pe);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> lateProbability(0, 1);

    cout << "percentLate,maxLateness,gatesNeeded" << endl;
    for (double percentLate = 0; percentLate <= 1; percentLate += 0.1)
    {
        for (double maxLateness = 0; maxLateness <= 6; maxLateness += 0.15)
        {
            std::uniform_real_distribution<double> delayTime(0, maxLateness);
            vector<PlaneEntry> testData;
            for (PlaneEntry& p : pe)
            {
                testData.push_back(p);
                if (lateProbability(generator) < percentLate)
                {
                    double delay = delayTime(generator);
                    PlaneEntry& plane = testData.back();
                    plane.startTime += delay;
                    if (plane.startTime > plane.endTime)
                    {
                        plane.endTime = plane.startTime + 0.25;
                    }
                }
            }

            int gatesNeeded = numberOfGatesNeeded(testData);
            cout << percentLate << "," << maxLateness << "," << gatesNeeded << endl;
        }
    }
}


void part2Punish(int i)
{
    // Earliest possible departure strategy
    vector<PlaneEntry> pe;
    loadFileIntoVector(i, pe);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> lateProbability(0, 1);

    cout << "percentLate,maxLateness,gatesNeeded" << endl;
    for (double percentLate = 0; percentLate <= 1; percentLate += 0.1)
    {
        for (double maxLateness = 0; maxLateness <= 6; maxLateness += 0.15)
        {
            std::uniform_real_distribution<double> delayTime(0, maxLateness);
            vector<PlaneEntry> testData;
            for (PlaneEntry& p : pe)
            {
                testData.push_back(p);
                if (lateProbability(generator) < percentLate)
                {
                    double delay = delayTime(generator);
                    PlaneEntry& plane = testData.back();
                    plane.startTime += delay;
                    plane.endTime += delay;
                }
            }

            int gatesNeeded = numberOfGatesNeeded(testData);
            cout << percentLate << "," << maxLateness << "," << gatesNeeded << endl;
        }
    }
}

void part2Hybrid(int i)
{
    // Earliest possible departure strategy
    vector<PlaneEntry> pe;
    loadFileIntoVector(i, pe);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> lateProbability(0, 1);

    cout << "percentLate,maxLateness,gatesNeeded" << endl;
    for (double percentLate = 0; percentLate <= 1; percentLate += 0.1)
    {
        for (double maxLateness = 0; maxLateness <= 6; maxLateness += 0.15)
        {
            std::uniform_real_distribution<double> delayTime(0, maxLateness);
            vector<PlaneEntry> testData;
            for (PlaneEntry& p : pe)
            {
                testData.push_back(p);
                if (lateProbability(generator) < percentLate)
                {
                    double delay = delayTime(generator);
                    PlaneEntry& plane = testData.back();
                    plane.startTime += delay;
                    if (plane.startTime > plane.endTime)
                    {
                        plane.endTime = plane.startTime + delay;
                    }
                }
            }

            int gatesNeeded = numberOfGatesNeeded(testData);
            cout << percentLate << "," << maxLateness << "," << gatesNeeded << endl;
        }
    }
}

int main()
{
    //testInputs();
    part1();
    //part2Early(2);
    //part2Punish(2);
    //part2Hybrid(2);
}

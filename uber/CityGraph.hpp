#ifndef CITYGRAPH_H
#define CITYGRAPH_H

#include <vector>
#include <queue>
#include <string>
#include <fstream>

using std::vector;
using std::string;

struct NodeDist
{
    int node;
    int dist;

    bool operator<(const NodeDist& other) const;
};


template<unsigned int N>
class CityGraph
{
  private:
    vector<vector<int> > matrix;
    vector<int>* cachedDistances[N];
    vector<int>* cachedPaths[N];

    /*
     * Generates shortest distances and paths from a starting
     * node using dijkstra's algorithm
     */
    void generateDistancesAndPathsFrom(int source);

  public:
    CityGraph();
    ~CityGraph();

    /*
     * Parses the CSV file and loads the graph into memory
     */
    void parseCSV(const string &filename);

    /*
     * Return a pointer to the shortest distances from a starting node.
     */
    const vector<int> * shortestDistancesFrom(int source);

    /*
     * Return a pointer to the shortest paths from a starting node.
     */
    const vector<int> * shortestPathsFrom(int source);
};

#endif

#ifndef CITYGRAPH_HPP
#define CITYGRAPH_HPP

#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <cstring>
#include <iostream>

using std::vector;
using std::string;
using std::cerr;
using std::endl;
using std::cout;


template<unsigned int N>
class CityGraph
{
    private:
    vector<vector<int> > matrix;
    vector<int>* cachedDistances[N];
    vector<int>* cachedPaths[N];

    /*
     * Temporary helper object for dijkstra's algorithm
     */
    struct NodeDist
    {
        int node;
        int dist;

        bool operator<(const NodeDist& other) const
        {
            return dist > other.dist;
        }
    };

    /*
     * Generates shortest distances and paths from a starting
     * node using dijkstra's algorithm.
     */
    void generateDistancesAndPathsFrom(int source)
    {
        // Check if this path has been computed before.
        if (cachedPaths[source] != nullptr &&
                cachedDistances[source] != nullptr)
        {
            return;
        }

        vector<int>* dist = new vector<int>(N, 1e6);
        vector<int>* prev = new vector<int>(N, -1);
        vector<bool> visited(N, false);

        (*dist)[source] = 0;
        visited[source] = true;

        std::priority_queue<NodeDist> pq;
        pq.push({source, (*dist)[source]});

        while (!pq.empty())
        {
            NodeDist n = pq.top();
            pq.pop();
            int u = n.node;
            for (int v = 0; v < N; v++)
            {
                if (matrix[u][v] != 0)
                {
                    int alt = (*dist)[u] + matrix[u][v];
                    if (alt < (*dist)[v])
                    {
                        (*dist)[v] = alt;
                        (*prev)[v] = u;

                        pq.push({v, alt});
                        visited[v] = true;
                    }
                }
            }
        }

        // Sanity checking. These should never happen.
        if (cachedPaths[source] != nullptr)
        {
            cerr << "Warning: An existing cached path was deleted." << endl;
            delete cachedPaths[source];
        }

        if (cachedDistances[source] != nullptr)
        {
            cerr << "Warning: An existing cached distance was deleted." << endl;
            delete cachedDistances[source];
        }

        cachedPaths[source] = prev;
        cachedDistances[source] = dist;
    }

    public:
    CityGraph() :
        matrix(N, vector<int>(N, 0))
    { 
        for (int i = 0; i < N; i++)
        {
            cachedPaths[i] = nullptr;
            cachedDistances[i] = nullptr;
        }

        return;
    }

    ~CityGraph()
    {
        for (int i = 0; i < N; i++)
        {
            if (cachedDistances[i] != nullptr)
            {
                delete cachedDistances[i];
            }

            if (cachedPaths[i] != nullptr)
            {
                delete cachedPaths[i];
            }
        }
    }

    /*
     * Parses the CSV file and loads the graph into memory
     */
    void parseCSV(const string &filename)
    {
        std::fstream file(filename);

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                string parsedInt;
                std::getline(file, parsedInt, ',');
                matrix[i][j] = std::stoi(parsedInt);
            }
        }
    }

    /*
     * Return a pointer to the shortest distances from a starting node.
     */
    const vector<int>& shortestDistancesFrom(int source)
    {
        if (cachedDistances[source] == nullptr)
        {
            generateDistancesAndPathsFrom(source);
        }

        return *cachedDistances[source];
    }

    /*
     * Return a pointer to the shortest paths from a starting node.
     */
    const vector<int>& shortestPathsFrom(int source)
    {
        if (cachedPaths[source] == nullptr)
        {
            generateDistancesAndPathsFrom(source);
        }

        return *cachedPaths[source];
    }
};

#endif

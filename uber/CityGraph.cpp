#include <iostream>
#include "CityGraph.hpp"

using std::cerr;
using std::endl;

bool NodeDist::operator<(const NodeDist& other) const
{
    return dist > other.dist;
}

template<unsigned int N>
CityGraph<N>::CityGraph() :
    matrix(N, vector<int>(N, 0))
{ 
    memset(cachedDistances, nullptr, N);
    memset(cachedPaths, nullptr, N);

    return;
}

template<unsigned int N>
CityGraph<N>::~CityGraph()
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

template<unsigned int N>
void CityGraph<N>::parseCSV(const string &filename)
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

template<unsigned int N>
void CityGraph<N>::generateDistancesAndPathsFrom(int source)
{
    // Check if this path has been computed before.
    if (cachedPaths[source] != nullptr &&
        cachedDistances[source] != nullptr)
    {
        return;
    }

    vector<int>* dist = new vector<int>(N, INT_MAX);
    vector<int>* prev = new vector<int>(N, -1);
    (*dist)[source] = 0;

    std::priority_queue<NodeDist> pq;
    pq.push({source, (*dist)[source]});

    while (!pq.empty())
    {
        NodeDist n = pq.top();
        pq.pop();
        int u = n.node;
        for (int v = 0; v < N; v++)
        {
            if (v != u)
            {
                int alt = (*dist)[u] + matrix[u][v];
                if (alt < (*dist)[v])
                {
                    (*dist)[v] = alt;
                    (*prev)[v] = u;
                    pq.push({v, alt});
                }
            }
        }
    }

    // Sanity checking. These should never happen.
    if (cachedPaths[source] == nullptr)
    {
        cerr << "Warning: An existing cached path was deleted." << endl;
        delete cachedPaths[source];
    }
    
    if (cachedDistances[source] == nullptr)
    {
        cerr << "Warning: An existing cached distance was deleted." << endl;
        delete cachedDistances[source];
    }

    cachedPaths[source] = dist;
    cachedDistances[source] = prev;
}


template<unsigned int N>
const vector<int> * CityGraph<N>::shortestDistancesFrom(int source)
{
    if (cachedDistances[source] == nullptr)
    {
        generateDistancesAndPathsFrom(source);
    }

    return cachedDistances[source];
}

template<unsigned int N>
const vector<int> * CityGraph<N>::shortestPathsFrom(int source)
{
    if (cachedPaths[source] == nullptr)
    {
        generateDistancesAndPathsFrom(source);
    }

    return cachedPaths[source];
}

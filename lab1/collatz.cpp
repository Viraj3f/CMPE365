#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <numeric>
#include <array>
#include <random>
#include <algorithm>


using std::cout;
using std::endl;

// Stateful Collatz solver
class CollatzSolver
{
    private:
        // Previously solved collatz values for input n are stored in
        // element n - 1. Unsolved values are set to -1;
        const static int maxCachedElements = 262144;
        std::array<int, maxCachedElements> cachedSolutions;

        // Keeps track of the vised values which haven't been solved before when solving
        // the collatz problem.
        std::array <int, 500> visited;

    public:
        // The number of computed iterations gone through since this oject was created.
        int computedIterations;

        // Keeps track of the largest value when solving the collatz problem.
        int largestValue;

        CollatzSolver()
        {
            this->reset();
        }

        // Resets the previously solved solutions;
        void reset()
        {
            for (int i = 1; i < maxCachedElements; i++)
            {
                cachedSolutions[i] = -1;
            }

            // We know that for n = 1, collatz is already equal to 0.
            cachedSolutions[0] = 0;
            computedIterations = 0;
        }

        // Solve the collatz, with the only bounds checking being if n - 1 is greater
        // than the physical cache.
        void solve(int n)
        {
           int currentIteration = 0;
           while (cachedSolutions.at(n - 1) == -1)
           {
               visited[currentIteration++] = n;
               if ((n & 1) == 0)
               {
                   n /= 2;
               }
               else
               {
                   n = 3 * n + 1;
               }
           }

           int solution = cachedSolutions[n - 1]; // The value that was previously solved.

           // Traverse the visited paths in reverse, and add to the cache.
           for (int i = currentIteration - 1; i >= 0; i--)
           {
               int visitedValue = visited[i];
               cachedSolutions[visitedValue - 1] = ++solution;
           }

           computedIterations += currentIteration;
        }

        void solveWithUpperBound(int n, int upperBound)
        {
           int currentIteration = 0;
           while (n - 1 >= upperBound || cachedSolutions[n - 1] == -1)
           {
               visited[currentIteration++] = n;
               if ((n & 1) == 0)
               {
                   n /= 2;
               }
               else
               {
                   n = 3 * n + 1;
               }
           }

           int solution = cachedSolutions[n - 1]; // The value that was previously solved.

           // Traverse the visited paths in reverse, and add to the cache.
           for (int i = currentIteration - 1; i >= 0; i--)
           {
               int visitedValue = visited[i];
               if (visitedValue <= upperBound)
               {
                   cachedSolutions[visitedValue - 1] = ++solution;
               }
           }

           computedIterations += currentIteration;
        }

        void dump()
        {
            for (int i = 0; i < maxCachedElements; i++)
            {
                if (cachedSolutions[i] != -1)
                {
                    std::cout << i + 1 << " " << cachedSolutions[i] << std::endl;
                }
            }
        }

        void dumpTo(int n, std::string filename)
        {
            std::ofstream file(filename, std::ofstream::out);
            for (int i = 0; i < n; i++)
            {
                if (cachedSolutions[i] != -1)
                {
                    file << i + 1 << "," << cachedSolutions[i] << std::endl;
                }
            }
        }
};

void lab1Part1()
{
    CollatzSolver cs;

    for (int i = 1; i <= 1000; i++)
    {
        cs.solveWithUpperBound(i, 1000);
    }

    cs.dumpTo(1000, "part1.txt");
}

void lab1Part2()
{
    CollatzSolver cs;
    std::ofstream file("part2.txt", std::ofstream::out);

    // Solve in order
    for (int i = 1; i <= 1000; i++)
    {
        cs.solveWithUpperBound(i, 1000);
        file << cs.computedIterations << ",";
    }
    file << endl;
    cs.reset();

    // Solve in reverse order
    for (int i = 1000; i >= 1; i--)
    {
        cs.solveWithUpperBound(i, 1000);
        file << cs.computedIterations << ",";
    }
    file << endl;
    cs.reset();

    // Randomly solve
    std::vector<int> nValues(1000);
    std::iota(nValues.begin(), nValues.end(), 1);
    std::shuffle(nValues.begin(), nValues.end(), std::default_random_engine(0));
    for (int i = 0; i < 1000; i++)
    {
        cs.solveWithUpperBound(nValues[i], 1000);
        file << cs.computedIterations << ",";
    }
    file << endl;
}

void lab1Part3a()
{
    CollatzSolver cs;

    // Solve in order
    for (int i = 1; i <= 1000; i++)
    {
        cs.solve(i);
    }

    cs.dumpTo(262144, "part3a.txt");
}

void lab1Part3b()
{
    CollatzSolver cs;
    std::ofstream file("part3b.txt", std::ofstream::out);

    // Solve in order
    for (int i = 1; i <= 1000; i++)
    {
        cs.solve(i);
        file << cs.computedIterations << ",";
    }
    file << endl;
    cs.reset();

    // Solve in reverse order
    for (int i = 1000; i >= 1; i--)
    {
        cs.solve(i);
        file << cs.computedIterations << ",";
    }
    file << endl;
    cs.reset();

    // Randomly solve
    std::vector<int> nValues(1000);
    std::iota(nValues.begin(), nValues.end(), 1);
    std::shuffle(nValues.begin(), nValues.end(), std::default_random_engine(0));
    for (int i = 0; i < 1000; i++)
    {
        cs.solve(nValues[i]);
        file << cs.computedIterations << ",";
    }
    file << endl;
}

void lab1Part4()
{
}

int main()
{
    lab1Part3a();
    return 0;
}

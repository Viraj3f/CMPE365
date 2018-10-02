#include <iostream>
#include <random>
#include <stack>
#include <unordered_set>
#include <utility>

using std::cout;
using std::endl;
using std::vector;
using std::unordered_set;
using std::stack;
using std::string;

struct Point
{
    int x;
    int y;

    std::string hash()
    {
        return std::to_string(x) + "," + std::to_string(y);
    }
};

std::ostream& operator<<(std::ostream& io, Point& s)
{
    io << '{' << s.x << ", " << s.y << '}';
    return io;
}

class ConvexHullSolver
{
private:
    vector<Point>& points;
    unordered_set<string>& pointsInConvexHull;

    /*
     * For a line that passes through x and y, return 1 is z is 
     * above the line, -1 if it is below or 0 if it is tangent.
     */
    int getTangentDirection(Point& a, Point& b, Point& z)
    {
        int det = a.x * b.y - a.x * z.y
                - a.y * b.x + a.y * z.x
                + b.x * z.y - b.y * z.x;

        if (det == 0)
        {
            return det;
        }

        return det;
    }

    void rejoin(int first, int mid, int last)
    {
        // Check if points are either clockwise or anticlockwise of
        // the midpoint on the left.
        stack<Point*> aClockwise;
        stack<Point*> aAntiClockwise;
        for (int i = first; i < mid; i++)
        {
            if (pointsInConvexHull.count(points[i].hash()))
            {
                if (points[i].y <= points[mid].y)
                {
                    aClockwise.push(&points[i]);
                }
                else if (points[i].y > points[mid].y)
                {
                    aAntiClockwise.push(&points[i]);
                }
            }
        }

        // Check if points are either clockwise or anticlocksize of
        // the midpoint on the right.
        stack<Point*> bClockwise;
        stack<Point*> bAntiClockwise;
        for (int i = last; i > mid + 1; i--)
        {
            if (pointsInConvexHull.count(points[i].hash()))
            {
                if (points[i].y <= points[mid + 1].y)
                {
                    bAntiClockwise.push(&points[i]);
                }
                else if (points[i].y > points[mid + 1].y)
                {
                    bClockwise.push(&points[i]);
                }
            }
        }

        // Eliminate points between the middle and the lower tangent.
        Point* a = &points[mid];
        Point* aNext = aClockwise.empty() ? a : aClockwise.top();
        Point* b = &points[mid + 1];
        Point* bNext = bAntiClockwise.empty() ? b : bAntiClockwise.top();
        while (getTangentDirection(*a, *b, *aNext) > 0 ||
                getTangentDirection(*a, *b, *bNext) > 0)
        {
            while (getTangentDirection(*a, *b, *aNext) > 0)
            {
                pointsInConvexHull.erase(a->hash());
                a = aNext;
                if (aClockwise.size() > 1)
                {
                    aClockwise.pop();
                    aNext = aClockwise.top();
                }
            }

            while (getTangentDirection(*a, *b, *bNext) > 0)
            {
                pointsInConvexHull.erase(b->hash());
                b = bNext;
                if (bAntiClockwise.size() > 1)
                {
                    bAntiClockwise.pop();
                    bNext = bAntiClockwise.top();
                }
            }
        }

        // Eliminate points in the middle and upper tangent.
        a = &points[mid];
        aNext = aAntiClockwise.empty() ? a : aAntiClockwise.top();
        b = &points[mid + 1];
        bNext = bClockwise.empty() ? b : bClockwise.top();
        while (getTangentDirection(*a, *b, *aNext) < 0 ||
                getTangentDirection(*a, *b, *bNext) < 0)
        {
            while (getTangentDirection(*a, *b, *aNext) < 0)
            {
                pointsInConvexHull.erase(a->hash());
                a = aNext;
                if (aAntiClockwise.size() > 1)
                {
                    aAntiClockwise.pop();
                    aNext = aAntiClockwise.top();
                }
            }

            while (getTangentDirection(*a, *b, *bNext) < 0)
            {
                pointsInConvexHull.erase(b->hash());
                b = bNext;
                if (bClockwise.size() > 1)
                {
                    bClockwise.pop();
                    bNext = bClockwise.top();
                }
            }
        }
    }

public:
    ConvexHullSolver(
        vector<Point>& _points, 
        unordered_set<string>& _pointsInConvexHull) : 
            points(_points),
            pointsInConvexHull(_pointsInConvexHull) { }

    void getConvexHull(int first, int last)
    {
        // If there are 3 points or less, then we are guaranteed to be in a convex hull
        if (last - first + 1 <= 3)
        {
            for (int i = first; i < last; i++)
            {
                pointsInConvexHull.insert(points[i].hash());
            }
        }
        else
        {
            int mid = (first + last)/2;
            getConvexHull(first, mid);
            getConvexHull(mid + 1, last);
            rejoin(first, mid, last);
        }
    }
};


void uniformPointDistribution()
{
    const int maxNumberofPointsTogenerate = 1000;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(-100, 100);

    vector<Point> points(maxNumberofPointsTogenerate);
    for (int n = 999; n < maxNumberofPointsTogenerate; n += 10)
    {
        // Generate n points
        for (int i = 0; i < n; i++)
        {
            points[i].x = distribution(generator);
            points[i].y = distribution(generator);
        }

        // Sort points by x value
        std::sort(points.begin(), points.begin() + n,
        [](const Point &a, const Point &b) -> bool
        {
            return a.x < b.x;
        });

        unordered_set<string> pointsInConvexHull;
        ConvexHullSolver cs(points, pointsInConvexHull);
        cs.getConvexHull(0, n - 1);

        for (Point& p : points)
        {
            char flag = pointsInConvexHull.count(p.hash()) ? '1' : '0';
            cout << p.x << ',' << p.y << ',' << flag << endl;
        }
    }
}

int main()
{
    uniformPointDistribution();
    return 0;
}

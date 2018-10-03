#include <iostream>
#include <random>
#include <list>
#include <unordered_set>
#include <utility>

using std::cout;
using std::endl;
using std::vector;
using std::unordered_set;
using std::string;
using std::pair;

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

    std::pair<Point*, Point*> mergeCH(vector<Point*> aCH, vector<Point*> bCH, int direction)
    {

        cout << "ch points" << endl;
        for (Point* a : aCH)
        {
            cout << *a << ',';
        }
        cout << endl;

        for (Point* b : bCH)
        {
            cout << *b << ',';
        }
        cout << endl;


        int aIndex = 0, bIndex = 0;
        Point* a = aCH[aIndex];
        Point* aNext = aCH[(aIndex + 1) % aCH.size()];
        Point* b = bCH[bIndex];
        Point* bNext = bCH[(bIndex + 1) % bCH.size()];
        while (direction * getTangentDirection(*a, *b, *aNext) > 0 ||
                direction * getTangentDirection(*a, *b, *bNext) > 0)
        {
            while (direction * getTangentDirection(*a, *b, *aNext) > 0)
            {
                pointsInConvexHull.erase(a->hash());
                aIndex = (aIndex + 1) % aCH.size();
                a = aCH[aIndex];
                aNext = aCH[(aIndex + 1) % aCH.size()];
            }

            while (direction * getTangentDirection(*a, *b, *bNext) > 0)
            {
                pointsInConvexHull.erase(b->hash());
                bIndex = (bIndex + 1) % bCH.size();
                b = bCH[bIndex];
                bNext = bCH[(bIndex + 1) % bCH.size()];
            }
        }

        cout << "bounding pts" << endl;
        cout << *a << *b << endl;
        cout << endl;
        return std::pair<Point*, Point*>{a, b};
    }

    void rejoin(int first, int mid, int last)
    {
        // This creates two vector that gets the points in the convex
        // hull in clockwise and anticlockwise order. This can be done
        // more efficiently with a circular doubly linked list but stl
        // doesn't provide that and I'm too busy to do it myself :'(
        vector<Point*> aUpperHalf;
        vector<Point*> aLowerHalf;
        for (int i = mid - 1; i >= first; i--)
        {
            if (pointsInConvexHull.count(points[i].hash()))
            {
                if (points[i].y <= points[mid].y)
                {
                    aLowerHalf.push_back(&points[i]);
                }
                else if (points[i].y > points[mid].y)
                {
                    aUpperHalf.push_back(&points[i]);
                }
            }
        }
        vector<Point*> aClockwise;
        aClockwise.push_back(&points[mid]);
        aClockwise.insert(aClockwise.end(), aLowerHalf.begin(), aLowerHalf.end());
        aClockwise.insert(aClockwise.end(), aUpperHalf.rbegin(), aUpperHalf.rend());

        vector<Point*> aAntiClockwise;
        aAntiClockwise.push_back(&points[mid]);
        aAntiClockwise.insert(aAntiClockwise.end(), aUpperHalf.begin(), aUpperHalf.end());
        aAntiClockwise.insert(aAntiClockwise.end(), aLowerHalf.rbegin(), aLowerHalf.rend());

        // Repeat the same process for the b convex hull
        vector<Point*> bUpperHalf;
        vector<Point*> bLowerHalf;
        for (int i = mid + 2; i <= last; i++)
        {
            if (pointsInConvexHull.count(points[i].hash()))
            {
                if (points[i].y <= points[mid + 1].y)
                {
                    bLowerHalf.push_back(&points[i]);
                }
                else if (points[i].y > points[mid + 1].y)
                {
                    bUpperHalf.push_back(&points[i]);
                }
            }
        }

        vector<Point*> bAntiClockwise;
        bAntiClockwise.push_back(&points[mid + 1]);
        bAntiClockwise.insert(bAntiClockwise.end(), bLowerHalf.begin(), bLowerHalf.end());
        bAntiClockwise.insert(bAntiClockwise.end(), bUpperHalf.rbegin(), bUpperHalf.rend());

        vector<Point*> bClockwise;
        bClockwise.push_back(&points[mid + 1]);
        bClockwise.insert(bClockwise.end(), bUpperHalf.begin(), bUpperHalf.end());
        bClockwise.insert(bClockwise.end(), bLowerHalf.rbegin(), bLowerHalf.rend());
        
        cout << "First second" << endl;
        std::pair<Point*, Point*> bottomPoints = mergeCH(aClockwise, bAntiClockwise, -1);
        std::pair<Point*, Point*> upperPoints = mergeCH(aAntiClockwise, bClockwise, +1);

        pointsInConvexHull.insert(bottomPoints.first->hash());
        pointsInConvexHull.insert(bottomPoints.second->hash());
        pointsInConvexHull.insert(upperPoints.first->hash());
        pointsInConvexHull.insert(upperPoints.second->hash());
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
            for (int i = first; i <= last; i++)
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
    // Generate n points
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(-100, 100);
    const int n = 10;
    vector<Point> points(n);
    unordered_set<string> generated;
    for (int i = 0; i < n; i++)
    {
        do
        {
            points[i].x = distribution(generator);
            points[i].y = distribution(generator);
        } while (generated.find(points[i].hash()) != generated.end());
        generated.insert(points[i].hash());
    }

    // Sort points by x value
    std::sort(points.begin(), points.begin() + n,
    [](const Point &a, const Point &b) -> bool
    {
        return a.x < b.x;
    });

    // DEBUG
    //for (Point& p : points)
    //{
    //    cout << p << ' ';
    //}
    //cout << endl;

    unordered_set<string> pointsInConvexHull;
    ConvexHullSolver cs(points, pointsInConvexHull);
    cs.getConvexHull(0, n - 1);

    cout << "x,y,isCH" << endl;
    for (Point& p : points)
    {
        char flag = pointsInConvexHull.count(p.hash()) ? '1' : '0';
        cout << p.x << ',' << p.y << ',' << flag << endl;
    }
}

int main()
{
    uniformPointDistribution();
    return 0;
}

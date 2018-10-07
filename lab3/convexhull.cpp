#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <unordered_set>

using std::cout;
using std::endl;
using std::vector;
using std::string;

struct Point
{
    double x;
    double y;
    double s;
    double phi;

    void calculatePolar(const Point& ref)
    {
        double x_rel = (double)x - (double)ref.x;
        double y_rel = (double)y - (double)ref.y;
        this->s = sqrt(pow(x_rel, 2) + pow(y_rel, 2));
        this->phi = atan2(y_rel, x_rel);
    }

    float calculateDist(const Point& other)
    {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
    }

    std::string hash()
    {
        return std::to_string(x) + "," + std::to_string(y);
    }
};

std::ostream& operator<<(std::ostream& io, Point& s)
{
    io << s.x << "," << s.y;
    return io;
}

/*
 * Determines wheter point c is found by going clockwise from
 * line ab.
 */
bool isCW(Point& a, Point& b, Point& c)
{
    int ans = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return ans > 0;
}

/*
 * Finds a convex hull in a set of n points
 */
vector<Point*> grahmScan(vector<Point>& points)
{
    // Sort points by y value
    std::sort(points.begin(), points.end(),
        [](const Point &a, const Point &b) -> bool
        {
            return a.y < b.y;
        });

    Point& p0 = points[0];
    for (int i = 1; i < points.size(); i++)
    {
        points[i].calculatePolar(p0);
    }

    // Sort points by relative phi value
    std::sort(points.begin() + 1, points.end(),
        [](const Point &a, const Point &b) -> bool
        {
            return a.phi < b.phi;
        });

    // Compute graham scan.
    vector<Point*> st;
    st.push_back(&p0);
    st.push_back(&points[1]);
    st.push_back(&points[2]);
    for (int i = 3; i < points.size(); i++)
    {
        while (isCW(*st[st.size() - 1], *st[st.size() - 2], points[i]))
        {
            st.pop_back();
        }
        st.push_back(&points[i]);
    }

    return st;
}


/*
 * Checks if two convex hulls interect by using bounding circles.
 * Can sometimes create false positives
 */
bool doConvexHullsIntersect(vector<Point>& l, vector<Point>& r)
{
    Point lMean;
    double lDist = 0;
    for (Point& p : l)
    {
        lMean.x += p.x;
        lMean.y += p.y;
    }
    lMean.x /= l.size();
    lMean.y /= l.size();
    for (Point& p : l)
    {
        lDist = fmax(lDist, lMean.calculateDist(p));
    }

    cout << lMean << endl;
    cout << lDist << endl;

    Point rMean;
    double rDist = 0;
    for (Point& p : r)
    {
        rMean.x += p.x;
        rMean.y += p.y;
    }
    rMean.x /= r.size();
    rMean.y /= r.size();
    for (Point& p : r)
    {
        rDist = fmax(rDist, rMean.calculateDist(p));
    }

    cout << rMean << endl;
    cout << rDist << endl;

    return rDist + lDist >= lMean.calculateDist(rMean);
}

/*
 * For a line that passes through x and y, return 1 is z is 
 * above the line, -1 if it is below or 0 if it is tangent.
 */
int getTangentDirection(Point& a, Point& b, Point& z)
{
    int det = a.x * b.y - a.x * z.y
            - a.y * b.x + a.y * z.x
            + b.x * z.y - b.y * z.x;

    return det;
}

/*
 * Checks if two convex hulls interect by using a separating line.
 * Assumes the two convex hulls l and r have their points in increasingly
 * monotomically in either the CW or CCW direction.
 */
bool doConvexHullsIntersectGuaranteed(vector<Point>& l, vector<Point>& r)
{
    Point lMean;
    for (Point& p : l)
    {
        lMean.x += p.x;
        lMean.y += p.y;
    }
    lMean.x /= l.size();
    lMean.y /= l.size();

    Point rMean;
    for (Point& p : r)
    {
        rMean.x += p.x;
        rMean.y += p.y;
    }
    rMean.x /= r.size();
    rMean.y /= r.size();

    if (lMean.x > rMean.x)
    {
        swap(l, r);
    }

    for (int i = 0; i < l.size() - 1; i++)
    {
        Point& a = l[i];
        Point& b = l[(i + 1) % l.size()];
        int counter = 0;
        for (Point& z : r)
        {
            if (getTangentDirection(a, b, z) <= 0)
            {
                break;
            }
            counter++;
        }

        if (counter == r.size())
        {
            cout << a << ";" << b << endl;
            return false;
        }
    }

    for (int i = 0; i < r.size() - 1; i++)
    {
        Point& a = r[i];
        Point& b = r[(i + 1) % r.size()];
        int counter = 0;
        for (Point& z : l)
        {
            if (getTangentDirection(a, b, z) <= 0)
            {
                break;
            }
            counter++;
        }

        if (counter == l.size())
        {
            cout << a << ";" << b << endl;
            return false;
        }
    }

    cout << endl;
    return true;
}

      


void uniformPointDistribution()
{
    // Generate n points
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(-100, 100);
    cout << "n,c" << endl;
    //cout << "x,y,isCH" << endl;
    for (int n = 10; n < 1000; n += 10)
    {
        vector<Point> points(n);
        std::unordered_set<string> generated;
        for (int i = 0; i < n; i++)
        {
            do
            {
                points[i].x = distribution(generator);
                points[i].y = distribution(generator);
            }
            while (generated.find(points[i].hash()) != generated.end());
            generated.insert(points[i].hash());
        }

        vector<Point*> convexHull = grahmScan(points);
        cout << n - convexHull.size() << ',' << convexHull.size() << endl;

        //for (Point &p : points)
        //{
        //    cout << p.x << "," << p.y << ",0" << endl;
        //}

        //for (Point* p : convexHull)
        //{
        //    cout << p->x << "," << p->y << ",1" << endl;
        //}
    }
}

void gaussianPointDistribution()
{
    // Generate n points
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0, 1);
    cout << "n,c" << endl;
    //cout << "x,y,isCH" << endl;
    for (int n = 10; n < 1000; n += 10)
    {
        vector<Point> points(n);
        std::unordered_set<string> generated;
        for (int i = 0; i < n; i++)
        {
            do
            {
                points[i].x = distribution(generator) * 100;
                points[i].y = distribution(generator) * 100;
            }
            while (generated.find(points[i].hash()) != generated.end());
            generated.insert(points[i].hash());
        }

        vector<Point*> convexHull = grahmScan(points);
        cout << n - convexHull.size() << ',' << convexHull.size() << endl;

        //for (Point &p : points)
        //{
        //    cout << p.x << "," << p.y << ",0" << endl;
        //}

        //for (Point* p : convexHull)
        //{
        //    cout << p->x << "," << p->y << ",1" << endl;
        //}
    }
}

void intersectingHulls()
{
    vector<Point> ch1{{-10, -10}, {-9, 4}, {10, 7}, {0, -4}};
    vector<Point> ch2{{20, 20}, {25, 10}, {5, 5}, {10, 15}, {17, 19}};
    for (auto& p : ch1)
    {
        cout << p << ';';
    }
    cout << endl;

    for (auto& p : ch2)
    {
        cout << p << ';';
    }
    cout << endl;
    cout << doConvexHullsIntersect(ch1, ch2) << endl;
}

void intersectingHullsComplete()
{
    //vector<Point> ch1{{-10, -10}, {-9, 4}, {3, 3}, {0, -4}};
    //vector<Point> ch2{{20, 20}, {25, 10}, {5, 5}, {10, 15}, {17, 19}};
    //vector<Point> ch1{{-10, -10}, {-9, 4}, {3, 10}, {7, 9}};
    //vector<Point> ch2{{20, 20}, {25, 10}, {10, 5}, {10, 15}, {17, 19}};
    vector<Point> ch1{{-10, -10}, {-9, 4}, {3, 10}, {7, 9}};
    vector<Point> ch2{{20, 20}, {25, 10}, {0, 0}, {10, 15}, {17, 19}};
    for (auto& p : ch1)
    {
        cout << p << ';';
    }
    cout << endl;

    for (auto& p : ch2)
    {
        cout << p << ';';
    }
    cout << endl;
    cout << doConvexHullsIntersectGuaranteed(ch1, ch2) << endl;
}

int main()
{
    //uniformPointDistribution();
    //gaussianPointDistribution();
    //intersectingHulls();
    intersectingHullsComplete();
    return 0;
}

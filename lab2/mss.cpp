#include <iostream>
#include <vector>
#include <assert.h>
#include "shared.h"

using std::cout;
using std::endl;

Sublist mssThatCrossesMidpoint(std::vector<int>& v, int start, int end, int mid)
{
    // Final option is that mss crosses through the midpoint
    Sublist leftMidRight;

    int localSum = 0;
    int leftMaxSumWithMid = INT_MIN;
    for (int i = mid; i >= start; i--)
    {
        localSum += v[i];
        if (localSum > leftMaxSumWithMid)
        {
            leftMaxSumWithMid = localSum;
            leftMidRight.start = i;
        }
    }

    localSum = 0;
    int rightMaxSumWithMid = INT_MIN;
    for (int i = mid + 1; i <= end; i++)
    {
        localSum += v[i];
        if (localSum > rightMaxSumWithMid)
        {
            rightMaxSumWithMid = localSum;
            leftMidRight.end = i;
        }
    }

    leftMidRight.sum = leftMaxSumWithMid + rightMaxSumWithMid;

    return leftMidRight;
}

Sublist mss(std::vector<int>& v, int start, int end)
{
    if (start == end)
    {
        return {start, end, v[start]};
    }

    // Generate mss on the left and right
    int mid = (start + end)/2;
    Sublist sublistWithMaxSum;
    Sublist left = mss(v, start, mid);
    Sublist right = mss(v, mid + 1, end);
    if (left.sum != right.sum)
    {
        sublistWithMaxSum = left.sum > right.sum ? left : right;
    }
    else
    {
        sublistWithMaxSum = left.size() <= right.size() ? left : right;
    }

    Sublist midpoint = mssThatCrossesMidpoint(v, start, end, mid);
    if (midpoint.sum > sublistWithMaxSum.sum ||
            (midpoint.sum == sublistWithMaxSum.sum && midpoint.size() < sublistWithMaxSum.size()))
    {
        sublistWithMaxSum = midpoint;
    }

    return sublistWithMaxSum;
}

Sublist mss2(std::vector<int>& v, int, int)
{
    Sublist firstSolution = mss(v, 0, v.size() - 1);
    if (firstSolution.size() == v.size())
    {
        return {-1, -1, INT_MIN};
    }

    int min = *(std::min_element(v.begin(), v.end()));
    int minReplacement = min < 0 ? min : 0;

    for (int i = firstSolution.start; i <= firstSolution.end; i++)
    {
        v[i] = minReplacement;
    }

    return mss(v, 0, v.size() - 1);
}

void part1()
{
    cout << "Part 1" << endl;
    Sublist sol;
    std::vector<int> v1{2, -1, 2};
    sol = mss(v1, 0, v1.size() - 1);
    cout << sol << endl;
    assert(sol.sum == 3);
    assert(sol.start == 0);
    assert(sol.end == 2);

    std::vector<int> v2{-2, 1, -3, 4, -1, 2, 1, -5, 4};
    sol = mss(v2, 0, v2.size() - 1);
    cout << sol << endl;
    assert(sol.sum == 6);
    assert(sol.start == 3);
    assert(sol.end == 6);

    std::vector<int> v3{100, 250, 100};
    sol = mss(v3, 0, v3.size() - 1);
    cout << sol << endl;
    assert(sol.sum == 450);
    assert(sol.start == 0);
    assert(sol.end == 2);

    std::vector<int> v4{-1, -4, -3, 0, -1};
    sol = mss(v4, 0, v4.size() - 1);
    cout << sol << endl;
    assert(sol.sum == 0);
    assert(sol.start == 3);
    assert(sol.end == 3);

    std::vector<int> v5{-1, 2, 3, -5, 8, 2, -2, 3};
    sol = mss(v5, 0, v5.size() - 1);
    cout << sol << endl;
    assert(sol.sum == 11);
    assert(sol.start == 4);
    assert(sol.end == 7);

    std::vector<int> v6{-22, -10, -1, -1, -22, -9};
    sol = mss(v6, 0, v6.size() - 1);
    cout << sol << endl;
    assert(sol.sum == -1);
    assert(sol.start == 2);
    assert(sol.end == 2);

    std::vector<int> v7{-8, 3, 4, 1, -8, 8, -8, 6, 2, -8};
    sol = mss(v7, 0, v7.size() - 1);
    cout << sol << endl;
    assert(sol.sum == 8);
    assert(sol.start == 5);
    assert(sol.end == 5);

    cout << endl;
}

void part2()
{
    cout << "Part 2" << endl;
    Sublist sol;
    std::vector<int> v1{2, -1, 2};
    sol = mss2(v1, 0, v1.size() - 1);
    cout << sol << endl;
    assert(sol.sum == INT_MIN);
    assert(sol.start == -1);
    assert(sol.end == -1);

    std::vector<int> v2{-2, 1, -3, 4, -1, 2, 1, -5, 4};
    sol = mss2(v2, 0, v2.size() - 1);
    cout << sol << endl;
    assert(sol.sum == 4);
    assert(sol.start == 8);
    assert(sol.end == 8);

    std::vector<int> v3{100, 250, 100};
    sol = mss2(v3, 0, v3.size() - 1);
    cout << sol << endl;
    assert(sol.sum == INT_MIN);
    assert(sol.start == -1);
    assert(sol.end == -1);

    std::vector<int> v4{-1, -4, -3, 0, -1};
    sol = mss2(v4, 0, v4.size() - 1);
    cout << sol << endl;
    assert(sol.sum == -1);
    assert(sol.start == 0);
    assert(sol.end == 0);

    std::vector<int> v5{-1, 2, 3, -5, 8, 2, -2, 3};
    sol = mss2(v5, 0, v5.size() - 1);
    cout << sol << endl;
    assert(sol.sum == 5);
    assert(sol.start == 1);
    assert(sol.end == 2);

    std::vector<int> v6{-22, -10, -1, -1, -22, -9};
    sol = mss2(v6, 0, v6.size() - 1);
    cout << sol << endl;
    assert(sol.sum == -1);
    assert(sol.start == 3);
    assert(sol.end == 3);

    std::vector<int> v7{-8, 3, 4, 1, -8, 8, -8, 6, 2, -8};
    sol = mss2(v7, 0, v7.size() - 1);
    cout << sol << endl;
    assert(sol.sum == 8);
    assert(sol.start == 7);
    assert(sol.end == 8);
}


int main()
{
    part1();
    part2();


    return 0;
}

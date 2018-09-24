#include <iostream>
#include <vector>
#include <assert.h>
#include <utility>
#include <algorithm>

#include "shared.h"

using std::cout;
using std::endl;

/*
 * Returns the two largest mss that in the the range (start, mid) and (mid + 1, end)
 */
std::pair<Sublist, Sublist> mssThatCrossesMidpoint(std::vector<int>& v, int start, int end, int mid)
{

    // Set the greatest sublist to INT_MIN, since it will be passed to secondGreatestSublist.
    Sublist greatestSublist = {INT_MIN, INT_MAX, INT_MIN};
    Sublist secondGreatestsublist = {};

    int localSum = 0;
    int leftMaxSum = INT_MIN;
    int secondLeftMaxSum = INT_MIN;
    for (int i = mid; i >= start; i--)
    {
        localSum += v[i];
        if (localSum > leftMaxSum)
        {
            // Pass information from greatest to second greatest
            secondGreatestsublist.start = greatestSublist.start;
            secondLeftMaxSum = leftMaxSum;

            // Update the greatest
            leftMaxSum = localSum;
            greatestSublist.start = i;
        }
        else if (localSum > secondLeftMaxSum)
        {
            // Second greatest can be updated, but not the greatest.
            secondGreatestsublist.start = i;
            secondGreatestsublist.sum = localSum;
        }
    }

    // Repeat the same process, but in the reverse direction.
    localSum = 0;
    int rightMaxSum = INT_MIN;
    int secondRightMaxSum = INT_MIN;
    for (int i = mid + 1; i <= end; i++)
    {
        localSum += v[i];
        if (localSum > rightMaxSum)
        {
            secondGreatestsublist.end = greatestSublist.end;
            secondRightMaxSum = rightMaxSum;

            rightMaxSum = localSum;
            greatestSublist.end = i;
        }
        else if (localSum > secondRightMaxSum)
        {
            secondGreatestsublist.end = i;
            secondGreatestsublist.sum = localSum;
        }
    }

    greatestSublist.sum = leftMaxSum + rightMaxSum;
    if (secondLeftMaxSum == INT_MIN || secondRightMaxSum == INT_MIN)
    {
        // Since adding two INT_MIN values will result in overflow, 
        // set the value to INT_MIN if either is true. This will happen
        // in cases where there is no "second largest" sublist, such as 
        // a list with three elements.
        secondGreatestsublist.sum = INT_MIN;
    }
    else
    {
        secondGreatestsublist.sum = secondLeftMaxSum + secondRightMaxSum;
    }

    return {greatestSublist, secondGreatestsublist};
}

/*
 * Returns the two largest overlapping mss. 
 * rval.first is the largest value
 * rval.second is the second largest value
 */
std::pair<Sublist, Sublist> mss(std::vector<int>& v, int start, int end)
{
    if (start == end)
    {
        return {
            {start, end, v[start]},
            {start, end, INT_MIN}
        };
    }

    // Generate mss on the left and right and midpoint
    int mid = (start + end)/2;
    std::pair<Sublist, Sublist> left = mss(v, start, mid);
    std::pair<Sublist, Sublist> right = mss(v, mid + 1, end);
    std::pair<Sublist, Sublist> midpoint = mssThatCrossesMidpoint(v, start, end, mid);

    // Sort all the sublist values
    std::vector<Sublist> sublists{left.first, left.second, right.first, right.second, midpoint.first, midpoint.second};
    std::sort(sublists.begin(), sublists.end());

    // Select the top two values
    return {sublists[sublists.size() - 1], sublists[sublists.size() - 2]};
}

void part3()
{
    cout << "Part 3" << endl;
    std::pair<Sublist, Sublist> sol;

    std::vector<int> v1{2, -1, 2};
    sol = mss(v1, 0, v1.size() - 1);
    cout << sol.first << endl;
    cout << sol.second << endl;
    assert(sol.first.start == 0);
    assert(sol.first.end == 2);
    assert(sol.first.sum == 3);
    assert(sol.second.start == 2);
    assert(sol.second.end == 2);
    assert(sol.second.sum == 2);

    
    std::vector<int> v2{-2, 1, -3, 4, -1, 2, 1, -5, 4};
    sol = mss(v2, 0, v2.size() - 1);
    cout << sol.first << endl;
    cout << sol.second << endl;
    assert(sol.first.start == 3);
    assert(sol.first.end == 6);
    assert(sol.first.sum == 6);
    assert(sol.second.start == 8);
    assert(sol.second.end == 8);
    assert(sol.second.sum == 4);

    std::vector<int> v3{100, 250, 100};
    sol = mss(v3, 0, v3.size() - 1);
    cout << sol.first << endl;
    cout << sol.second << endl;
    assert(sol.first.start == 0);
    assert(sol.first.end == 2);
    assert(sol.first.sum == 450);
    assert(sol.second.start == 0);
    assert(sol.second.end == 1);
    assert(sol.second.sum == 350);
    

    
    std::vector<int> v4{-1, -4, -3, 0, -1};
    sol = mss(v4, 0, v4.size() - 1);
    cout << sol.first << endl;
    cout << sol.second << endl;
    assert(sol.first.start == 3);
    assert(sol.first.end == 3);
    assert(sol.first.sum == 0);
    assert(sol.second.start == 4);
    assert(sol.second.end == 4);
    assert(sol.second.sum == -1);
    
    
    std::vector<int> v5{-1, 2, 3, -5, 8, 2, -2, 3};
    sol = mss(v5, 0, v5.size() - 1);
    cout << sol.first << endl;
    cout << sol.second << endl;
    assert(sol.first.start == 4);
    assert(sol.first.end == 7);
    assert(sol.first.sum == 11);
    assert(sol.second.start == 1);
    assert(sol.second.end == 7);
    assert(sol.second.sum == 11);


    std::vector<int> v6{-22, -10, -1, -1, -22, -9};
    sol = mss(v6, 0, v6.size() - 1);
    cout << sol.first << endl;
    cout << sol.second << endl;
    assert(sol.first.start == 3);
    assert(sol.first.end == 3);
    assert(sol.first.sum == -1);
    assert(sol.second.start == 2);
    assert(sol.second.end == 2);
    assert(sol.second.sum == -1);


    std::vector<int> v7{-8, 3, 4, 1, -8, 8, -8, 6, 2, -8};
    sol = mss(v7, 0, v7.size() - 1);
    cout << sol.first << endl;
    cout << sol.second << endl;
    assert(sol.first.start == 5);
    assert(sol.first.end == 5);
    assert(sol.first.sum == 8);
    assert(sol.second.start == 7);
    assert(sol.second.end == 8);
    assert(sol.second.sum == 8);

    std::vector<int> v8{0, -2, 1, 0, 0, -1};
    sol = mss(v8, 0, v8.size() - 1);
    cout << sol.first << endl;
    cout << sol.second << endl;
    assert(sol.first.start == 2);
    assert(sol.first.end == 2);
    assert(sol.first.sum == 1);
    assert(sol.second.start == 2);
    assert(sol.second.end == 3);
    assert(sol.second.sum == 1);

    cout << endl;
}

int main()
{
    part3();

    return 0;
}

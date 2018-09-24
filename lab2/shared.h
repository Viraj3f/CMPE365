#include <iostream>
#include <vector>

struct Sublist
{
    int start;
    int end;
    int sum;

    int size() const
    {
        return end - start + 1;
    }

    bool operator<(const Sublist& right) const
    {
        if (this->sum == right.sum)
        {
            return this->size() > right.size();
        }
        else
        {
            return this->sum < right.sum;
        }
    }

    bool operator==(const Sublist& right) const
    {
        return this->start == right.start && this->end == right.end && this->sum == right.sum;
    }
};

std::ostream& operator<<(std::ostream& io, Sublist& s)
{
    io << "Sum: " << s.sum << " Start: " << s.start << " End: " << s.end;
    return io;
}

std::ostream& operator<<(std::ostream& io, std::vector<int> v)
{
    io << "[";
    if (v.size() > 0)
    {
        for (int i = 0; i < v.size() - 1; i++)
        {
            io << v[i] << ", ";
        }
        io << v[v.size() - 1];
    }
    io << "]";
    return io;
}

#pragma once

#include <limits>
#include <cassert>
#include <ostream>
#include <utility>



using Timestamp = int;


class Interval
{
public:
	Timestamp start; // inclusive
	Timestamp end;   // inclusive

public:
	Interval(Timestamp start, Timestamp end) noexcept
	:
		start(start),
		end(end)
	{
		assert(start <= end);
	}


	bool operator < (const Interval& rhs) const noexcept
	{
		return start < rhs.start;
	}


	bool overlaps(const Interval& other) const noexcept
	{
		return start <= other.end && other.start <= end;
	}


	bool covers(const Interval& other) const noexcept
	{
		return start <= other.start && end >= other.end;
	}


	Timestamp getLength() const noexcept
	{
		return end - start + 1;
	}


	friend void swap(Interval& a, Interval& b) noexcept
	{
		using std::swap;
		swap(a.start, b.start);
		swap(a.end  , b.end  );
	}


	friend std::ostream& operator << (std::ostream &out, const Interval& self)
	{
		return out << '[' << self.start << ',' << self.end << ']';
	}
};





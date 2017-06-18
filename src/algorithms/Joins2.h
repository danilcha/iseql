#pragma once

#include "Joins1.h"



template <typename Consumer>
void startPrecedingJoin(const Relation& R, const Relation& S, const Consumer& consumer) noexcept
{
	joinBySStart(R, S, Iterator(R.getIndex()), consumer);
}



template <typename Consumer>
void endFollowingJoin(const Relation& R, const Relation& S, const Consumer& consumer) noexcept
{
	joinBySEnd(R, S, Iterator(R.getIndex()), consumer);
}



template <typename Consumer>
void beforeJoin(const Relation& R, const Relation& S, Timestamp delta, const Consumer& consumer) noexcept
{
	constexpr auto START = Endpoint::Type::START;
	constexpr auto END   = Endpoint::Type::END;

	assert(delta > 0);

	const Index& index = R.getIndex();


	joinBySStart(
		R,
		S,
		makeMergingIterator(
			makeShiftingIterator(makeFilteringIterator(index, END), 1, END, START), // r.end + 1     -> r.start
			makeShiftingIterator(makeFilteringIterator(index, END), delta)          // r.end + delta -> r.end
		),
		consumer
	);
}



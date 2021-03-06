#include <set>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "algorithms/IEJoin.h"
#include "algorithms/Joins.h"
#include "algorithms/LMJoins.h"


using testing::Test;
using testing::Pair;
using testing::UnorderedElementsAre;



static Relation R = {
	{1,  6, 1},
	{1, 11, 2},
	{7, 12, 3},
	{9, 11, 4},
};

static Relation S = {
	{2,  3, 1},
	{3, 13, 2},
	{4,  6, 3},
	{5,  7, 4},
	{8, 10, 5},
};


static Index indexR(R);
static Index indexS(S);

static std::set<std::pair<int, int>> result;


static auto consumer = [] (const Tuple& r, const Tuple& s)
{
	result.emplace(r.getId(), s.getId());
};



class Joins : public Test
{
protected:
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
	static void SetUpTestCase()
	{
		R.setIndex(indexR);
		S.setIndex(indexS);
	}
	#pragma clang diagnostic pop

	virtual void TearDown()
	{
//		for (const auto& item : result)
//			std::cout << item.first << ' ' << item.second << std::endl;

		result.clear();
	}
};



TEST_F(Joins, startPreceding)
{
	startPrecedingJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(1, 2),
		Pair(1, 3),
		Pair(1, 4),
		Pair(2, 1),
		Pair(2, 2),
		Pair(2, 3),
		Pair(2, 4),
		Pair(2, 5),
		Pair(3, 5)
	));
}



TEST_F(Joins, startPrecedingStrict)
{
	startPrecedingStrictJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(1, 2),
		Pair(1, 3),
		Pair(1, 4),
		Pair(2, 1),
		Pair(2, 2),
		Pair(2, 3),
		Pair(2, 4),
		Pair(2, 5),
		Pair(3, 5)
	));
}

TEST_F(Joins, startPrecedingStrictIE)
{
	ieJoinStartPrecedingStrictJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(1, 2),
		Pair(1, 3),
		Pair(1, 4),
		Pair(2, 1),
		Pair(2, 2),
		Pair(2, 3),
		Pair(2, 4),
		Pair(2, 5),
		Pair(3, 5)
	));
}



TEST_F(Joins, reverseStartPreceding)
{
	reverseStartPrecedingJoin(S, R, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(2, 1),
		Pair(3, 1),
		Pair(4, 1),
		Pair(1, 2),
		Pair(2, 2),
		Pair(3, 2),
		Pair(4, 2),
		Pair(5, 2),
		Pair(5, 3)
	));
}



TEST_F(Joins, startPrecedingWithDelta)
{
	startPrecedingJoin(R, S, 1, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(2, 1),
		Pair(3, 5)
	));
}



TEST_F(Joins, endFollowing)
{
	endFollowingJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(1, 3),
		Pair(2, 1),
		Pair(2, 3),
		Pair(2, 4),
		Pair(2, 5),
		Pair(3, 5),
		Pair(4, 5)
	));
}



TEST_F(Joins, endFollowingStrict)
{
	endFollowingStrictJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(2, 1),
		Pair(2, 3),
		Pair(2, 4),
		Pair(2, 5),
		Pair(3, 5),
		Pair(4, 5)
	));
}



TEST_F(Joins, reverseEndFollowing)
{
	reverseEndFollowingJoin(S, R, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(3, 1),
		Pair(1, 2),
		Pair(3, 2),
		Pair(4, 2),
		Pair(5, 2),
		Pair(5, 3),
		Pair(5, 4)
	));
}



TEST_F(Joins, endFollowingWithEpsilon)
{
	endFollowingJoin(R, S, 1, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 3),
		Pair(2, 5),
		Pair(4, 5)
	));
}



TEST_F(Joins, leftOverlap)
{
	leftOverlapJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 2),
		Pair(1, 3),
		Pair(1, 4),
		Pair(2, 2)
	));
}


TEST_F(Joins, leftOverlapStrict)
{
	leftOverlapStrictJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 2),
		Pair(1, 4),
		Pair(2, 2)
	));
}



TEST_F(Joins, leftOverlapWithDelta)
{
	leftOverlapJoin(R, S, 2, UNBOUND, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 2),
		Pair(2, 2)
	));
}



TEST_F(Joins, rightOverlap)
{
	rightOverlapJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(4, 5)
	));
}



TEST_F(Joins, rightOverlapWithEpsilon)
{
	rightOverlapJoin(R, S, UNBOUND, 0, consumer);

	EXPECT_THAT(result, UnorderedElementsAre());
}



TEST_F(Joins, during)
{
	duringJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(3, 2),
		Pair(4, 2)
	));
}





TEST_F(Joins, duringWithDelta)
{
	duringJoin(R, S, 4, UNBOUND, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(3, 2)
	));
}



TEST_F(Joins, reverseDuring)
{
	reverseDuringJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(1, 3),
		Pair(2, 1),
		Pair(2, 3),
		Pair(2, 4),
		Pair(2, 5),
		Pair(3, 5)
	));
}


TEST_F(Joins, reverseDuringStrict)
{
	reverseDuringStrictJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(2, 1),
		Pair(2, 3),
		Pair(2, 4),
		Pair(2, 5),
		Pair(3, 5)
	));
}


TEST_F(Joins, reverseDuringStrictLM)
{
	leungMuntzReverseDuringStrictJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(2, 1),
		Pair(2, 3),
		Pair(2, 4),
		Pair(2, 5),
		Pair(3, 5)
	));
}


TEST_F(Joins, reverseDuringStrictIE)
{
	ieJoinReverseDuringStrictJoin(R, S, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(2, 1),
		Pair(2, 3),
		Pair(2, 4),
		Pair(2, 5),
		Pair(3, 5)
	));
}


TEST_F(Joins, reverseDuringWithDeltaAndEpsilon)
{
	reverseDuringJoin(R, S, 4, 3, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 1),
		Pair(1, 3),
		Pair(3, 5)
	));
}



TEST_F(Joins, before)
{
	beforeJoin(R, S, 2, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(1, 5)
	));
}



TEST_F(Joins, after)
{
	afterJoin(R, S, 1, consumer);

	EXPECT_THAT(result, UnorderedElementsAre(
		Pair(3, 3),
		Pair(3, 4)
	));
}




#pragma once

#include <iostream>
#include "Iterators.h"
#include "containers/GaplessHashMap.h"



template <typename IteratorR, typename IteratorS, typename Compare, typename Consumer>
void joinByS(const Relation& R, const Relation& S, IteratorR itR, IteratorS itS, Compare comp, const Consumer& consumer)
{
	GaplessHashMap<TID, Tuple> activeR(32);

	for (;;)
	{
		if (comp(itR.getEndpoint(), itS.getEndpoint()))
		{
			const Endpoint& endpointR = itR.getEndpoint();
			TID tid = endpointR.getTID();

			if (endpointR.isStart())
				activeR.insert(tid, R[tid]);
			else
				activeR.erase(tid);

			itR.moveToNextEndpoint();
			if (itR.isFinished())
				break;
		}
		else
		{
			Tuple buffer[32];
			auto bufferBegin = std::begin(buffer);
			auto bufferEnd   = std::end(buffer);
			auto bufferNext  = bufferBegin;

			do
			{
				*bufferNext = S[itS.getEndpoint().getTID()];
				++bufferNext;
				itS.moveToNextEndpoint();
			}
			while (!itS.isFinished() && !comp(itR.getEndpoint(), itS.getEndpoint()) && bufferNext != bufferEnd);


			switch (bufferNext - bufferBegin)
			{
				case 1:
					for (const auto& r : activeR)
					{
						consumer(r, buffer[0]);
					}
					break;

				case 2:
					for (const auto& r : activeR)
					{
						consumer(r, buffer[0]);
						consumer(r, buffer[1]);
					}
					break;

				case 3:
					for (const auto& r : activeR)
					{
						consumer(r, buffer[0]);
						consumer(r, buffer[1]);
						consumer(r, buffer[2]);
					}
					break;

				case 4:
					for (const auto& r : activeR)
					{
						consumer(r, buffer[0]);
						consumer(r, buffer[1]);
						consumer(r, buffer[2]);
						consumer(r, buffer[3]);
					}
					break;

				default:
					for (const auto& r : activeR)
					{
						for (auto s = bufferBegin; s != bufferNext; ++s)
						{
							consumer(r, *s);
						}
					}
			}

			if (itS.isFinished())
				break;
		}
	}
}


//template <typename IteratorR, typename IteratorS, typename Compare, typename Consumer>
//void joinByS(const Relation& R, const Relation& S, IteratorR itR, IteratorS itS, Compare comp, const Consumer& consumer)
//{
//	GaplessHashMap<TID, Tuple> activeR(32);
//
//	for (;;)
//	{
//		if (comp(itR.getEndpoint(), itS.getEndpoint()))
//		{
//			const Endpoint& endpointR = itR.getEndpoint();
//			TID tid = endpointR.getTID();
//
//			if (endpointR.isStart())
//				activeR.insert(tid, R[tid]);
//			else
//				activeR.erase(tid);
//
//			itR.moveToNextEndpoint();
//			if (itR.isFinished())
//				break;
//		}
//		else
//		{
//			const Tuple& s = S[itS.getEndpoint().getTID()];
//			for (const auto& r : activeR)
//				consumer(r, s);
//
//			itS.moveToNextEndpoint();
//			if (itS.isFinished())
//				break;
//		}
//	}
//}

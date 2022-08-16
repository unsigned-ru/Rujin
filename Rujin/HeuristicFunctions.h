#ifndef RUJIN_HEURISTIC_FUNCTIONS_H
#define RUJIN_HEURISTIC_FUNCTIONS_H

#include <cmath>

#pragma warning( push )
#pragma warning( disable : 4505 )
namespace rujin::heuristic_functions
{
	//Manhattan distance
	static float Manhattan(const float x, const float y)
	{
		return x + y;
	}

	//Euclidean distance
	static float Euclidean(const float x, const float y)
	{
		return sqrt(x * x + y * y);
	}

	//Sqrt Euclidean distance
	static float SqrtEuclidean(const float x, const float y)
	{
		return x * x + y * y;
	}

	//Octile distance
	static float Octile(const float x, const float y)
	{
		constexpr auto f = 0.414213562373095048801f; // == sqrt(2) - 1;
		return (x < y) ? f * x + y : f * y + x;
	}

	//Chebyshev distance
	static float Chebyshev(const float x, const float y)
	{
#undef max
		return std::max(x, y);
	};
}

#pragma warning( pop )
#endif // RUJIN_HEURISTIC_FUNCTIONS_H Include Guard.
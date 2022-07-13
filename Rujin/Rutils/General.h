#pragma once
#include <string>

#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

namespace rutils
{
	enum TimeLocale : uint32_t { Local, UTC };
	tm GetDisplayTime(TimeLocale locale = Local);

	int RandInt(const int min, const int max);
	float RandFloatWithPreciscion(const float min, const float max, const int preciscion);
	float RandFloat(const float min, const float max);

	inline bool FloatEquals(float a, float b, float epsilon = FLT_EPSILON) { return fabs(a - b) < epsilon; }
}
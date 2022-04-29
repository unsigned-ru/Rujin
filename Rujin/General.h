#pragma once
#include <string>

#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

namespace rutils
{
	int RandInt(const int min, const int max);
	float RandFloatWithPreciscion(const float min, const float max, const int preciscion);
	float RandFloat(const float min, const float max);
}
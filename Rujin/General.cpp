#include "stdafx.h"

#include "General.h"

int rutils::RandInt(const int min, const int max)
{
	return rand() % (max - min + 1) + min;
}

float rutils::RandFloatWithPreciscion(const float min, const float max, const int preciscion)
{
	return static_cast<float>(rand() % static_cast<int>(max * pow(10, static_cast<float>(preciscion)) - min * pow(10, static_cast<float>(preciscion))) + min * pow(10, static_cast<float>(preciscion))) / powf(10, static_cast<float>(preciscion));
}

float rutils::RandFloat(const float min, const float max)
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min) + min;
}


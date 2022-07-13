#include "../RujinPCH.h"
#include "General.h"
#include <ctime>

tm rutils::GetDisplayTime(TimeLocale locale)
{
	tm rv{};
	const time_t now = time(nullptr);

	switch (locale)
	{
	case Local:
		localtime_s(&rv, &now);
		break;
	case UTC:
		gmtime_s(&rv, &now);
		break;
	default:
		assert(!"Unimplemented time locale requested.");
		break;
	}

	rv.tm_year += 1900;
	rv.tm_mon += 1;
	rv.tm_wday += 1;
	rv.tm_yday += 1;

	return rv;
}

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
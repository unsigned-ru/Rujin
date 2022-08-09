#ifndef INPUTEVENTS_H
#define INPUTEVENTS_H
#include <cstdint>


enum class InputAction : uint32_t
{
	Shoot
};

enum class AxisAction : uint32_t
{
	MoveRight,
	MoveUp,
	AimRight,
	AimUp
};


#endif // Include Guard: INPUTEVENTS_H

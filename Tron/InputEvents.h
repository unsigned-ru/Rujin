#ifndef INPUTEVENTS_H
#define INPUTEVENTS_H
#include <cstdint>


enum class InputAction : uint32_t
{
	LeftClick,
	RightClick
};

enum class AxisAction : uint32_t
{
	MoveRight,
	MoveUp
};


#endif // Include Guard: INPUTEVENTS_H

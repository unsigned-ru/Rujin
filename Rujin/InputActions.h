#pragma once
#include <stdint.h>

enum class InputAction : uint32_t
{
	StartSound1,
	StopSound1,
	PauseSound1,
	SwitchSoundSystem
};

enum class AxisAction : uint32_t
{
	Volume,
	LookRight
};
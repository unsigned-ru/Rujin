#pragma once
#ifndef PCH_H
#define PCH_H

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma warning(push, 0)
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <SDL.h>
#undef main;
#include <SDL_mixer.h>
#pragma warning (pop)

#include "Rutils/Logger.h"
#endif PCH_H


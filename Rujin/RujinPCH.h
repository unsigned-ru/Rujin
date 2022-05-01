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

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <SDL.h>
#include <SDL_mixer.h>

#include "Rutils/Logger.h"

#endif PCH_H
#pragma warning (pop)


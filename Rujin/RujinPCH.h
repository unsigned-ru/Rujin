#ifndef RUJIN_PCH_H
#define RUJIN_PCH_H

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <numbers> // For mathematical constants.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <SDL.h> 
#undef main
#include <SDL_mixer.h>
#include <GL/gl.h>
#include <GL/GLU.h>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#pragma warning (pop)

#include "Rutils/Logger.h"


#endif RUJIN_PCH_H



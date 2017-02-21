#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <vector>
#include <map>
#include <list>
#include <functional>

using namespace std;

#include <SDL.h>
#include <SDL_image.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "SmartPtr.h"
#include "helpers.h"
#include "Log.h"

#define O_MAX_PATH 128

using ODiagnostics::LogDebug;
using ODiagnostics::LogInfo;
using ODiagnostics::LogWarning;
using ODiagnostics::LogError;

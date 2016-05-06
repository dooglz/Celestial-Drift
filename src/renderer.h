#pragma once

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
#include "pc_renderer.h"
class PC_Renderer;
typedef PC_Renderer Renderer;
#elif defined(_PLATFORM_PS4)
#include "ps4_renderer.h"
class PS4_Renderer;
typedef PS4_Renderer Renderer;
#else

#endif
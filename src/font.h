#pragma once
#include <string>

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
#include "pc_font.h"
class PC_Font;
typedef PC_Font Font;
#elif defined(_PLATFORM_PS4)
#include "ps4_font.h"
class PS4_Font;
typedef PS4_Font Font;
#else

#endif
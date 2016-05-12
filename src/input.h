#pragma once
#include "genericInput.h"
#include <string>
#include <vector>

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
#include "pc_input.h"
class PC_Input;
typedef PC_Input Input;
#elif defined(_PLATFORM_PS4)
#include "ps4_input.h"
class PS4_Input;
typedef PS4_Input Input;
#else

#endif

void Update(float delta);

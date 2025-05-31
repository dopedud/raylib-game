#pragma once

#include <cstdarg>

#define MAX_TRACELOG_MSG_LENGTH 1024

/* 
** NOTE:
** When trying to debug, use Raylib's built-in TraceLog() function to print C-formatted debug messages along with
** timestamps. Example usage: 
** 
** TraceLog(LOG_DEBUG, "formatted text means you can insert a %s in a middle of a sentence", <variable's value>);
**
*/

constexpr int SCREEN_WIDTH { 1280 };
constexpr int SCREEN_HEIGHT { 720 };

constexpr int TARGET_FPS { 512 };

constexpr float TEXELS_PER_UNIT { 32.0f };

//physics settings
/*
** NOTE: The time step and the sub-step count are related. As the time step decreases, the size of the sub-steps also 
** decreases. For example, at 60Hz time step and 4 sub-steps, the sub-steps operate at 240Hz. With 8 sub-steps the 
** sub-step is 480Hz.
*/

constexpr float GRAVITY_ACCELERATION { -1.0f };
constexpr float TIMESTEP { 1.0f / 60.0f };
constexpr int SUBSTEP_COUNT { 4 };

void initialise();

void TimestampLogCallback(int logType, const char *text, va_list args);
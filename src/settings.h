#pragma once

#include <cstdarg>

#define MAX_TRACELOG_MSG_LENGTH 1024

constexpr int SCREEN_WIDTH { 1280 };
constexpr int SCREEN_HEIGHT { 720 };

constexpr float TEXELS_PER_UNIT { 32.0f };

void initialise();

void TimestampLogCallback(int logType, const char *text, va_list args);
#pragma once

#include <cstdarg>

#define MAX_TRACELOG_MSG_LENGTH 1024

void TimestampLogCallback(int logType, const char *text, va_list args);
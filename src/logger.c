#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "raylib.h"

#include "logger.h"

void TimestampLogCallback(int logType, const char *text, va_list args)
{
    char buffer[MAX_TRACELOG_MSG_LENGTH] = { 0 };
    
    // get current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    // format time as HH:MM:SS
    strftime(buffer, sizeof(buffer), "[%H:%M:%S]", tm_info);

    switch (logType)
    {
        case LOG_TRACE:     strcat(buffer, "[TRACE]     - "); break;
        case LOG_DEBUG:     strcat(buffer, "[DEBUG]     - "); break;
        case LOG_INFO:      strcat(buffer, "[INFO]      - "); break;
        case LOG_WARNING:   strcat(buffer, "[WARNING]   - "); break;
        case LOG_ERROR:     strcat(buffer, "[ERROR]     - "); break;
        case LOG_FATAL:     strcat(buffer, "[FATAL]     - "); break;
        default: break;
    }
    
    // print the formatted message with timestamp and log level
    // printf("[%s][%s]: %s\n", timeStr, logType, buffer);

    unsigned int text_size = (unsigned int)strlen(text);
    memcpy(buffer + strlen(buffer), text, (text_size < (MAX_TRACELOG_MSG_LENGTH - 12)) ? text_size : (MAX_TRACELOG_MSG_LENGTH - 12));
    strcat(buffer, "\n");
    vprintf(buffer, args);
    fflush(stdout);

    if (logType == LOG_FATAL) exit(EXIT_FAILURE);  // If fatal logging, exit program
}
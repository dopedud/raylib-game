#include "settings.h"

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <ctime>

#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "rlgl.h"
#include "imgui.h"
#include "rlImGui.h"

#include "resource_manager.h"

void settings::initialise()
{
    SetTraceLogCallback(TimestampLogCallback);

    ChangeDirectory(GetApplicationDirectory());

    InitWindow(GENERAL::INITIAL_SCREEN_WIDTH, GENERAL::INITIAL_SCREEN_HEIGHT, "WINDOW");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(GENERAL::INITIAL_SCREEN_WIDTH, GENERAL::INITIAL_SCREEN_HEIGHT);

    SetExitKey(KEY_NULL);

    SetTargetFPS(settings::GENERAL::TARGET_FPS);
}

void settings::deinitialise()
{
    CloseWindow();
}

void settings::TimestampLogCallback(int logType, const char *text, va_list args)
{
    char buffer[MAX_TRACELOG_MSG_LENGTH] = { 0 };
    
    // get current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    // format time as HH:MM:SS
    strftime(buffer, sizeof(buffer), "[%H:%M:%S]", tm_info);

    switch (logType)
    {
        case LOG_TRACE:     strcat(buffer, "[TRACE] - "); break;
        case LOG_DEBUG:     strcat(buffer, "[DEBUG] - "); break;
        case LOG_INFO:      strcat(buffer, "[INFO] - "); break;
        case LOG_WARNING:   strcat(buffer, "[WARNING] - "); break;
        case LOG_ERROR:     strcat(buffer, "[ERROR] - "); break;
        case LOG_FATAL:     strcat(buffer, "[FATAL] - "); break;
        default: break;
    }
    
    // print the formatted message with timestamp and log level
    unsigned int text_size = (unsigned int)strlen(text);
    memcpy(buffer + strlen(buffer), text, (text_size < (MAX_TRACELOG_MSG_LENGTH - 12)) ? text_size : (MAX_TRACELOG_MSG_LENGTH - 12));
    strcat(buffer, "\n");
    vprintf(buffer, args);
    fflush(stdout);

    // if fatal logging, exit program
    if (logType == LOG_FATAL) exit(EXIT_FAILURE);
}
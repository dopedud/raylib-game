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
    ChangeDirectory(GetApplicationDirectory());

    SetTraceLogLevel(LOG_DEBUG);
    SetTraceLogCallback(TimestampLogCallback);

    SetRandomSeed(time(NULL));

    InitWindow(GENERAL::SCREEN_WIDTH, GENERAL::SCREEN_HEIGHT, "WINDOW");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(GENERAL::SCREEN_WIDTH, GENERAL::SCREEN_HEIGHT);

    SetExitKey(KEY_NULL);

    rlImGuiSetup(true);

    ResourceManager::instance();

    ImGuiIO& imgui_io = ImGui::GetIO();
    imgui_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void settings::deinitialise()
{
    rlImGuiShutdown();
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

void settings::DrawGridY(int slices, float spacing)
{
    int halfSlices = slices/2;

    rlBegin(RL_LINES);
        for (int i = -halfSlices; i <= halfSlices; i++)
        {
            if (i == 0)
            {
                rlColor3f(0.5f, 0.5f, 0.5f);
            }
            else
            {
                rlColor3f(0.75f, 0.75f, 0.75f);
            }

            rlVertex3f((float)i*spacing, (float)-halfSlices*spacing, 0.0f);
            rlVertex3f((float)i*spacing, (float)halfSlices*spacing, 0.0f);

            rlVertex3f((float)-halfSlices*spacing, (float)i*spacing, 0.0f);
            rlVertex3f((float)halfSlices*spacing, (float)i*spacing, 0.0f);
        }
    rlEnd();
}
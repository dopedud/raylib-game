__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#include <iostream>
#include <algorithm>
#include <queue>
#include <functional>

#include <cstdlib>
#include <ctime>

extern "C"
{
    #include "raylib.h"
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

// #include "btBulletDynamicsCommon.h"

#include "logger.h"

#define MAX_BATCH_ELEMENTS  8192

constexpr int MAX_SPAWN_CIRCLES {1};

constexpr float MIN_CIRCLE_RADIUS {75.0f};
constexpr float MAX_CIRCLE_RADIUS {150.0f};

constexpr int SCREEN_WIDTH {1280};
constexpr int SCREEN_HEIGHT {720};

// durations and intervals are measured in milliseconds
constexpr int MIN_CIRCLE_DURATION {10};
constexpr int MAX_CIRCLE_DURATION {20};
constexpr int MIN_SPAWN_INTERVAL {1};
constexpr int MAX_SPAWN_INTERVAL {1};

float spawn_interval {(float)GetRandomValue(MIN_SPAWN_INTERVAL, MAX_SPAWN_INTERVAL) / 1000.0f};

void initialise();

class Circle
{
    int x;
    int y;
    float radius;
    Color color;
    float duration;

public:
    Circle(int x, int y, float radius, Color color, float duration) :
    x{x}, y{y}, radius{radius},
    color{color},
    duration{duration}
    {}

    float get_duration() const
    {
        return duration;
    }

    bool has_timer_finished()
    {
        return duration <= .0f;
    }

    void decrease_timer(float amount)
    {
        duration -= amount;
    }

    void draw_self()
    {
        DrawCircleGradient(x, y, radius, BLACK, color);
    }

    static Circle random_circle()
    {
        return Circle{GetRandomValue(0, GetScreenWidth()), GetRandomValue(0, GetScreenHeight()), 
            (float)GetRandomValue(MIN_CIRCLE_RADIUS, MAX_CIRCLE_RADIUS),
            RED, (float)GetRandomValue(MIN_CIRCLE_DURATION, MAX_CIRCLE_DURATION) / 1000.0f};
    }
};

struct CompareCircles
{
    bool operator()(const Circle& a, const Circle& b) const
    {
        return a.get_duration() < b.get_duration();
    }
};

int main(int argc, char* argv[])
{
    initialise();

    CompareCircles compare_circle {};

    std::vector<Circle> circles_container {};

    for (long i = 0; i < GetRandomValue(1, MAX_SPAWN_CIRCLES); i++)
    {
        circles_container.push_back(Circle::random_circle());
    }

    std::make_heap(circles_container.begin(), circles_container.end(), compare_circle);

    while (!WindowShouldClose())
    {
        float deltatime {GetFrameTime()};

        if (spawn_interval <= .0f)
        {
            for (int i = 0; i < GetRandomValue(1, MAX_SPAWN_CIRCLES); i++)
            {
                circles_container.push_back(Circle::random_circle());
            }

            spawn_interval = (float)GetRandomValue(MIN_SPAWN_INTERVAL, MAX_SPAWN_INTERVAL) / 1000.0f;
        }

        else spawn_interval -= deltatime;

        std::make_heap(circles_container.begin(), circles_container.end(), compare_circle);

        for (auto& circle : circles_container)
        {
            circle.decrease_timer(deltatime);
        }

        while (!circles_container.empty())
        {
            if (circles_container.back().has_timer_finished()) circles_container.pop_back();
            else break;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            if (!circles_container.empty())
            {
                for (auto& circle : circles_container) circle.draw_self();
            }

            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void initialise()
{
    ChangeDirectory(GetApplicationDirectory());

    SetTraceLogLevel(LOG_DEBUG);
    SetTraceLogCallback(TimestampLogCallback);

    SetRandomSeed(time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);
}
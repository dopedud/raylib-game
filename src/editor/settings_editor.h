#pragma once

#include <cstdarg>

#include "raylib.h"

namespace settings_editor
{
    void initialise();
    void deinitialise();
    
    void DrawGridY(int slices, float spacing);
}
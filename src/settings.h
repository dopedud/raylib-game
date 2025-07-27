/** 
 * @file
 * @brief Source file to store game's settings.
 *
 * Source file to store game's settings. It won't necessarily store player's preferences, such as game volume and size
 * of the game window.
 * 
 * @note When trying to debug, use Raylib's built-in @c TraceLog() function to print C-formatted debug messages along
 * with timestamps. Example usage:
 * 
 * @code
 * TraceLog(LOG_DEBUG, "formatted text means you can insert a %s in a middle of a sentence", string_var);
 * @endcode
 * 
 * @note
 * The time step and the sub-step count are related. As the time step decreases, the size of the sub-steps also
 * decrease. For example, at 60Hz time step and 4 sub-steps, the sub-steps operate at 240Hz. With 8 sub-steps the
 * sub-step is 480Hz. 
 */

#pragma once

#include <cstdarg>

#include "raylib.h"

#define MAX_TRACELOG_MSG_LENGTH 1024

namespace settings
{
    struct GENERAL
    {
        static constexpr int SCREEN_WIDTH { 1280 };
        static constexpr int SCREEN_HEIGHT { 720 };

        static constexpr int TARGET_FPS { 512 };

        static constexpr float TEXELS_PER_UNIT { 32.0f };
    };

    struct PHYSICS
    {
        static constexpr float GRAVITY_ACCELERATION { -9.81f };
        static constexpr float TIMESTEP { 1.0f / 60.0f };
        static constexpr int SUBSTEP_COUNT { 4 };
    };

    struct INPUT
    {
        static constexpr float TIMESTEP { 1.0f / 512.0f };
    };

    /**
     * @fn
     * @brief Game settings initialiser.
     * 
     * Initialiser to initialise game settings upon startup of the program. It mainly initialises the program's window,
     * along with additional initialisation such as setting the random seed and changing working directory to the
     * program's directory.
     */
    void initialise();

    /**
     * @private
     */
    void deinitialise();

    /**
     * @private
     */
    void TimestampLogCallback(int logType, const char *text, va_list args);

    /**
     * @private
     */
    void DrawGridY(int slices, float spacing);
}
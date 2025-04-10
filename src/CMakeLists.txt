cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(raylib-game)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# dependencies
set(RAYLIB_VERSION 5.5)
set(RAYGUI_VERSION 4.0)

find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
find_package(raygui ${RAYGUI_VERSION} QUIET) # QUIET or REQUIRED

include(FetchContent)

if (NOT raylib_FOUND) # if there's none, fetch and build raylib
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )

    FetchContent_GetProperties(raylib)

    if (NOT raylib_POPULATED) # have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

if (NOT raygui_FOUND)
    FetchContent_Declare(
        raygui
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYGUI_VERSION}.tar.gz
    )

    FetchContent_GetProperties(raygui)

    if (NOT raygui_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raygui)
        set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE)
    endif()
endif()

add_library(raygui_header INTERFACE)
target_include_directories(raygui_header INTERFACE ${raygui_SOURCE_DIR}/src)

# copy resources
if(EXISTS "${CMAKE_SOURCE_DIR}/shaders")
    file(COPY 
        ${CMAKE_SOURCE_DIR}/shaders
    DESTINATION ${CMAKE_BINARY_DIR}/release)
endif()

# add project as executable
add_executable(${PROJECT_NAME} main.c)
set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib raygui_header)

# Set the output directory just for this target
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release
)

# create a custom target for full clean
# NOTE: this will throw errors since we're deleting the very files needed to build itself; this is expected, and should
# then be proceed with configuration step as per usual
add_custom_target(full_clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}
    COMMENT "Removing build directory and rebuilding from scratch"
)

# web configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

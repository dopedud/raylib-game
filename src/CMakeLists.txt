cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(raylib-game)
set(EXECUTABLE_NAME main)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# dependencies
set(RAYLIB_VERSION 5.5)
set(RAYGUI_VERSION 4.0)

find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
find_package(raygui ${RAYGUI_VERSION} QUIET) # QUIET or REQUIRED

set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/../_deps")
include(FetchContent)

# if there's none, fetch and build raylib
if (NOT raylib_FOUND)
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

# if there's none, fetch and build raygui
if (NOT raygui_FOUND)
    FetchContent_Declare(
        raygui
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYGUI_VERSION}.tar.gz
    )

    FetchContent_GetProperties(raygui)

    if (NOT raygui_POPULATED) # have we downloaded raygui yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raygui)
        set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

# use raygui header file as an interface library since raygui doesn't have its own CMakeLists.txt
# unlike raylib which does
add_library(raygui_header INTERFACE)
target_include_directories(raygui_header INTERFACE ${raygui_SOURCE_DIR}/src)

# create a custom target for full clean
# NOTE: this will throw errors since we're deleting the very files needed to build itself; this is expected, and should
# then be proceed with configuration step as per usual
add_custom_target(full_clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "removing build directory and rebuilding from scratch"
)

# (OPTIONAL) build tests
option(BUILD_TESTS OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

set(SOURCES
    main.cpp
    logger.cpp
)

# add project as executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})
set(raylib_VERBOSE 1)
target_link_libraries(${EXECUTABLE_NAME} raylib raygui_header)

# web configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES SUFFIX ".html") # tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${EXECUTABLE_NAME} "-framework IOKit")
    target_link_libraries(${EXECUTABLE_NAME} "-framework Cocoa")
    target_link_libraries(${EXECUTABLE_NAME} "-framework OpenGL")
endif()

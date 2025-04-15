cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(raylib-game)
set(EXECUTABLE_NAME main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# dependencies
set(RAYLIB_VERSION 5.5)
set(RAYGUI_VERSION 4.0)
set(BULLET_VERSION 3.25)

# raylib options
option(BUILD_EXAMPLES "Build the examples" OFF)

# bullet options
option(BUILD_BULLET_EXAMPLES "Build Bullet examples" OFF)
option(BUILD_EXTRAS "Build Bullet extras" OFF)
option(BUILD_UNIT_TESTS "Build Bullet unit tests" OFF)

set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/../_deps")
include(FetchContent)

# fetch raylib
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
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
endif()

# fetch raygui
FetchContent_Declare(
    raygui_src
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYGUI_VERSION}.tar.gz
)

FetchContent_GetProperties(raygui_src)

if (NOT raygui_src_POPULATED) # have we downloaded raygui yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raygui_src)
    set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
endif()

# fetch bullet
FetchContent_Declare(
    bullet
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/bulletphysics/bullet3/archive/refs/tags/${BULLET_VERSION}.tar.gz
)

FetchContent_GetProperties(bullet)

if (NOT bullet_POPULATED) # have we downloaded bullet yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(bullet)
    set(USE_MSVC_RUNTIME_LIBRARY_DLL    ON CACHE BOOL "" FORCE)
    set(BUILD_BULLET3                   ON CACHE BOOL "" FORCE)
    set(BUILD_EXTRAS                    OFF CACHE BOOL "" FORCE)
    set(BUILD_BULLET2_DEMOS             OFF CACHE BOOL "" FORCE)
    set(BUILD_CPU_DEMOS                 OFF CACHE BOOL "" FORCE)
    set(BUILD_OPENGL3_DEMOS             OFF CACHE BOOL "" FORCE)
    set(BUILD_UNIT_TESTS                OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS               OFF CACHE BOOL "" FORCE)
endif()

# use raygui header file as an interface library since raygui doesn't have its own CMakeLists.txt
# unlike raylib which does
add_library(raygui INTERFACE)
target_include_directories(raygui INTERFACE ${raygui_src_SOURCE_DIR}/src)
# use the compiler flag "-w" to ignore all warnings when compiling raygui
target_compile_options(raygui INTERFACE -w)

target_compile_options(bullet PRIVATE -w)

set(SOURCES
    main.cxx
)

# add project as executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})
set(raylib_VERBOSE 1)
target_link_libraries(${EXECUTABLE_NAME}
    raylib 
    raygui
    bullet
)

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

# create a custom target for full clean
# NOTE: this will throw errors since we're deleting the very files needed to build itself; this is expected, and should
# then be proceed with configuration step as per usual
add_custom_target(full_clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "removing build directory and rebuilding from scratch"
)

# create a custom target for full clean with dependency
# NOTE: this will throw errors since we're deleting the very files needed to build itself; this is expected, and should
# then be proceed with configuration step as per usual
add_custom_target(full_clean_dependency
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/../_deps
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "removing build and dependency directory and rebuilding from scratch"
)

# (OPTIONAL) build tests
option(BUILD_TESTS OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

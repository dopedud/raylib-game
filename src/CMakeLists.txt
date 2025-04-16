cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(raylib-game)
set(EXECUTABLE_NAME main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# dependencies
set(RAYLIB_VERSION 5.5)
set(RAYGUI_VERSION 4.0)
set(BULLET_VERSION 3.25)

set(BUILD_SHARED_LIBS ON CACHE BOOL "build shared libraries" FORCE)

set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/../_deps")
include(FetchContent)

# raylib options
set(BUILD_EXAMPLES OFF CACHE BOOL "build raylib examples" FORCE)

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
endif()

set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "build raygui examples" FORCE)

# fetch raygui
FetchContent_Declare(
    raygui
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYGUI_VERSION}.tar.gz
)

FetchContent_GetProperties(raygui)

if (NOT raygui_POPULATED) # have we downloaded raygui yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raygui)
endif()

set(USE_MSVC_RUNTIME_LIBRARY_DLL    ON CACHE BOOL "use MSVC runtime library" FORCE)
set(BUILD_BULLET3                   ON CACHE BOOL "build bullet3" FORCE)
set(BUILD_BULLET_EXAMPLES           OFF CACHE BOOL "build Bullet examples" FORCE)
set(BUILD_EXTRAS                    OFF CACHE BOOL "build bullet extras" FORCE)
set(BUILD_BULLET2_DEMOS             OFF CACHE BOOL "build bullet2" FORCE)
set(BUILD_CPU_DEMOS                 OFF CACHE BOOL "build bullet CPU demos" FORCE)
set(BUILD_OPENGL3_DEMOS             OFF CACHE BOOL "build bullet openGL3 demos" FORCE)
set(BUILD_UNIT_TESTS                OFF CACHE BOOL "build bullet unit tests" FORCE)

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
endif()

# use raygui header file as an interface library since raygui doesn't have its own CMakeLists.txt
# unlike raylib which does
add_library(raygui_header INTERFACE)
target_include_directories(raygui_header INTERFACE ${raygui_SOURCE_DIR}/src)
# use the compiler flag "-w" to ignore all warnings when compiling raygui
target_compile_options(raygui_header INTERFACE -w)

# use the compiler flag "-w" to ignore all warnings when compiling bullet's libraries
target_compile_options(Bullet3Common PUBLIC -w)
target_compile_options(Bullet3Geometry PUBLIC -w)
target_compile_options(Bullet3Collision PUBLIC -w)
target_compile_options(Bullet3Dynamics PUBLIC -w)
target_compile_options(BulletDynamics PUBLIC -w)
target_compile_options(BulletCollision PUBLIC -w)
target_compile_options(BulletInverseDynamics PUBLIC -w)
target_compile_options(BulletSoftBody PUBLIC -w)
target_compile_options(LinearMath PUBLIC -w)

target_compile_options(Bullet2FileLoader PUBLIC -w)
target_compile_options(Bullet3OpenCL_clew PUBLIC -w)

set(SOURCES
    main.cxx
)

# add project as executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${bullet_SOURCE_DIR}/src)
target_link_libraries(${EXECUTABLE_NAME}
    raylib
    raygui_header
    Bullet3Dynamics
    Bullet3Collision
    BulletSoftBody
    LinearMath
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

# Override output directory for our specific target
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

install(TARGETS ${EXECUTABLE_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

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

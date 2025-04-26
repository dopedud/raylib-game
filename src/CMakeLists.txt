cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(raylib-game)
set(EXECUTABLE_NAME game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# dependencies' versions
set(RAYLIB_VERSION 5.5)
set(RAYGUI_VERSION 4.0)
set(BULLET_VERSION 3.25)
set(BOX2D_VERSION 3.1.0)

# set to use shared libraries so the main executable's size remains small
set(BUILD_SHARED_LIBS ON CACHE BOOL "build shared libraries" FORCE)

# set dependencies folder
set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/../_deps")
include(FetchContent)

#########################
######################### START FETCHING DEPENDENCIES
#########################

# verbose option for FetchContent
set(FETCHCONTENT_QUIET NO)

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
    FetchContent_MakeAvailable(raylib)
endif()

# raygui options
set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "build raygui examples" FORCE)

# fetch raygui
FetchContent_Declare(
    raygui
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYGUI_VERSION}.tar.gz
)

FetchContent_GetProperties(raygui)

if (NOT raygui_POPULATED) # have we downloaded raygui yet?
    FetchContent_MakeAvailable(raygui)
endif()

# use raygui header file as an interface library since raygui doesn't have its own CMakeLists.txt
# unlike raylib which does
add_library(raygui INTERFACE)
target_include_directories(raygui INTERFACE ${raygui_SOURCE_DIR}/src)
# use the compiler flag "-w" to ignore all warnings when compiling raygui
target_compile_options(raygui INTERFACE -w)

# # bullet options
# set(USE_MSVC_RUNTIME_LIBRARY_DLL    ON CACHE BOOL "use MSVC runtime library" FORCE)
# set(BUILD_BULLET3                   ON CACHE BOOL "build bullet3" FORCE)
# set(BUILD_BULLET_EXAMPLES           OFF CACHE BOOL "build Bullet examples" FORCE)
# set(BUILD_EXTRAS                    OFF CACHE BOOL "build bullet extras" FORCE)
# set(BUILD_BULLET2_DEMOS             OFF CACHE BOOL "build bullet2" FORCE)
# set(BUILD_CPU_DEMOS                 OFF CACHE BOOL "build bullet CPU demos" FORCE)
# set(BUILD_OPENGL3_DEMOS             OFF CACHE BOOL "build bullet openGL3 demos" FORCE)
# set(BUILD_UNIT_TESTS                OFF CACHE BOOL "build bullet unit tests" FORCE)

# # fetch bullet
# FetchContent_Declare(
#     bullet
#     DOWNLOAD_EXTRACT_TIMESTAMP OFF
#     URL https://github.com/bulletphysics/bullet3/archive/refs/tags/${BULLET_VERSION}.tar.gz
# )

# FetchContent_GetProperties(bullet)

# if (NOT bullet_POPULATED) # have we downloaded bullet yet?
#     FetchContent_MakeAvailable(bullet)
# endif()

# use the compiler flag "-w" to ignore all warnings when compiling bullet's libraries
# target_compile_options(Bullet3Common PUBLIC -w)
# target_compile_options(Bullet3Geometry PUBLIC -w)
# target_compile_options(Bullet3Dynamics PUBLIC -w)
# target_compile_options(Bullet3Collision PUBLIC -w)
# target_compile_options(BulletDynamics PUBLIC -w)
# target_compile_options(BulletCollision PUBLIC -w)
# target_compile_options(BulletInverseDynamics PUBLIC -w)
# target_compile_options(BulletSoftBody PUBLIC -w)
# target_compile_options(LinearMath PUBLIC -w)

# target_compile_options(Bullet2FileLoader PUBLIC -w)
# target_compile_options(Bullet3OpenCL_clew PUBLIC -w)

# box2d options

# fetch box2d
FetchContent_Declare(
    box2d
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/erincatto/box2d/archive/refs/tags/v${BOX2D_VERSION}.tar.gz
)

FetchContent_GetProperties(box2d)

if (NOT box2d_POPULATED) # have we downloaded box2d yet?
    FetchContent_MakeAvailable(box2d)
endif()

#########################
######################### END FETCHING DEPENDENCIES
#########################

set(SOURCES
    main.cxx
    logger.cxx
)

set(LIBRARIES
    raylib
    raygui
    box2d

    # Bullet3Common
    # Bullet3Collision
    # Bullet3Geometry
    # Bullet3Dynamics
    # LinearMath

    # Bullet2FileLoader
    # Bullet3OpenCL_clew
)

# add main executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_include_directories(${EXECUTABLE_NAME} INTERFACE ${raygui_SOURCE_DIR}/src)
# target_include_directories(${EXECUTABLE_NAME} PRIVATE ${bullet_SOURCE_DIR}/src)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIBRARIES})

# web configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES SUFFIX ".html") # tell Emscripten to build an example.html file
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} 
    -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${EXECUTABLE_NAME} "-framework IOKit")
    target_link_libraries(${EXECUTABLE_NAME} "-framework Cocoa")
    target_link_libraries(${EXECUTABLE_NAME} "-framework OpenGL")
endif()

# Override output directory for main executable
# NOTE: for some reason shared libraries always created under the lib subdirectory during build, and for now there's no
# way to change this
# this is unlike during installs, where shared libraries are correctly placed along with the main executable
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../installs)

install(TARGETS ${EXECUTABLE_NAME} ${LIBRARIES}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib
)

# create a custom target to clean install files
add_custom_target(clean_install
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_INSTALL_PREFIX}
    COMMENT "removing install directory"
)

# create a custom target for full clean
# NOTE: this will throw errors since we're deleting the very files needed to build itself; this is expected, and should
# then be proceed with configuration step as per usual
add_custom_target(full_clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_INSTALL_PREFIX}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "removing build and install directory"
)

# create a custom target for full clean with dependencies
# NOTE: this will throw errors since we're deleting the very files needed to build itself; this is expected, and should
# then be proceed with configuration step as per usual
add_custom_target(full_clean_dependency
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${FETCHCONTENT_BASE_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_INSTALL_PREFIX}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "removing build, install, and dependency directory"
)

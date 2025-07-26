cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+

set(CMAKE_GENERATOR "MinGW Makefiles")

set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")
set(CMAKE_MAKE_PROGRAM "C:/msys64/ucrt64/bin/mingw32-make.exe")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# dependencies' versions
set(RAYLIB_VERSION 5.5)
set(IMGUI_VERSION 1.92.1)
set(RLIMGUI_VERSION master)
set(BULLET_VERSION 3.25)
set(BOX2D_VERSION 3.1.0)

# set to use shared libraries so the main executable's size remains small
set(BUILD_SHARED_LIBS ON CACHE BOOL "build shared libraries" FORCE)

# use vcpkg toolchain if available
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "vcpkg found, using it to download pre-built binaries for dependencies...")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    # enable vcpkg manifest mode
    set(VCPKG_MANIFEST_MODE ON)
endif()

project(raylib-game)
set(EXECUTABLE_NAME game)

# set dependencies folder
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/../_deps")
include(FetchContent)
# verbose option for FetchContent
set(FETCHCONTENT_QUIET NO)

#########################
######################### START FETCHING DEPENDENCIES
#########################

# try to find raylib via vcpkg first, fallback to FetchContent
find_package(raylib QUIET)
if(NOT raylib_FOUND)
    message(STATUS "raylib not found via vcpkg, using FetchContent")
    # raylib options
    set(BUILD_EXAMPLES OFF CACHE BOOL "build raylib examples" FORCE)

    # fetch raylib
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL "https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz"
    )
    FetchContent_MakeAvailable(raylib)
else()
    message(STATUS "raylib found via vcpkg")
endif()

# try to find imgui via vcpkg first, fallback to FetchContent
find_package(imgui QUIET)
if(NOT imgui_FOUND)
    message(STATUS "imgui not found via vcpkg, using FetchContent")
    # imgui options
    set(IMGUI_BUILD_EXAMPLES OFF CACHE BOOL "build imgui examples" FORCE)

    # fetch imgui
    FetchContent_Declare(
        imgui
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL "https://github.com/ocornut/imgui/archive/refs/tags/v${IMGUI_VERSION}.tar.gz"
    )
    FetchContent_MakeAvailable(imgui)

    # create imgui library from source files
    set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    )

    add_library(imgui SHARED ${IMGUI_SOURCES})
    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
    target_compile_options(imgui PUBLIC -w)
else()
    message(STATUS "imgui found via vcpkg")
endif()

# fetch rlimgui
FetchContent_Declare(
    rlimgui
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL "https://github.com/raylib-extras/rlImGui/archive/refs/heads/${RLIMGUI_VERSION}.tar.gz"
)
FetchContent_MakeAvailable(rlimgui)

# create rlimgui library
add_library(rlimgui SHARED ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
target_include_directories(rlimgui PUBLIC ${rlimgui_SOURCE_DIR})
target_link_libraries(rlimgui PUBLIC raylib imgui)
target_compile_options(rlimgui PUBLIC -w)

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
# FetchContent_MakeAvailable(bullet)


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

# try to find box2d via vcpkg first, fallback to FetchContent
find_package(box2d QUIET)
if(NOT box2d_FOUND)
    message(STATUS "box2d not found via vcpkg, using FetchContent")
    # box2d options

    # fetch box2d
    FetchContent_Declare(
        box2d
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL "https://github.com/erincatto/box2d/archive/refs/tags/v${BOX2D_VERSION}.tar.gz"
    )
    FetchContent_MakeAvailable(box2d)
else()
    message(STATUS "box2d found via vcpkg")
endif()

#########################
######################### END FETCHING DEPENDENCIES
#########################

# set your sources here
set(SOURCE
    main.cxx
    settings.cxx

    resource_manager.cxx
    state_manager.cxx

    player.cxx
    animated_model.cxx
    camera_controller.cxx
)

# set your tests here
set(TEST
    main.cxx
)

set(LIBRARIES
    raylib
    imgui::imgui
    rlimgui
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
add_executable(${EXECUTABLE_NAME} "${CMAKE_SOURCE_DIR}/src/${SOURCE}")
# target_include_directories(${EXECUTABLE_NAME} PRIVATE ${bullet_SOURCE_DIR}/src)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIBRARIES})

# checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${EXECUTABLE_NAME} "-framework IOKit")
    target_link_libraries(${EXECUTABLE_NAME} "-framework Cocoa")
    target_link_libraries(${EXECUTABLE_NAME} "-framework OpenGL")
endif()

# Override output directory for main executable
# NOTE: for some reason shared libraries are created in their respective build directories (for example, the
# libraylib.dll file under raylib's build directory) and not copied into the executable's directory
# this is unlike during installs, where shared libraries are correctly placed along with the main executable
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_INSTALL_MESSAGE LAZY)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../installs")

install(TARGETS ${EXECUTABLE_NAME} ${LIBRARIES}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib
)

install(DIRECTORY resources/ DESTINATION resources)

# create a custom target to clean (remove) install files
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

# create a custom target for full clean including dependencies
# NOTE: this will throw errors since we're deleting the very files needed to build itself; this is expected, and should
# then be proceed with configuration step as per usual
add_custom_target(full_clean_dependency
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${FETCHCONTENT_BASE_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_INSTALL_PREFIX}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "removing build, install, and dependency directory"
)

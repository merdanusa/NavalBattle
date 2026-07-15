cmake_minimum_required(VERSION 3.15)
project(NavalBattle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(FETCHCONTENT_BASE_DIR "C:/Libraries/_deps")

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(NavalBattle
    NavalBattle.cpp
    NavalBattle.h
)

target_link_libraries(NavalBattle PRIVATE raylib)

if(WIN32)
    target_link_libraries(NavalBattle PRIVATE winmm)
endif()
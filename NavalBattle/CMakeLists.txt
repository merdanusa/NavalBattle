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
    "main.cpp"
    "src/CellState.h" "src/Board.h" "src/Board.cpp"
    "assets/ships/s_h.png"   "assets/ships/s_v.png"
    "assets/ships/d_h.png"   "assets/ships/d_v.png"
    "assets/ships/b_h.png"  "assets/ships/b_v.png"
    "assets/ships/c_h.png"  "assets/ships/c_v.png"
)

target_link_libraries(NavalBattle PRIVATE raylib)

if(WIN32)
    target_link_libraries(NavalBattle PRIVATE winmm)
endif()
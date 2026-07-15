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
    "src/CellState.h"
    "src/Constants.h"
    "src/GameState.h"
    "src/Board.h"
    "src/Board.cpp"
    "src/ShipTextures.h"
    "src/ShipTextures.cpp"
    "src/InputUtils.h"
    "src/InputUtils.cpp"
    "src/AI.h"
    "src/AI.cpp"
    "src/Renderer.h"
    "src/Renderer.cpp"
)

target_link_libraries(NavalBattle PRIVATE raylib)

if(WIN32)
    target_link_libraries(NavalBattle PRIVATE winmm)
endif()

add_custom_command(TARGET NavalBattle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:NavalBattle>/assets"
)
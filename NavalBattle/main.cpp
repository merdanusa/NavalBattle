#include <raylib.h>

enum class CellState {
    Empty,
    Ship,
    Hit,
    Miss
};

int main() {
    InitWindow(800, 600, "let's gooo");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawText("yo this is raylib", 200, 300, 20, YELLOW);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
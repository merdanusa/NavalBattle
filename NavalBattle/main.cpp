#include "raylib.h"

const int BoardSize = 8;
const int CellSize = 80;
const int ScreenWidth = BoardSize * CellSize;
const int ScreenHeight = BoardSize * CellSize;

void DrawGrid() {
    for (int row = 0; row < BoardSize; row++) {
        for (int col = 0; col < BoardSize; col++) {
            int x = col * CellSize;
            int y = row * CellSize;

            DrawRectangle(x, y, CellSize, CellSize, RAYWHITE);
            DrawRectangleLines(x, y, CellSize, CellSize, DARKGRAY);
        }
    }
}

int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Naval Battle");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawGrid();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
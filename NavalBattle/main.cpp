#include "raylib.h"
#include <iostream>

const int BoardSize = 8;
const int CellSize = 80;
const int LabelMargin = 30;
const int ScreenWidth = BoardSize * CellSize + LabelMargin;
const int ScreenHeight = BoardSize * CellSize + LabelMargin;

void DrawGrid() {
    for (int row = 0; row < BoardSize; row++) {
        for (int col = 0; col < BoardSize; col++) {
            int x = col * CellSize + LabelMargin;
            int y = row * CellSize + LabelMargin;

            DrawRectangle(x, y, CellSize, CellSize, RAYWHITE);
            DrawRectangleLines(x, y, CellSize, CellSize, DARKGRAY);
        }
    }
}

void DrawLabels() {
    for (int col = 0; col < BoardSize; col++) {
        const char* label = TextFormat("%d", col + 1);
        int x = LabelMargin + col * CellSize + CellSize / 2 - 5;
        DrawText(label, x, 5, 20, BLACK);
    }

    for (int row = 0; row < BoardSize; row++) {
        char letter = 'A' + row;
        const char* label = TextFormat("%c", letter);
        int y = LabelMargin + row * CellSize + CellSize / 2 - 10;
        DrawText(label, 5, y, 20, BLACK);
    }
}

bool MouseToGrid(Vector2 mouse, int& outRow, int& outCol) {
    if (mouse.x < LabelMargin || mouse.y < LabelMargin) return false;

    int col = (mouse.x - LabelMargin) / CellSize;
    int row = (mouse.y - LabelMargin) / CellSize;

    if (col < 0 || col >= BoardSize || row < 0 || row >= BoardSize) return false;

    outRow = row;
    outCol = col;
    return true;
}

int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Naval Battle");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            int row, col;

            if (MouseToGrid(mouse, row, col)) {
                char letter = 'A' + row;
                std::cout << "Clicked: " << letter << (col + 1) << "\n";
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawGrid();
        DrawLabels();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
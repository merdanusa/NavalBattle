#include "raylib.h"
#include "src/Board.h"

const int BoardSize = 8;
const int CellSize = 70;
const int LabelMargin = 30;
const int BoardGap = 60;

const int ScreenWidth = (BoardSize * CellSize + LabelMargin) * 2 + BoardGap;
const int ScreenHeight = BoardSize * CellSize + LabelMargin + 40;

enum class Turn {
    Player,
    Enemy
};

void DrawGrid(const Board& board, int originX, int originY, bool revealShips) {
    for (int row = 0; row < BoardSize; row++) {
        for (int col = 0; col < BoardSize; col++) {
            int x = originX + col * CellSize;
            int y = originY + row * CellSize;

            Color fillColor = RAYWHITE;
            CellState cell = board.GetCell(row, col);

            if (cell == CellState::Ship)  fillColor = revealShips ? SKYBLUE : RAYWHITE;
            if (cell == CellState::Hit)   fillColor = RED;
            if (cell == CellState::Miss)  fillColor = LIGHTGRAY;

            DrawRectangle(x, y, CellSize, CellSize, fillColor);
            DrawRectangleLines(x, y, CellSize, CellSize, DARKGRAY);
        }
    }
}

void DrawLabels(int originX, int originY) {
    for (int col = 0; col < BoardSize; col++) {
        const char* label = TextFormat("%d", col + 1);
        int x = originX + col * CellSize + CellSize / 2 - 5;
        DrawText(label, x, originY - 25, 20, BLACK);
    }

    for (int row = 0; row < BoardSize; row++) {
        char letter = 'A' + row;
        const char* label = TextFormat("%c", letter);
        int y = originY + row * CellSize + CellSize / 2 - 10;
        DrawText(label, originX - 20, y, 20, BLACK);
    }
}

bool MouseToGrid(Vector2 mouse, int originX, int originY, int& outRow, int& outCol) {
    float localX = mouse.x - originX;
    float localY = mouse.y - originY;

    if (localX < 0 || localY < 0) return false;

    int col = localX / CellSize;
    int row = localY / CellSize;

    if (col < 0 || col >= BoardSize || row < 0 || row >= BoardSize) return false;

    outRow = row;
    outCol = col;
    return true;
}

int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Naval Battle");
    SetTargetFPS(60);

    Board playerBoard;
    Board enemyBoard;

    playerBoard.PlaceShip(1, 1);
    playerBoard.PlaceShip(4, 6);

    enemyBoard.PlaceShip(2, 3);
    enemyBoard.PlaceShip(5, 7);

    Turn turn = Turn::Player;

    int playerOriginX = LabelMargin + 20;
    int playerOriginY = LabelMargin + 40;

    int enemyOriginX = playerOriginX + BoardSize * CellSize + BoardGap;
    int enemyOriginY = playerOriginY;

    while (!WindowShouldClose()) {
        if (turn == Turn::Player && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            int row, col;

            if (MouseToGrid(mouse, enemyOriginX, enemyOriginY, row, col)) {
                enemyBoard.Shoot(row, col);
                turn = Turn::Enemy;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawGrid(playerBoard, playerOriginX, playerOriginY, true);
        DrawLabels(playerOriginX, playerOriginY);

        DrawGrid(enemyBoard, enemyOriginX, enemyOriginY, false);
        DrawLabels(enemyOriginX, enemyOriginY);

        const char* turnText = (turn == Turn::Player) ? "Your turn" : "Enemy turn";
        DrawText(turnText, ScreenWidth / 2 - 50, 5, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
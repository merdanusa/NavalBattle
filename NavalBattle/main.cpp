#include "raylib.h"
#include "src/Board.h"
#include <cstdlib>
#include <string>

const int BoardSize = 8;
const int CellSize = 70;
const int LabelMargin = 30;
const int BoardGap = 60;

const int ScreenWidth = (BoardSize * CellSize + LabelMargin) * 2 + BoardGap;
const int ScreenHeight = BoardSize * CellSize + LabelMargin + 40;

enum class GameState {
    Welcome,
    NameEntry,
    Placement,
    PlayerTurn,
    EnemyTurn,
    GameOver
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

bool EnemyTakeShot(Board& playerBoard, int& outRow, int& outCol) {
    int row, col;

    do {
        row = std::rand() % BoardSize;
        col = std::rand() % BoardSize;
    } while (playerBoard.GetCell(row, col) == CellState::Hit ||
        playerBoard.GetCell(row, col) == CellState::Miss);

    outRow = row;
    outCol = col;
    return playerBoard.Shoot(row, col);
}

int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Naval Battle");
    SetTargetFPS(60);

    Board playerBoard;
    Board enemyBoard;

    const int ShipsToPlace = 4;
    int placedCount = 0;

    enemyBoard.PlaceShip(2, 3);
    enemyBoard.PlaceShip(5, 7);
    enemyBoard.PlaceShip(0, 0);
    enemyBoard.PlaceShip(6, 2);

    GameState state = GameState::Welcome;
    const char* winnerText = "";

    std::string playerName = "";
    const int MaxNameLength = 12;

    std::string popupMessage = "";
    float popupTimer = 0.0f;
    const float PopupDuration = 1.2f;

    int playerOriginX = LabelMargin + 20;
    int playerOriginY = LabelMargin + 40;

    int enemyOriginX = playerOriginX + BoardSize * CellSize + BoardGap;
    int enemyOriginY = playerOriginY;

    while (!WindowShouldClose()) {
        switch (state) {
        case GameState::Welcome: {
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::NameEntry;
            }
            break;
        }

        case GameState::NameEntry: {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 125 && playerName.length() < MaxNameLength) {
                    playerName += (char)key;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty()) {
                playerName.pop_back();
            }

            if (IsKeyPressed(KEY_ENTER) && !playerName.empty()) {
                state = GameState::Placement;
            }
            break;
        }

        case GameState::Placement: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse = GetMousePosition();
                int row, col;

                if (MouseToGrid(mouse, playerOriginX, playerOriginY, row, col)) {
                    if (playerBoard.PlaceShip(row, col)) {
                        placedCount++;
                    }
                }

                if (placedCount >= ShipsToPlace) {
                    state = GameState::PlayerTurn;
                }
            }
            break;
        }

        case GameState::PlayerTurn: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse = GetMousePosition();
                int row, col;

                if (MouseToGrid(mouse, enemyOriginX, enemyOriginY, row, col)) {
                    bool hit = enemyBoard.Shoot(row, col);

                    popupMessage = hit ? "You hit a ship!" : "You missed!";
                    popupTimer = PopupDuration;

                    if (enemyBoard.AllShipsSunk()) {
                        winnerText = TextFormat("%s wins!", playerName.c_str());
                        state = GameState::GameOver;
                    }
                    else {
                        state = GameState::EnemyTurn;
                    }
                }
            }
            break;
        }

        case GameState::EnemyTurn: {
            int hitRow, hitCol;
            bool hit = EnemyTakeShot(playerBoard, hitRow, hitCol);

            popupMessage = hit ? "AI hit your ship!" : "AI missed!";
            popupTimer = PopupDuration;

            if (playerBoard.AllShipsSunk()) {
                winnerText = "AI wins!";
                state = GameState::GameOver;
            }
            else {
                state = GameState::PlayerTurn;
            }
            break;
        }

        case GameState::GameOver:
            break;
        }

        BeginDrawing();
        if (popupTimer > 0.0f) {
            popupTimer -= GetFrameTime();
        }

        ClearBackground(RAYWHITE);

        if (state == GameState::Welcome) {
            DrawText("NAVAL BATTLE", ScreenWidth / 2 - 130, ScreenHeight / 2 - 60, 40, DARKBLUE);
            DrawText("Press ENTER to start", ScreenWidth / 2 - 110, ScreenHeight / 2 + 10, 20, DARKGRAY);
        }
        else if (state == GameState::NameEntry) {
            DrawText("Enter your name:", ScreenWidth / 2 - 110, ScreenHeight / 2 - 60, 24, DARKGRAY);

            int boxX = ScreenWidth / 2 - 150;
            int boxY = ScreenHeight / 2 - 10;
            DrawRectangle(boxX, boxY, 300, 40, RAYWHITE);
            DrawRectangleLines(boxX, boxY, 300, 40, DARKGRAY);
            DrawText(playerName.c_str(), boxX + 10, boxY + 10, 20, BLACK);

            DrawText("Press ENTER to confirm", ScreenWidth / 2 - 110, boxY + 60, 18, GRAY);
        }
        else {
            DrawGrid(playerBoard, playerOriginX, playerOriginY, true);
            DrawLabels(playerOriginX, playerOriginY);

            DrawGrid(enemyBoard, enemyOriginX, enemyOriginY, false);
            DrawLabels(enemyOriginX, enemyOriginY);

            const char* statusText = "";
            switch (state) {
            case GameState::Placement:  statusText = TextFormat("Place your ships (%d/%d)", placedCount, ShipsToPlace); break;
            case GameState::PlayerTurn: statusText = TextFormat("%s's turn", playerName.c_str()); break;
            case GameState::EnemyTurn:  statusText = "Enemy turn"; break;
            case GameState::GameOver:   statusText = winnerText; break;
            default: break;
            }

            DrawText(statusText, ScreenWidth / 2 - 80, 5, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
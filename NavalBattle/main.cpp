#include "raylib.h"
#include "src/Board.h"
#include "src/Constants.h"
#include "src/GameState.h"
#include "src/ShipTextures.h"
#include "src/InputUtils.h"
#include "src/AI.h"
#include "src/Renderer.h"
#include <string>

using namespace GameConfig;

int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Naval Battle");

    ShipTextures subTextures = LoadShipTextures("s");
    ShipTextures desTextures = LoadShipTextures("d");
    ShipTextures batTextures = LoadShipTextures("b");
    ShipTextures carTextures = LoadShipTextures("c");

    ShipTextures* shipTexturesByIndex[] = {
        &carTextures, &batTextures, &batTextures,
        &desTextures, &desTextures, &desTextures,
        &subTextures, &subTextures, &subTextures, &subTextures
    };

    SetTargetFPS(60);

    Board playerBoard;
    Board enemyBoard;

    PlaceFleetRandomly(enemyBoard);

    int currentShipIndex = 0;
    Orientation currentOrientation = Orientation::Horizontal;

    GameState state = GameState::Welcome;
    const char* winnerText = "";

    std::string playerName = "";
    std::string popupMessage = "";
    float popupTimer = 0.0f;

    int playerOriginX = LabelMargin + 20;
    int playerOriginY = LabelMargin + 40;

    int enemyOriginX = playerOriginX + BoardSize * CellSize + BoardGap;
    int enemyOriginY = playerOriginY;

    int playerShots = 0;
    int playerHits = 0;
    int aiShots = 0;
    int aiHits = 0;

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
            if (IsKeyPressed(KEY_R)) {
                currentOrientation = (currentOrientation == Orientation::Horizontal) ? Orientation::Vertical : Orientation::Horizontal;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && currentShipIndex < ShipQueueSize) {
                Vector2 mouse = GetMousePosition();
                int row, col;

                if (MouseToGrid(mouse, playerOriginX, playerOriginY, row, col)) {
                    int length = ShipQueue[currentShipIndex];
                    if (playerBoard.PlaceShip(row, col, length, currentOrientation)) {
                        currentShipIndex++;
                    }
                }
            }

            if (currentShipIndex >= ShipQueueSize) {
                state = GameState::PlayerTurn;
            }
            break;
        }

        case GameState::PlayerTurn: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse = GetMousePosition();
                int row, col;

                if (MouseToGrid(mouse, enemyOriginX, enemyOriginY, row, col)) {
                    CellState existingCell = enemyBoard.GetCell(row, col);

                    if (existingCell == CellState::Hit || existingCell == CellState::Miss) {
                        popupMessage = "Already shot there!";
                        popupTimer = PopupDuration;
                    }
                    else {
                        bool sunk = false;
                        bool hit = enemyBoard.Shoot(row, col, sunk);

                        playerShots++;
                        if (hit) playerHits++;

                        if (sunk) popupMessage = "You sank a ship!";
                        else popupMessage = hit ? "You hit a ship!" : "You missed!";
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
            }
            break;
        }

        case GameState::EnemyTurn: {
            int hitRow, hitCol;
            bool sunk = false;
            bool hit = EnemyTakeShot(playerBoard, hitRow, hitCol, sunk);

            aiShots++;
            if (hit) aiHits++;

            if (sunk) popupMessage = "AI sank your ship!";
            else popupMessage = hit ? "AI hit your ship!" : "AI missed!";
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
            DrawWelcomeScreen();
        }
        else if (state == GameState::NameEntry) {
            DrawNameEntryScreen(playerName);
        }
        else {
            DrawText(playerName.c_str(), playerOriginX, playerOriginY - 55, 22, DARKBLUE);

            int aiTextWidth = MeasureText("AI", 22);
            DrawText("AI", enemyOriginX + BoardSize * CellSize - aiTextWidth, enemyOriginY - 55, 22, MAROON);

            DrawGrid(playerBoard, playerOriginX, playerOriginY, true, shipTexturesByIndex);
            DrawLabels(playerOriginX, playerOriginY);

            DrawGrid(enemyBoard, enemyOriginX, enemyOriginY, false, shipTexturesByIndex);
            DrawLabels(enemyOriginX, enemyOriginY);

            if (state == GameState::Placement && currentShipIndex < ShipQueueSize) {
                DrawPlacementPreview(playerBoard, playerOriginX, playerOriginY, currentShipIndex, currentOrientation, shipTexturesByIndex);
            }

            const char* statusText = "";
            switch (state) {
            case GameState::Placement:
                statusText = (currentShipIndex < ShipQueueSize)
                    ? TextFormat("Place your %s (size %d) - R to rotate", ShipNames[currentShipIndex], ShipQueue[currentShipIndex])
                    : "All ships placed!";
                break;
            case GameState::PlayerTurn: statusText = TextFormat("%s's turn", playerName.c_str()); break;
            case GameState::EnemyTurn:  statusText = "Enemy turn"; break;
            case GameState::GameOver:   statusText = winnerText; break;
            default: break;
            }

            DrawText(statusText, ScreenWidth / 2 - 140, 5, 20, DARKGRAY);

            const char* playerStats = TextFormat("%s: %d shots, %d hits", playerName.c_str(), playerShots, playerHits);
            const char* aiStats = TextFormat("AI: %d shots, %d hits", aiShots, aiHits);

            DrawText(playerStats, playerOriginX, ScreenHeight - 30, 18, DARKGRAY);

            int aiStatsWidth = MeasureText(aiStats, 18);
            DrawText(aiStats, enemyOriginX + BoardSize * CellSize - aiStatsWidth, ScreenHeight - 30, 18, DARKGRAY);

            DrawPopup(popupMessage, popupTimer);
        }

        EndDrawing();
    }

    UnloadShipTextures(subTextures);
    UnloadShipTextures(desTextures);
    UnloadShipTextures(batTextures);
    UnloadShipTextures(carTextures);

    CloseWindow();
    return 0;
}

#include "raylib.h"
#include "src/Board.h"
#include <cstdlib>
#include <string>
#include <vector>

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

            CellState cell = board.GetCell(row, col);
            Color fillColor = RAYWHITE;

            if (cell == CellState::Ship) fillColor = revealShips ? SKYBLUE : RAYWHITE;
            if (cell == CellState::Miss) fillColor = LIGHTGRAY;
            if (cell == CellState::Hit) {
                bool sunk = board.IsCellPartOfSunkShip(row, col);
                fillColor = sunk ? RED : (revealShips ? SKYBLUE : RAYWHITE);
            }

            DrawRectangle(x, y, CellSize, CellSize, fillColor);
            DrawRectangleLines(x, y, CellSize, CellSize, DARKGRAY);

            if (cell == CellState::Hit && !board.IsCellPartOfSunkShip(row, col)) {
                int pad = 16;
                DrawLineEx({ (float)(x + pad), (float)(y + pad) }, { (float)(x + CellSize - pad), (float)(y + CellSize - pad) }, 3.0f, RED);
                DrawLineEx({ (float)(x + CellSize - pad), (float)(y + pad) }, { (float)(x + pad), (float)(y + CellSize - pad) }, 3.0f, RED);
            }
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

bool IsPlacementValid(const Board& board, int row, int col, int length, Orientation orientation) {
    int dRow = (orientation == Orientation::Vertical) ? 1 : 0;
    int dCol = (orientation == Orientation::Horizontal) ? 1 : 0;

    for (int i = 0; i < length; i++) {
        int r = row + dRow * i;
        int c = col + dCol * i;
        if (r < 0 || r >= BoardSize || c < 0 || c >= BoardSize) return false;
        if (board.GetCell(r, c) != CellState::Empty) return false;
    }
    return true;
}

bool EnemyTakeShot(Board& playerBoard, int& outRow, int& outCol, bool& outSunk) {
    int row, col;

    do {
        row = std::rand() % BoardSize;
        col = std::rand() % BoardSize;
    } while (playerBoard.GetCell(row, col) == CellState::Hit ||
        playerBoard.GetCell(row, col) == CellState::Miss);

    outRow = row;
    outCol = col;
    return playerBoard.Shoot(row, col, outSunk);
}

void PlaceFleetRandomly(Board& board) {
    std::vector<int> shipLengths = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    for (int length : shipLengths) {
        bool placed = false;
        while (!placed) {
            int row = std::rand() % BoardSize;
            int col = std::rand() % BoardSize;
            Orientation orientation = (std::rand() % 2 == 0) ? Orientation::Horizontal : Orientation::Vertical;
            placed = board.PlaceShip(row, col, length, orientation);
        }
    }
}

int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Naval Battle");
    SetTargetFPS(60);

    Board playerBoard;
    Board enemyBoard;

    PlaceFleetRandomly(enemyBoard);

    std::vector<int> shipQueue = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    const char* shipNames[] = { "Carrier", "Battleship", "Battleship", "Destroyer", "Destroyer", "Destroyer", "Submarine", "Submarine", "Submarine", "Submarine" };
    int currentShipIndex = 0;
    Orientation currentOrientation = Orientation::Horizontal;

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

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && currentShipIndex < (int)shipQueue.size()) {
                Vector2 mouse = GetMousePosition();
                int row, col;

                if (MouseToGrid(mouse, playerOriginX, playerOriginY, row, col)) {
                    int length = shipQueue[currentShipIndex];
                    if (playerBoard.PlaceShip(row, col, length, currentOrientation)) {
                        currentShipIndex++;
                    }
                }
            }

            if (currentShipIndex >= (int)shipQueue.size()) {
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
            DrawText(playerName.c_str(), playerOriginX, playerOriginY - 55, 22, DARKBLUE);

            int aiTextWidth = MeasureText("AI", 22);
            DrawText("AI", enemyOriginX + BoardSize * CellSize - aiTextWidth, enemyOriginY - 55, 22, MAROON);

            DrawGrid(playerBoard, playerOriginX, playerOriginY, true);
            DrawLabels(playerOriginX, playerOriginY);

            DrawGrid(enemyBoard, enemyOriginX, enemyOriginY, false);
            DrawLabels(enemyOriginX, enemyOriginY);

            if (state == GameState::Placement && currentShipIndex < (int)shipQueue.size()) {
                Vector2 mouse = GetMousePosition();
                int hoverRow, hoverCol;

                if (MouseToGrid(mouse, playerOriginX, playerOriginY, hoverRow, hoverCol)) {
                    int length = shipQueue[currentShipIndex];
                    bool valid = IsPlacementValid(playerBoard, hoverRow, hoverCol, length, currentOrientation);
                    Color previewColor = valid ? Fade(YELLOW, 0.4f) : Fade(RED, 0.4f);

                    int dRow = (currentOrientation == Orientation::Vertical) ? 1 : 0;
                    int dCol = (currentOrientation == Orientation::Horizontal) ? 1 : 0;

                    for (int i = 0; i < length; i++) {
                        int r = hoverRow + dRow * i;
                        int c = hoverCol + dCol * i;
                        if (r >= 0 && r < BoardSize && c >= 0 && c < BoardSize) {
                            int px = playerOriginX + c * CellSize;
                            int py = playerOriginY + r * CellSize;
                            DrawRectangle(px, py, CellSize, CellSize, previewColor);
                        }
                    }
                }
            }

            const char* statusText = "";
            switch (state) {
            case GameState::Placement:
                if (currentShipIndex < (int)shipQueue.size()) {
                    statusText = TextFormat("Place your %s (size %d) - R to rotate", shipNames[currentShipIndex], shipQueue[currentShipIndex]);
                }
                else {
                    statusText = "All ships placed!";
                }
                break;
            case GameState::PlayerTurn: statusText = TextFormat("%s's turn", playerName.c_str()); break;
            case GameState::EnemyTurn:  statusText = "Enemy turn"; break;
            case GameState::GameOver:   statusText = winnerText; break;
            default: break;
            }

            DrawText(statusText, ScreenWidth / 2 - 140, 5, 20, DARKGRAY);

            if (popupTimer > 0.0f) {
                int textWidth = MeasureText(popupMessage.c_str(), 30);
                int boxX = ScreenWidth / 2 - textWidth / 2 - 20;
                int boxY = ScreenHeight / 2 - 25;

                DrawRectangle(boxX, boxY, textWidth + 40, 50, Fade(BLACK, 0.75f));
                DrawText(popupMessage.c_str(), boxX + 20, boxY + 12, 30, RAYWHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
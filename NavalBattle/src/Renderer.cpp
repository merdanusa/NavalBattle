#include "Renderer.h"
#include "Constants.h"
#include "InputUtils.h"

using namespace GameConfig;

void DrawGrid(const Board& board, int originX, int originY, bool revealShips, ShipTextures** texturesByIndex) {
    for (int row = 0; row < BoardSize; row++) {
        for (int col = 0; col < BoardSize; col++) {
            int x = originX + col * CellSize;
            int y = originY + row * CellSize;

            CellState cell = board.GetCell(row, col);
            Color fillColor = RAYWHITE;

            if (cell == CellState::Miss) fillColor = LIGHTGRAY;
            if (cell == CellState::Hit && board.IsCellPartOfSunkShip(row, col)) fillColor = Fade(RED, SunkOpacity);

            DrawRectangle(x, y, CellSize, CellSize, fillColor);
            DrawRectangleLines(x, y, CellSize, CellSize, DARKGRAY);
        }
    }

    for (int i = 0; i < board.GetShipCount(); i++) {
        int row, col, length;
        Orientation orientation;
        bool sunk;

        board.GetShipInfo(i, row, col, length, orientation, sunk);

        if (!revealShips && !sunk) continue;

        Texture2D tex = (orientation == Orientation::Horizontal)
            ? texturesByIndex[i]->horizontal
            : texturesByIndex[i]->vertical;

        float alpha = sunk ? SunkOpacity : 1.0f;
        DrawShipTexture(tex, row, col, length, orientation, originX, originY, CellSize, alpha);
    }

    for (int row = 0; row < BoardSize; row++) {
        for (int col = 0; col < BoardSize; col++) {
            CellState cell = board.GetCell(row, col);
            if (cell == CellState::Hit && !board.IsCellPartOfSunkShip(row, col)) {
                int x = originX + col * CellSize;
                int y = originY + row * CellSize;
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

void DrawPlacementPreview(const Board& playerBoard, int originX, int originY,
    int currentShipIndex, Orientation currentOrientation, ShipTextures** texturesByIndex) {

    Vector2 mouse = GetMousePosition();
    int hoverRow, hoverCol;

    if (!MouseToGrid(mouse, originX, originY, hoverRow, hoverCol)) return;

    int length = ShipQueue[currentShipIndex];
    bool valid = IsPlacementValid(playerBoard, hoverRow, hoverCol, length, currentOrientation);

    Texture2D previewTexture = (currentOrientation == Orientation::Horizontal)
        ? texturesByIndex[currentShipIndex]->horizontal
        : texturesByIndex[currentShipIndex]->vertical;

    if (valid) {
        DrawShipTexture(previewTexture, hoverRow, hoverCol, length, currentOrientation, originX, originY, CellSize);
    }

    Color previewColor = valid ? Fade(YELLOW, 0.25f) : Fade(RED, 0.45f);

    int dRow = (currentOrientation == Orientation::Vertical) ? 1 : 0;
    int dCol = (currentOrientation == Orientation::Horizontal) ? 1 : 0;

    for (int i = 0; i < length; i++) {
        int r = hoverRow + dRow * i;
        int c = hoverCol + dCol * i;
        if (r >= 0 && r < BoardSize && c >= 0 && c < BoardSize) {
            int px = originX + c * CellSize;
            int py = originY + r * CellSize;
            DrawRectangle(px, py, CellSize, CellSize, previewColor);
        }
    }
}

void DrawPopup(const std::string& message, float timer) {
    if (timer <= 0.0f) return;

    int textWidth = MeasureText(message.c_str(), 30);
    int boxX = ScreenWidth / 2 - textWidth / 2 - 20;
    int boxY = ScreenHeight / 2 - 25;

    DrawRectangle(boxX, boxY, textWidth + 40, 50, Fade(BLACK, 0.75f));
    DrawText(message.c_str(), boxX + 20, boxY + 12, 30, RAYWHITE);
}

void DrawWelcomeScreen() {
    DrawText("NAVAL BATTLE", ScreenWidth / 2 - 130, ScreenHeight / 2 - 60, 40, DARKBLUE);
    DrawText("Press ENTER to start", ScreenWidth / 2 - 110, ScreenHeight / 2 + 10, 20, DARKGRAY);
}

void DrawNameEntryScreen(const std::string& playerName) {
    DrawText("Enter your name:", ScreenWidth / 2 - 110, ScreenHeight / 2 - 60, 24, DARKGRAY);

    int boxX = ScreenWidth / 2 - 150;
    int boxY = ScreenHeight / 2 - 10;
    DrawRectangle(boxX, boxY, 300, 40, RAYWHITE);
    DrawRectangleLines(boxX, boxY, 300, 40, DARKGRAY);
    DrawText(playerName.c_str(), boxX + 10, boxY + 10, 20, BLACK);

    DrawText("Press ENTER to confirm", ScreenWidth / 2 - 110, boxY + 60, 18, GRAY);
}

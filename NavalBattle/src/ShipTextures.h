#pragma once
#include "raylib.h"
#include "Board.h"

struct ShipTextures {
    Texture2D horizontal;
    Texture2D vertical;
};

ShipTextures LoadShipTextures(const char* baseName);
void UnloadShipTextures(ShipTextures& textures);

void DrawShipTexture(Texture2D texture, int row, int col, int length, Orientation orientation,
    int originX, int originY, int cellSize, float alpha = 1.0f);

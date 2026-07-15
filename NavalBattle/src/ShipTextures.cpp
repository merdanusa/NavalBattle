#include "ShipTextures.h"

ShipTextures LoadShipTextures(const char* baseName) {
    ShipTextures textures;
    textures.horizontal = LoadTexture(TextFormat("assets/ships/%s_h.png", baseName));
    textures.vertical = LoadTexture(TextFormat("assets/ships/%s_v.png", baseName));
    return textures;
}

void UnloadShipTextures(ShipTextures& textures) {
    UnloadTexture(textures.horizontal);
    UnloadTexture(textures.vertical);
}

void DrawShipTexture(Texture2D texture, int row, int col, int length, Orientation orientation,
    int originX, int originY, int cellSize, float alpha) {

    int destWidth = (orientation == Orientation::Horizontal) ? length * cellSize : cellSize;
    int destHeight = (orientation == Orientation::Vertical) ? length * cellSize : cellSize;

    int x = originX + col * cellSize;
    int y = originY + row * cellSize;

    Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
    Rectangle dest = { (float)x, (float)y, (float)destWidth, (float)destHeight };

    DrawTexturePro(texture, source, dest, { 0, 0 }, 0.0f, Fade(WHITE, alpha));
}

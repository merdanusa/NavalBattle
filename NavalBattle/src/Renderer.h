#pragma once
#include "raylib.h"
#include "Board.h"
#include "ShipTextures.h"
#include <string>

void DrawGrid(const Board& board, int originX, int originY, bool revealShips, ShipTextures** texturesByIndex);
void DrawLabels(int originX, int originY);

void DrawPlacementPreview(const Board& playerBoard, int originX, int originY,
    int currentShipIndex, Orientation currentOrientation, ShipTextures** texturesByIndex);

void DrawPopup(const std::string& message, float timer);
void DrawWelcomeScreen();
void DrawNameEntryScreen(const std::string& playerName);

#pragma once
#include "raylib.h"
#include "Board.h"

bool MouseToGrid(Vector2 mouse, int originX, int originY, int& outRow, int& outCol);
bool IsPlacementValid(const Board& board, int row, int col, int length, Orientation orientation);

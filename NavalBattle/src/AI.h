#pragma once
#include "Board.h"

bool EnemyTakeShot(Board& playerBoard, int& outRow, int& outCol, bool& outSunk);
void PlaceFleetRandomly(Board& board);

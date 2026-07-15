#pragma once

namespace GameConfig {
    inline constexpr int BoardSize = 8;
    inline constexpr int CellSize = 70;
    inline constexpr int LabelMargin = 30;
    inline constexpr int BoardGap = 60;

    inline constexpr int ScreenWidth = (BoardSize * CellSize + LabelMargin) * 2 + BoardGap;
    inline constexpr int ScreenHeight = BoardSize * CellSize + LabelMargin + 40 + 40;

    inline constexpr int MaxNameLength = 12;
    inline constexpr float PopupDuration = 1.2f;
    inline constexpr float SunkOpacity = 0.5f;

    inline constexpr int ShipQueue[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    inline constexpr int ShipQueueSize = 10;

    inline const char* ShipNames[] = {
        "Carrier", "Battleship", "Battleship",
        "Destroyer", "Destroyer", "Destroyer",
        "Submarine", "Submarine", "Submarine", "Submarine"
    };
}

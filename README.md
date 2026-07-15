# Naval Battle

A classic Battleship clone built in C++ with [raylib](https://www.raylib.com/). Place your fleet, take turns firing at the enemy grid, and sink every ship before the AI sinks yours.

## Features

- Two 8x8 boards side by side — yours and the enemy's
- Ship placement with rotation (`R` key) and live valid/invalid preview
- Your fleet is rendered with real ship sprites; the enemy fleet stays hidden until a ship is sunk, at which point its sprite appears at 50% opacity over the wreck
- Simple random-shot AI opponent
- Turn-based flow: name entry → placement → player turn → enemy turn → game over
- Shot/hit stats and a popup toast for hits, misses, and sinks

## Fleet

| Ship | Size | Count |
|---|---|---|
| Carrier | 4 | 1 |
| Battleship | 3 | 2 |
| Destroyer | 2 | 3 |
| Submarine | 1 | 4 |

## Controls

- **Left Click** — place a ship / fire a shot
- **R** — rotate the ship being placed
- **Enter** — confirm name / start game
- **Backspace** — edit your name

## Project structure

```
NavalBattle/
├── main.cpp              # entry point, game loop, state transitions
├── src/
│   ├── Board.h/.cpp       # grid state, ship placement, shooting logic
│   ├── CellState.h        # Empty / Ship / Hit / Miss enum
│   ├── GameState.h        # Welcome / NameEntry / Placement / Turn / GameOver enum
│   ├── Constants.h        # board size, screen size, fleet layout, timings
│   ├── ShipTextures.h/.cpp# loading and drawing ship sprites (with opacity support)
│   ├── Renderer.h/.cpp    # grid, labels, previews, popups, menu screens
│   ├── InputUtils.h/.cpp  # mouse-to-grid conversion, placement validation
│   └── AI.h/.cpp          # enemy shot logic, random fleet placement
├── assets/
│   └── ships/             # ship sprites (horizontal & vertical, per ship type)
└── CMakeLists.txt
```

## Building

Requires CMake 3.15+ and a C++17 compiler. Raylib is fetched automatically via `FetchContent`.

```bash
cmake -B build
cmake --build build
```

The `assets` folder is copied next to the built executable automatically.

## License

MIT — do whatever you want with it.

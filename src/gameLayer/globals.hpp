#pragma once
#include <raylib.h>
#include "gameMap.h"

struct GameData
{
    GameMap gameMap;
    Camera2D camera;
    std::uint16_t selectedBlock = Block::dirt;
};
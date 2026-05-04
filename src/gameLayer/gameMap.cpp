#include "gameMap.h"
#include <asserts.h>

void GameMap::create(int w, int h)
{
    *this = {}; // reset all data

    mapData.resize(w * h);

    this->w = w;
    this->h = h;

    for (auto &e : mapData)
    {
        e = {};
    } // clear all block data
}

Block &GameMap::getBlockUnsafe(int x, int y)
{
    permaAssertCommentDevelopement(mapData.size() == w * h, "Map data not initialised");

    permaAssertCommentDevelopement(x >= 0 && x < w && y >= 0 && y < h, "getBlocksUnsafe out of bounds error");

    return mapData[y * w + x];
}

Block *GameMap::getBlockSafe(int x, int y)
{
    permaAssertCommentDevelopement(mapData.size() == w * h, "Map data not initialised");

    if (x < 0 || x >= w || y < 0 || y >= h)
    {
        return nullptr;
    }

    return &mapData[y * w + x];
}
#include "Level.h"

Level::Level(std::wstring _tiles)
{
    this->sBaseTiles = _tiles;
    resetTiles();
}

std::wstring Level::getTiles()
{
    return sUsingTiles;
}

char Level::getTile(int x, int y)
{
    return sUsingTiles[x + y * 16];
}

void Level::setTile(int x, int y, char c)
{
    sUsingTiles[x + y * 16] = c;
}

void Level::resetTiles()
{
    sUsingTiles = sBaseTiles;
}

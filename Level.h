#ifndef LEVEL_H
#define LEVEL_H

#include <string>

class Level
{
public:
    Level(std::wstring sTiles);

    std::wstring getTiles();
    char getTile(int x, int y);
    void setTile(int x, int y, char c);
    void resetTiles();
private:
    std::wstring sBaseTiles;
    std::wstring sUsingTiles;
};

#endif // LEVEL_H

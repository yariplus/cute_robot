#include "ProgramTile.h"

ProgramTile::ProgramTile(int x, int y, int w, int h, olc::Pixel c, int s, char t)
{
    this->nPosX = x;
    nPosY = y;
    nWidth = w;
    nHeight = h;
    type = t;
    color = c;
    scale = s;
}

bool ProgramTile::bContains(int x, int y)
{
    if (x > nPosX &&
        y > nPosY &&
        x < nPosX + nWidth &&
        y < nPosY + nHeight)
        return true;
    else
        return false;
}

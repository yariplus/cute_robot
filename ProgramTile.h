#ifndef PROGRAMTILE_H
#define PROGRAMTILE_H

#include "olcPixelGameEngine.h"

class ProgramTile
{
public:
    int nPosX, nPosY, nWidth, nHeight;
    char type;
    int scale;
    olc::Pixel color;
    bool clickable = true;

    ProgramTile(int x, int y, int w, int h, olc::Pixel color, int s, char t);
    bool bContains(int x, int y);
};

#endif // PROGRAMTILE_H

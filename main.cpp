#define OLC_PGE_APPLICATION
#define OLC_PGEX_GRAPHICS2D

#include "Game.h"

int main()
{
    Game cutie;

    if (cutie.Construct(800, 600, 1, 1)) cutie.Start();

    return 0;
}

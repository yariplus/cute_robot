#include "Game.h"
#include "Level.h"

Game::Game()
{
    sAppName = "Cute Robot";
}

Game::~Game()
{
}

void Game::MakeChoices(int p)
{
    aChoices.clear();

    aChoices.push_back(ProgramTile(0, 0, 96, 12, olc::DARK_BLUE, 1, 'M'));
    aChoices.push_back(ProgramTile(0, 0, 96, 12, olc::DARK_BLUE, 1, 'L'));
    aChoices.push_back(ProgramTile(0, 0, 96, 12, olc::DARK_BLUE, 1, 'R'));
    aChoices.push_back(ProgramTile(0, 0, 96, 12, olc::DARK_BLUE, 1, 'U')); // Pick
    aChoices.push_back(ProgramTile(0, 0, 96, 12, olc::DARK_BLUE, 1, 'T')); // Cut
    //aChoices.push_back(ProgramTile(0, 0, 96, 12, olc::DARK_BLUE, 1, '.'));
    aChoices.push_back(ProgramTile(0, 0, 96, 12, olc::DARK_BLUE, 1, '~'));

    switch (p)
    {
    case 0:
        aChoices.push_back(ProgramTile(0, 0, 96, 12, olc::DARK_BLUE, 1, 'A'));
    case 1:
        aChoices.push_back(ProgramTile(0, 0, 96, 12, olc::DARK_BLUE, 1, 'B'));
    default:
        break;
    }
}

bool Game::OnUserCreate()
{
    sprCutie = new olc::Sprite("./cutie.png");
    decCutie = new olc::Decal(sprCutie);

    aTitle.push_back(ProgramTile(300, 300, 150, 50, olc::CYAN, 4, 'p'));
    aTitle.push_back(ProgramTile(300, 360, 150, 50, olc::CYAN, 4, 'x'));
    //aTitle.push_back(ProgramTile(300, 360, 150, 50, olc::CYAN, 4, 'e'));

    std::wstring tiles;
    
    tiles = L"";
    tiles += L"................";
    tiles += L".E++C++C++C+++c.";
    tiles += L"......ww........";
    tiles += L"................";
    tiles += L"................";
    tiles += L"................";
    tiles += L"................";
    tiles += L"................";
    aLevels.push_back(new Level(tiles));

    tiles = L"";
    tiles += L"................";
    tiles += L".++++++S+++++++.";
    tiles += L"................";
    tiles += L".++++++.+++++++.";
    tiles += L"................";
    tiles += L".++++++++++++++.";
    tiles += L".......c........";
    tiles += L"................";
    aLevels.push_back(new Level(tiles));

    tiles = L"";
    tiles += L"................";
    tiles += L".E+++++.+++.....";
    tiles += L"......+.+.+.....";
    tiles += L"......+++.+.....";
    tiles += L"..........c.....";
    tiles += L"................";
    tiles += L"................";
    tiles += L"................";
    aLevels.push_back(new Level(tiles));

    tiles = L"";
    tiles += L"................";
    tiles += L"................";
    tiles += L"................";
    tiles += L"..+C++C++C++C+..";
    tiles += L".E++++++++++++c.";
    tiles += L"................";
    tiles += L"................";
    tiles += L"................";
    aLevels.push_back(new Level(tiles));

    tiles = L"";
    tiles += L"................";
    tiles += L"................";
    tiles += L"..wwwwwwwwwww...";
    tiles += L"..+C+wC++C+wC+..";
    tiles += L".E+w++++w+++++c.";
    tiles += L"..wwwwwwwwwww...";
    tiles += L"................";
    tiles += L"................";
    aLevels.push_back(new Level(tiles));

    tiles = L"";
    tiles += L"................";
    tiles += L".++++++S+++++++.";
    tiles += L"......www.......";
    tiles += L".+++++...++++++.";
    tiles += L"...www...www....";
    tiles += L".++++++++++++++.";
    tiles += L"......wcw.......";
    tiles += L".......w........";
    aLevels.push_back(new Level(tiles));

    int px, py = 74;
    for (int i = 0; i < 10; ++i)
    {
        aMain.push_back(ProgramTile(64 + (96 + 30) * 0, py + 16*i, 96, 12, olc::DARK_BLUE, 1, 'M'));
        aSubA.push_back(ProgramTile(64 + (96 + 30) * 1, py + 16*i, 96, 12, olc::DARK_BLUE, 1, '~'));
        aSubB.push_back(ProgramTile(64 + (96 + 30) * 2, py + 16*i, 96, 12, olc::DARK_BLUE, 1, '~'));
    }

    strIntro.push_back("     Thank you for purchasing your new      ");
    strIntro.push_back("    CutieBot 5000 (tm)!  This high-tech     ");
    strIntro.push_back("    robot can be programmed to complete     ");
    strIntro.push_back("   complex tasks.  We are confident that    ");
    strIntro.push_back(" you will find it is a very Great Machine!  ");
    strIntro.push_back("                                            ");
    strIntro.push_back("Pick up all coins and stop on the gold goal.");
    strIntro.push_back("   Cut the dark green tall grass to pass.   ");
    strIntro.push_back("  Avoid water.  Try using the least power.  ");

    Restart();

    return true;
}

void Game::DrawWorld()
{
    int mx = GetMouseX();
    int my = GetMouseY();
    bool pressed =  GetMouse(0).bPressed;

    Clear(olc::Pixel(200, 100, 180));
    FillRect(nBorderSize, nBorderSize, 800 - nBorderSize - nBorderSize, 600 - nBorderSize - nBorderSize, olc::BLACK);

    for (int x = 0; x < nWorldTilesX; ++x)
    {
        for (int y = 0; y < nWorldTilesY; ++y)
        {
            int tilex = nWorldPosX + x*(nWorldTilesWidth + nWorldTilesSpacer);
            int tiley = nWorldPosY + y*(nWorldTilesWidth + nWorldTilesSpacer);
            int tilew = nWorldTilesWidth - 1;

            if (mx > tilex &&
                my > tiley &&
                mx < tilex + tilew &&
                my < tiley + tilew)
            {
                FillRect(tilex - 2, tiley - 2, tilew + 5, tilew + 5, olc::WHITE);

                switch (pActiveLevel->getTile(x, y))
                {
                case '.':
                    //DrawString(240, 140, "Wall", olc::Pixel(222,222,222));
                    break;
                case '+':
                    //DrawString(240, 140, "Grass", olc::Pixel(222,222,222));
                    break;
                case 'c':
                    //DrawString(240, 140, "Goal", olc::Pixel(222,222,222));
                    break;
                default:
                    break;
                }
            }

            switch (pActiveLevel->getTile(x, y))
            {
            case '.':
                FillRect(tilex, tiley, tilew, tilew, olc::VERY_DARK_GREEN);
                break;
            case '+':
                FillRect(tilex, tiley, tilew, tilew, olc::GREEN);
                break;
            case 'c':
                FillRect(tilex, tiley, tilew, tilew, olc::YELLOW);
                break;
            case 'B':
                FillRect(tilex, tiley, tilew, tilew, olc::GREEN);
                FillCircle(tilex + tilew/2, tiley + tilew/2, 24, olc::VERY_DARK_GREY);
                break;
            case 'C':
                FillRect(tilex, tiley, tilew, tilew, olc::GREEN);
                FillCircle(tilex + tilew/2, tiley + tilew/2, 10, olc::Pixel(200, 200, 120));
                FillCircle(tilex + tilew/2, tiley + tilew/2, 8, olc::YELLOW);
                break;
            case 'w':
                FillRect(tilex, tiley, tilew, tilew, olc::BLUE);
                break;
            default:
                break;
            }

            DrawRect(nWorldPosX + x*(nWorldTilesWidth + nWorldTilesSpacer), nWorldPosY + y*(nWorldTilesWidth + nWorldTilesSpacer), nWorldTilesWidth - 1, nWorldTilesWidth - 1, olc::DARK_GREY);
        }
    }

    nCutiePosX = nWorldPosX + nCutieTileX * (nWorldTilesWidth + nWorldTilesSpacer) + (nWorldTilesWidth)/2;
    nCutiePosY = nWorldPosY + nCutieTileY * (nWorldTilesWidth + nWorldTilesSpacer) + (nWorldTilesWidth)/2;

    DrawPrograms();

    DrawTile(tPlayPause);

    if (pressed) {
        if (bSelecting)
        {
            UpdateChoices();

            bSelecting = false;
        } else {
            if (nMainProcess == -1)
            {
                for (int i = 0; i < aMain.size(); ++i)
                {
                    if (aMain.at(i).bContains(mx, my))
                    {
                        nSelectedTile = &aMain.at(i);
                        bSelecting = true;

                        MakeChoices(0);

                        for (int c = 0; c < aChoices.size(); ++c)
                        {
                            aChoices.at(c).nPosX = aMain.at(i).nPosX + aMain.at(i).nWidth + 6;
                            aChoices.at(c).nPosY = aMain.at(i).nPosY + (aMain.at(i).nHeight + 4) * c;
                        }
                    }
                }

                for (int i = 0; i < aSubA.size(); ++i)
                {
                    if (aSubA.at(i).bContains(mx, my))
                    {
                        nSelectedTile = &aSubA.at(i);
                        bSelecting = true;

                        MakeChoices(1);

                        for (int c = 0; c < aChoices.size(); ++c)
                        {
                            aChoices.at(c).nPosX = aSubA.at(i).nPosX + aSubA.at(i).nWidth + 6;
                            aChoices.at(c).nPosY = aSubA.at(i).nPosY + (aSubA.at(i).nHeight + 4) * c;
                        }
                    }
                }

                for (int i = 0; i < aSubB.size(); ++i)
                {
                    if (aSubB.at(i).bContains(mx, my))
                    {
                        nSelectedTile = &aSubB.at(i);
                        bSelecting = true;

                        MakeChoices(2);

                        for (int c = 0; c < aChoices.size(); ++c)
                        {
                            aChoices.at(c).nPosX = aSubB.at(i).nPosX + aSubB.at(i).nWidth + 6;
                            aChoices.at(c).nPosY = aSubB.at(i).nPosY + (aSubB.at(i).nHeight + 4) * c;
                        }
                    }
                }
            }
        }

        if (tPlayPause.bContains(mx, my))
        {
            if (!bRunningMainProgram && nBatteryCharge > 0)
            {
                bRunningMainProgram = true;
                if (!(nMainProcess >= 0))
                {
                    IterMain();
                }
                fLastProcess = fTotalTime;
                tPlayPause.type = '-';
            } else {
                bRunningMainProgram = false;
                tPlayPause.type = '+';
            }
        }

        if (tPause.bContains(mx, my))
        {
            bUsingPauseScreen = true;
        }
    }

    if (bSelecting)
    {
        DrawChoices();
    }

    DrawBattery(640, 80);

    DrawTile(tPause);
}

void Game::DrawShadowText(int x, int y, std::string s, olc::Pixel p, int scale)
{
    DrawString(x - scale, y + scale, s, olc::Pixel(40, 40, 40), scale);
    DrawString(x, y, s, p, scale);
}

void Game::DrawPrograms()
{
    DrawTile(tBackMain);
    DrawTile(tBackSubA);
    DrawTile(tBackSubB);
    DrawTile(tBackBattery);

    for (int i = 0; i < aMain.size(); ++i)
    {
        DrawTile(aMain.at(i));
        DrawTile(aSubA.at(i));
        DrawTile(aSubB.at(i));
    }

    if (nMainProcess >= 0) {
        DrawRect(aMain.at(nMainProcess).nPosX, aMain.at(nMainProcess).nPosY, 96, 12, olc::RED);
        DrawRect(aMain.at(nMainProcess).nPosX - 1, aMain.at(nMainProcess).nPosY - 1, 96 + 2, 12 + 2, olc::RED);
    }
    if (nSubAProcess >= 0) {
        DrawRect(aSubA.at(nSubAProcess).nPosX, aSubA.at(nSubAProcess).nPosY, 96, 12, olc::RED);
        DrawRect(aSubA.at(nSubAProcess).nPosX - 1, aSubA.at(nSubAProcess).nPosY - 1, 96 + 2, 12 + 2, olc::RED);
    }
    if (nSubBProcess >= 0) {
        DrawRect(aSubB.at(nSubBProcess).nPosX, aSubB.at(nSubBProcess).nPosY, 96, 12, olc::RED);
        DrawRect(aSubB.at(nSubBProcess).nPosX - 1, aSubB.at(nSubBProcess).nPosY - 1, 96 + 2, 12 + 2, olc::RED);
    }

    DrawShadowText(70 + (96 + 30) * 0, 52, "Main", olc::WHITE, 2);
    DrawShadowText(70 + (96 + 30) * 1, 52, "Sub A", olc::WHITE, 2);
    DrawShadowText(70 + (96 + 30) * 2, 52, "Sub B", olc::WHITE, 2);
}

void Game::DrawChoices()
{
    FillRect(nSelectedTile->nPosX + nSelectedTile->nWidth, aChoices.at(0).nPosY + 4, 10, 3, olc::YELLOW);
    FillRect(aChoices.at(0).nPosX - 3, aChoices.at(0).nPosY - 3, 103, 16 * aChoices.size() + 3, olc::YELLOW);

    for (int i = 0; i < aChoices.size(); ++i)
    {
        DrawTile(aChoices.at(i));
    }
}

void Game::UpdateChoices()
{
    int mx = GetMouseX();
    int my = GetMouseY();
    bool pressed =  GetMouse(0).bPressed;

    for (int i = 0; i < aChoices.size(); ++i)
    {
        if (pressed)
        {
            if (aChoices.at(i).bContains(mx, my))
            {
                nSelectedTile->type = aChoices.at(i).type;
                bSelecting = false;
            }
        }
    }
}

void Game::DrawTile(ProgramTile tile)
{
    FillRect(tile.nPosX, tile.nPosY, tile.nWidth, tile.nHeight, tile.color);
    DrawRect(tile.nPosX, tile.nPosY, tile.nWidth, tile.nHeight, olc::WHITE);

    switch (tile.type)
    {
        case '~':
            DrawShadowText(tile.nPosX + 32, tile.nPosY + 2, "PASS", olc::WHITE, tile.scale);
            break;
        case '.':
            DrawShadowText(tile.nPosX + 32, tile.nPosY + 2, "WAIT", olc::WHITE, tile.scale);
            break;
        case 'M':
            DrawShadowText(tile.nPosX + 32, tile.nPosY + 2, "MOVE", olc::WHITE, tile.scale);
            break;
        case 'L':
            DrawShadowText(tile.nPosX + 32, tile.nPosY + 2, "LEFT", olc::WHITE, tile.scale);
            break;
        case 'R':
            DrawShadowText(tile.nPosX + 32 - 4, tile.nPosY + 2, "RIGHT", olc::WHITE, tile.scale);
            break;
        case 'U':
            DrawShadowText(tile.nPosX + 32 - 8, tile.nPosY + 2, "PICK UP", olc::WHITE, tile.scale);
            break;
        case 'T':
            DrawShadowText(tile.nPosX + 32 + 4, tile.nPosY + 2, "CUT", olc::WHITE, tile.scale);
            break;
        case 'A':
            DrawString(tile.nPosX + 32 - 4, tile.nPosY + 2, "SUB A");
            break;
        case 'B':
            DrawString(tile.nPosX + 32 - 4, tile.nPosY + 2, "SUB B");
            break;
        case 'p':
            DrawShadowText(tile.nPosX + 14, tile.nPosY + 12, "PLAY", olc::WHITE, 4);
            break;
        case 'e':
            DrawShadowText(tile.nPosX + 14, tile.nPosY + 12, "EDIT", olc::WHITE, 4);
            break;
        case 'x':
            DrawShadowText(tile.nPosX + 14, tile.nPosY + 12, "EXIT", olc::WHITE, 4);
            break;
        case '=':
            DrawShadowText(tile.nPosX + 10, tile.nPosY + 6, "Start", olc::WHITE, tile.scale);
            break;
        case '+':
            DrawShadowText(tile.nPosX + 3, tile.nPosY + 6, "Resume", olc::WHITE, tile.scale);
            break;
        case '-':
            DrawShadowText(tile.nPosX + 10, tile.nPosY + 6, "Pause", olc::WHITE, tile.scale);
            break;
        case '%':
            DrawShadowText(tile.nPosX + 10, tile.nPosY + 6, "Restart", olc::WHITE, tile.scale);
            break;
        case '^':
            DrawShadowText(tile.nPosX + 6, tile.nPosY + 10, "Last Level", olc::WHITE, tile.scale);
            break;
        case '>':
            DrawShadowText(tile.nPosX + 6, tile.nPosY + 10, "Next Level", olc::WHITE, tile.scale);
            break;
        case '<':
            DrawString(tile.nPosX + 3, tile.nPosY + 3, "X", olc::WHITE, tile.scale);
            break;
        case 'm':
            DrawShadowText(tile.nPosX + 10, tile.nPosY + 3, "Quit", olc::WHITE, tile.scale);
            break;
        case 'c':
            DrawShadowText(tile.nPosX + 10, tile.nPosY + 3, "Continue", olc::WHITE, tile.scale);
            break;
        case 'i':
            DrawShadowText(tile.nPosX + 26, tile.nPosY + 3, "Start", olc::WHITE, tile.scale);
            break;
        default:
            break;
    }
}

void Game::DrawTileHighlight(ProgramTile tile)
{
    olc::Pixel color = olc::Pixel(50, 50, 180);
    DrawRect(tile.nPosX - 1, tile.nPosY - 1, tile.nWidth + 2, tile.nHeight + 2, color);
    DrawRect(tile.nPosX - 2, tile.nPosY - 2, tile.nWidth + 4, tile.nHeight + 4, color);
    DrawRect(tile.nPosX - 3, tile.nPosY - 3, tile.nWidth + 6, tile.nHeight + 6, color);
}

void Game::DrawCutie(int x, int y, char f, olc::vf2d scale)
{
    SetPixelMode(olc::Pixel::MASK);
    olc::vi2d pos = olc::vi2d(x, y);
    olc::vf2d center = olc::vf2d(sprCutie->width / 2.0f, sprCutie->height / 2.0f);

    switch (f)
    {
    case 'N':
        DrawRotatedDecal(pos, decCutie, M_PI, center, scale);
        break;
    case 'S':
        DrawRotatedDecal(pos, decCutie, 0, center, scale);
        break;
    case 'W':
        DrawRotatedDecal(pos, decCutie, M_PI/2, center, scale);
        break;
    case 'E':
        DrawRotatedDecal(pos, decCutie, -M_PI/2, center, scale);
        break;
    default:
        break;
    }

    SetPixelMode(olc::Pixel::NORMAL);
}

void Game::DrawCutie(int x, int y, char f)
{
    nCutiePosX = x;
    nCutiePosY = y;
    cCutieFacing = f;

    DrawCutie();
}

void Game::DrawCutie()
{
    SetPixelMode(olc::Pixel::MASK);
    olc::vi2d pos = olc::vi2d(nCutiePosX, nCutiePosY);
    olc::vf2d center = olc::vf2d(sprCutie->width / 2.0f, sprCutie->height / 2.0f);

    switch (cCutieFacing)
    {
    case 'N':
        DrawRotatedDecal(pos, decCutie, M_PI, center);
        break;
    case 'S':
        DrawRotatedDecal(pos, decCutie, 0, center);
        break;
    case 'W':
        DrawRotatedDecal(pos, decCutie, M_PI/2, center);
        break;
    case 'E':
        DrawRotatedDecal(pos, decCutie, -M_PI/2, center);
        break;
    default:
        break;
    }

    SetPixelMode(olc::Pixel::NORMAL);
}

void Game::CutieCut()
{
    switch (cCutieFacing)
    {
    case 'N':
        if (nCutieTileY != 0)
            if (pActiveLevel->getTile(nCutieTileX, nCutieTileY - 1) == L'.')
                pActiveLevel->setTile(nCutieTileX, nCutieTileY - 1, '+');
        break;
    case 'S':
        if (nCutieTileY < nWorldTilesY-1)
            if (pActiveLevel->getTile(nCutieTileX, nCutieTileY + 1) == L'.')
                pActiveLevel->setTile(nCutieTileX, nCutieTileY + 1, '+');
        break;
    case 'W':
        if (nCutieTileX != 0)
            if (pActiveLevel->getTile(nCutieTileX - 1, nCutieTileY) == L'.')
                pActiveLevel->setTile(nCutieTileX - 1, nCutieTileY, L'+');
        break;
    case 'E':
        if (nCutieTileX < nWorldTilesX-1)
            if (pActiveLevel->getTile(nCutieTileX + 1, nCutieTileY) == L'.')
                pActiveLevel->setTile(nCutieTileX + 1, nCutieTileY, L'+');
        break;
    default:
        break;
    }
}

void Game::CutieMove()
{
    switch (cCutieFacing)
    {
    case 'N':
        if (nCutieTileY != 0)
            if (pActiveLevel->getTile(nCutieTileX, nCutieTileY - 1) != '.'
                && pActiveLevel->getTile(nCutieTileX, nCutieTileY - 1) != 'B'
                && pActiveLevel->getTile(nCutieTileX, nCutieTileY - 1) != 'C')
                nCutieTileY--;
        break;
    case 'S':
        if (nCutieTileY < nWorldTilesY-1)
            if (pActiveLevel->getTile(nCutieTileX, nCutieTileY + 1) != '.'
                && pActiveLevel->getTile(nCutieTileX, nCutieTileY + 1) != 'B'
                && pActiveLevel->getTile(nCutieTileX, nCutieTileY + 1) != 'C')
                nCutieTileY++;
        break;
    case 'W':
        if (nCutieTileX != 0)
            if (pActiveLevel->getTile(nCutieTileX - 1, nCutieTileY) != '.'
                && pActiveLevel->getTile(nCutieTileX - 1, nCutieTileY) != 'B'
                && pActiveLevel->getTile(nCutieTileX - 1, nCutieTileY) != 'C')
                nCutieTileX--;
        break;
    case 'E':
        if (nCutieTileX < nWorldTilesX-1)
            if (pActiveLevel->getTile(nCutieTileX + 1, nCutieTileY) != '.'
                && pActiveLevel->getTile(nCutieTileX + 1, nCutieTileY) != 'B'
                && pActiveLevel->getTile(nCutieTileX + 1, nCutieTileY) != 'C')
                nCutieTileX++;
        break;
    default:
        break;
    }

    if (pActiveLevel->getTile(nCutieTileX, nCutieTileY) == 'w')
    {
        nBatteryCharge = 0;
        nMainProcess = -1;
        nSubAProcess = -1;
        nSubBProcess = -1;
        bRunningMainProgram = false;
    }
}

void Game::CutieTurnLeft()
{
    switch (cCutieFacing)
    {
    case 'N':
        cCutieFacing = 'W';
        break;
    case 'S':
        cCutieFacing = 'E';
        break;
    case 'E':
        cCutieFacing = 'N';
        break;
    case 'W':
        cCutieFacing = 'S';
        break;
    default:
        break;
    }
}

void Game::CutieTurnRight()
{
    switch (cCutieFacing)
    {
    case 'N':
        cCutieFacing = 'E';
        break;
    case 'S':
        cCutieFacing = 'W';
        break;
    case 'E':
        cCutieFacing = 'S';
        break;
    case 'W':
        cCutieFacing = 'N';
        break;
    default:
        break;
    }
}

void Game::CutiePickUp()
{
    switch (cCutieFacing)
    {
    case 'N':
        if (nCutieTileY != 0)
            if (pActiveLevel->getTile(nCutieTileX, nCutieTileY - 1) == L'C')
                pActiveLevel->setTile(nCutieTileX, nCutieTileY - 1, L'+');
        break;
    case 'S':
        if (nCutieTileY < nWorldTilesY-1)
            if (pActiveLevel->getTile(nCutieTileX, nCutieTileY + 1) == L'C')
                pActiveLevel->setTile(nCutieTileX, nCutieTileY + 1, L'+');
        break;
    case 'W':
        if (nCutieTileX != 0)
            if (pActiveLevel->getTile(nCutieTileX - 1, nCutieTileY) == L'C')
                pActiveLevel->setTile(nCutieTileX - 1, nCutieTileY, L'+');
        break;
    case 'E':
        if (nCutieTileX < nWorldTilesX-1)
            if (pActiveLevel->getTile(nCutieTileX + 1, nCutieTileY) == L'C')
                pActiveLevel->setTile(nCutieTileX + 1, nCutieTileY, L'+');
        break;
    default:
        break;
    }
}

void Game::RunProgram(std::vector<ProgramTile> list, int process)
{
    fLastProcess = fTotalTime;

    if (process < 0) return;

    switch (list.at(process).type)
    {
    case 'M':
        CutieMove();
        break;
    case 'L':
        CutieTurnLeft();
        break;
    case 'R':
        CutieTurnRight();
        break;
    case 'A':
        RunProgram(aSubA, nSubAProcess);
        break;
    case 'B':
        RunProgram(aSubB, nSubBProcess);
        break;
    case 'T':
        CutieCut();
        break;
    case 'U':
        CutiePickUp();
        break;
    default:
        break;
    }
}

void Game::SetDelay(std::vector<ProgramTile> list, int process)
{
    if (process < 0) {
        delay = 0.1f;
        return;
    }

    switch (list.at(process).type)
    {
    case 'M':
        delay = 0.6f;
        break;
    case 'L':
        delay = 0.6f;
        break;
    case 'R':
        delay = 0.6f;
        break;
    case 'A':
        SetDelay(aSubA, nSubAProcess);
        break;
    case 'B':
        SetDelay(aSubB, nSubBProcess);
        break;
    case '.':
        delay = 0.6f;
        break;
    case '~':
        delay = 0.1f;
        break;
    default:
        break;
    }
}

bool Game::IterMain()
{
    if (nMainProcess == -1)
    {
        nMainProcess = 0;
        return false;
    }

    if (nMainProcess == 10)
    {
        nMainProcess = -1;
        return true;
    }

    if (aMain.at(nMainProcess).type == 'A')
    {
        if (IterSubA()) 
        {
            nMainProcess++;
            return IterMain();
        } else {
            return false;
        }
    } else
    if (aMain.at(nMainProcess).type == 'B')
    {
        if (IterSubB())
        {
            nMainProcess++;
            return IterMain();
        } else {
            return false;
        }
    } else
    {
        nMainProcess++;

        if (nMainProcess == 10)
        {
            nMainProcess = -1;
            return true;
        } else {
            return false;
        }
    }
}

bool Game::IterSubA()
{
    if (nSubAProcess == -1) 
    {
        nSubAProcess = 0;
        return false;
    }

    if (nSubAProcess == 10)
    {
        nSubAProcess = -1;
        return true;
    }

    if (aSubA.at(nSubAProcess).type == 'B')
    {
        if (IterSubB())
        {
            nSubAProcess++;
            return IterSubA();
        } else {
            return false;
        }
    } else
    {
        nSubAProcess++;

        if (nSubAProcess == 10)
        {
            nSubAProcess = -1;
            return true;
        } else {
            return false;
        }
    }
}

bool Game::IterSubB()
{
    nSubBProcess++;

    if (nSubBProcess == 10)
    {
        nSubBProcess = -1;
        return true;
    } else {
        return false;
    }
}

void Game::UpdateTitleScreen()
{
    int mx = GetMouseX();
    int my = GetMouseY();

    Clear(olc::Pixel(200, 100, 180));

    DrawShadowText(180, 100, "Cute Robot", olc::WHITE, 6);

    bool pressed =  GetMouse(0).bPressed;

    for (int i = 0; i < aTitle.size(); ++i)
    {
        DrawTile(aTitle.at(i));

        if (aTitle.at(i).bContains(mx, my))
        {
            DrawCutie(264, 324 + 60 * i, 'E', olc::vf2d(1.4f, 1.4f));

            if (pressed && aTitle.at(i).type == 'p') bInLevel = true;
            if (pressed && aTitle.at(i).type == 'x') bQuit = true;
        }
    }
}

void Game::DrawBattery(int x, int y)
{
    int border = 8;
    y += border * 2;

    FillRect(x + border * 2, y - border * 2, border * 3, border * 2, olc::BLACK);
    FillRect(x + 0, y + 0, border * 7, border * 9, olc::BLACK);
    FillRect(x + border, y + border, border * 5, border * 7, olc::VERY_DARK_GREY);

    olc::Pixel charge;
    switch (nBatteryCharge)
    {
    case 3:
        charge = olc::GREEN;
        break;
    case 2:
        charge = olc::YELLOW;
        break;
    case 1:
        charge = olc::RED;
        break;
    default:
        charge = olc::WHITE;
        break;
    }

    if (nBatteryCharge >= 3) FillRect(x + border * 2, y + border * 2, border * 3, border, charge);
    if (nBatteryCharge >= 2) FillRect(x + border * 2, y + border * 4, border * 3, border, charge);
    if (nBatteryCharge >= 1) FillRect(x + border * 2, y + border * 6, border * 3, border, charge);
}

void Game::DrawWinScreen()
{
    FillRect(200 - 3, 100 - 3, 400 + 6, 400 + 6, olc::WHITE);
    FillRect(200, 100, 400, 400, olc::VERY_DARK_GREY);

    if (bWon) {
        DrawShadowText(240, 240, "YOU WIN! :)", olc::WHITE, 3);
    } else {
        DrawShadowText(240, 240, "TRY AGAIN!", olc::WHITE, 3);
    }

    DrawTile(tRestart);
    DrawTile(tPrevLevel);
    DrawTile(tNextLevel);
}

void Game::ProcessWinScreen()
{
    int mx = GetMouseX();
    int my = GetMouseY();
    bool pressed =  GetMouse(0).bPressed;

    if (pressed) {
        if (tNextLevel.bContains(mx, my))
        {
            if (nLevel == aLevels.size() - 1)
            {
                nLevel = 0;
            } else {
                ++nLevel;
            }

            pActiveLevel = aLevels.at(nLevel);

            Restart();
            bWon = false;
            bUsingWinScreen = false;
        } else if (tPrevLevel.bContains(mx, my))
        {
            if (nLevel == 0)
            {
                nLevel = aLevels.size() - 1;
            } else {
                --nLevel;
            }

            pActiveLevel = aLevels.at(nLevel);

            Restart();
            bWon = false;
            bUsingWinScreen = false;
        } else if (tRestart.bContains(mx, my))
        {
            Restart();
            bWon = false;
            bUsingWinScreen = false;
        }
    }
}

void Game::DrawPauseScreen()
{
    FillRect(200 - 3, 100 - 3, 400 + 6, 400 + 6, olc::WHITE);
    FillRect(200, 100, 400, 400, olc::VERY_DARK_GREY);

    DrawTile(tRestart);
    DrawTile(tPrevLevel);
    DrawTile(tNextLevel);

    DrawTile(tMenu);
    DrawTile(tContinue);

    DrawShadowText(282, 142, "Cute Robot", olc::WHITE, 3);
}

void Game::ProcessPauseScreen()
{
    int mx = GetMouseX();
    int my = GetMouseY();
    bool pressed =  GetMouse(0).bPressed;

    if (pressed) {
        if (tNextLevel.bContains(mx, my))
        {
            if (nLevel == aLevels.size() - 1)
            {
                nLevel = 0;
            } else {
                ++nLevel;
            }

            pActiveLevel = aLevels.at(nLevel);

            Restart();
            bWon = false;
            bUsingPauseScreen = false;
        } else if (tPrevLevel.bContains(mx, my))
        {
            if (nLevel == 0)
            {
                nLevel = aLevels.size() - 1;
            } else {
                --nLevel;
            }

            pActiveLevel = aLevels.at(nLevel);

            Restart();
            bWon = false;
            bUsingPauseScreen = false;
        } else if (tRestart.bContains(mx, my))
        {
            Restart();
            bWon = false;
            bUsingPauseScreen = false;
        } else if (tMenu.bContains(mx, my))
        {
            Restart();
            bWon = false;
            bInLevel = false;
            bUsingPauseScreen = false;
        } else if (tContinue.bContains(mx, my))
        {
            bUsingPauseScreen = false;
        }
    }
}

void Game::Restart()
{
    nBatteryCharge = 3;
    pActiveLevel = aLevels.at(nLevel);
    pActiveLevel->resetTiles();

    for (int x = 0; x < nWorldTilesX; ++x)
    {
        for (int y = 0; y < nWorldTilesY; ++y)
        {
            switch (pActiveLevel->getTile(x, y))
            {
            case 'S':
                cCutieFacing = 'S';
                nCutieTileX = x;
                nCutieTileY = y;
                pActiveLevel->setTile(x, y, '+');
                break;
            case 'N':
                cCutieFacing = 'N';
                nCutieTileX = x;
                nCutieTileY = y;
                pActiveLevel->setTile(x, y, '+');
                break;
            case 'E':
                cCutieFacing = 'E';
                nCutieTileX = x;
                nCutieTileY = y;
                pActiveLevel->setTile(x, y, '+');
                break;
            case 'W':
                cCutieFacing = 'W';
                nCutieTileX = x;
                nCutieTileY = y;
                pActiveLevel->setTile(x, y, '+');
                break;
            default:
                break;
            }
        }
    }

    tPlayPause.type = '=';
    bRunningMainProgram = false;
    nMainProcess = -1;
    nSubAProcess = -1;
    nSubBProcess = -1;

    for (int i = 0; i < 10; ++i)
    {
        aMain.at(i).type = 'M';
        aSubA.at(i).type = '~';
        aSubB.at(i).type = '~';
    }
}

void Game::Refuel()
{
    nBatteryCharge--;
    tPlayPause.type = '=';
    bRunningMainProgram = false;

    if (pActiveLevel->getTile(nCutieTileX, nCutieTileY) == 'c')
    {
        int coins = 0;
        for (int x = 0; x < nWorldTilesX; ++x)
        {
            for (int y = 0; y < nWorldTilesY; ++y)
            {
                if (pActiveLevel->getTile(x, y) == 'C') coins++;
            }
        }
        if (coins == 0) bWon = true;
    }
}

bool Game::OnUserUpdate(float fElapsedTime)
{
    if (bQuit) exit(0);

    if (!bDoneIntro)
    {
        int mx = GetMouseX();
        int my = GetMouseY();
        bool pressed =  GetMouse(0).bPressed;

        Clear(olc::Pixel(200, 100, 180));

        for (int i = 0; i < strIntro.size(); ++i) DrawShadowText(56, 120 + 40 * i, strIntro.at(i), olc::WHITE, 2);

        DrawTile(tEndIntro);

        DrawRotatedDecal({380, 52}, decCutie, 0, {13, 13}, {3, 3});

        if (pressed && tEndIntro.bContains(mx, my)) bDoneIntro = true;

        return true;
    }

    if (!bInLevel) {
        UpdateTitleScreen();
        return true;
    }

    fTotalTime += fElapsedTime;

    DrawWorld();

    if (nBatteryCharge == 0 || bWon) bUsingWinScreen = true;

    if (bUsingWinScreen)
    {
        DrawWinScreen();
        ProcessWinScreen();
    } else if (bUsingPauseScreen)
    {
        DrawPauseScreen();
        ProcessPauseScreen();
    } else if (bShowingTutorial) {
    } else {
        DrawCutie();

        // TODO: Need to add additional error checking here.
        if (bRunningMainProgram && nMainProcess != -1 && !(nMainProcess >= 10))
        {
            // Wait time has elapsed.
            if (fTotalTime - fLastProcess > delay)
            {
                RunProgram(aMain, nMainProcess);
                if (IterMain())
                {
                    // done
                    Refuel();
                }
                SetDelay(aMain, nMainProcess);
            }
        }
    }

    olc::HWButton bLeft = GetMouse(0);

    if (bLeft.bPressed)
    {
        nPressedPosX = GetMouseX();
        nPressedPosY = GetMouseY();
        bPressing = true;

        for (int i = 0; i < aMain.size(); ++i)
        {
            ProgramTile tile = aMain.at(i);
            if (tile.nPosX < GetMouseX() &&
                tile.nPosY < GetMouseY() &&
                tile.nPosX + tile.nWidth > GetMouseX() &&
                tile.nPosY + tile.nHeight > GetMouseY())
            {
                bDragging = true;
                nDraggedPosX = tile.nPosX;
                nDraggedPosY = tile.nPosY;
                nDraggedTile = i;
            }
        }

        DrawString(10, 10, "Pressed");
    }

    if (bLeft.bHeld) {
        DrawString(10, 10, std::to_string(GetMouseX()) + ", " + std::to_string(GetMouseY()));

        if (bDragging && false)
        {
            DrawString(10, 30, "Dragging");
            DrawString(10, 50, std::to_string(nDraggedPosX - (nPressedPosX - GetMouseX())) + " - " + std::to_string(nPressedPosX));
            aMain.at(nDraggedTile).nPosX = nDraggedPosX - (nPressedPosX - GetMouseX());
            aMain.at(nDraggedTile).nPosY = nDraggedPosY - (nPressedPosY - GetMouseY());
        }
    }

    if (bLeft.bReleased) {
        bPressing = false;
        bDragging = false;
    }

    return true;
}

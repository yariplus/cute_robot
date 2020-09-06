#define OLC_PGE_APPLICATION
#define OLC_PGEX_GRAPHICS2D

#include "olcPixelGameEngine.h"
#include "sound.h"
#include "g2d.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

class ProgramTile
{
public:
    int nPosX, nPosY, nWidth, nHeight;
    char type;
    int scale;
    olc::Pixel color;
    bool clickable = true;

    ProgramTile(int x, int y, int w, int h, olc::Pixel c, int s, char t)
    {
        nPosX = x;
        nPosY = y;
        nWidth = w;
        nHeight = h;
        type = t;
        color = c;
        scale = s;
    }

    bool bContains(int x, int y)
    {
        if (x > nPosX &&
            y > nPosY &&
            x < nPosX + nWidth &&
            y < nPosY + nHeight)
            return true;
        else
            return false;
    }
};

class Cute_Robot : public olc::PixelGameEngine
{
private:
    bool bDoneIntro = false;
    bool bInLevel = false;
    bool bShowingTutorial = false;
    bool bQuit = false;

    vector<string> strIntro;

    vector<wstring> aLevels;
    wstring sActiveLevel;
    int nActiveLevel = 0;

    bool bUsingWinScreen = false;
    bool bUsingPauseScreen = false;

    int nBorderSize = 27;

    int nPressedPosX, nPressedPosY;
    bool bPressing = false;

    bool bDragging = false;
    int nDraggedPosX, nDraggedPosY;
    int nDraggedTile;

    int nWorldTilesX = 16;
    int nWorldTilesY = 8;
    int nWorldTilesWidth = 36;
    int nWorldTilesSpacer = 2;

    int nWorldPosX = 96;
    int nWorldPosY = 258;

    int nCutieTileX;
    int nCutieTileY;

    int nCutiePosX, nCutiePosY, nCutieEyesWidth, nCutieEyesSpacer;
    int nCutieWidth = 28;
    char cCutieFacing;

    vector<ProgramTile> aMain;
    vector<ProgramTile> aSubA;
    vector<ProgramTile> aSubB;
    vector<ProgramTile> aSubC;

    vector<ProgramTile> aChoices;

    vector<ProgramTile> aTitle;

    bool bSelecting = false;
    ProgramTile* nSelectedTile;

    char cTab = 'M';

    bool bRunningMainProgram = false;
    bool bRunningSubAProgram = false;
    bool bRunningSubBProgram = false;

    int nMainProcess = -1;
    int nSubAProcess = -1;
    int nSubBProcess = -1;

    float fTotalTime = 0.0f;
    float fLastTick = 0.0f;
    float fLastProcess = 0.0f;
    float delay = 0.6f;

    int nBatteryCharge = 3;

    ProgramTile tPlayPause = ProgramTile(600, 178, 142, 36, olc::CYAN, 3, '=');
    ProgramTile tPause = ProgramTile(773, 0, 26, 26, olc::BLACK, 3, '<');

    ProgramTile tPrevLevel = ProgramTile(230, 300, 170, 36, olc::CYAN, 2.2, '^');
    ProgramTile tNextLevel = ProgramTile(410, 300, 170, 36, olc::CYAN, 2.2, '>');
    ProgramTile tRestart = ProgramTile(280, 360, 185, 36, olc::CYAN, 3, '%');
    ProgramTile tContinue = ProgramTile(370, 420, 205, 36, olc::CYAN, 3, 'c');
    ProgramTile tMenu = ProgramTile(240, 420, 120, 36, olc::CYAN, 3, 'm');

    ProgramTile tEndIntro = ProgramTile(300, 480, 160, 36, olc::CYAN, 3, 'i');

    ProgramTile tBackBattery = ProgramTile(580, 50, 180, 180, olc::DARK_GREY, 1, ' ');
    ProgramTile tBackMain = ProgramTile(57 + (96 + 30) * 0, 45, 112, 192, olc::DARK_GREY, 1, ' ');
    ProgramTile tBackSubA = ProgramTile(57 + (96 + 30) * 1, 45, 112, 192, olc::DARK_GREY, 1, ' ');
    ProgramTile tBackSubB = ProgramTile(57 + (96 + 30) * 2, 45, 112, 192, olc::DARK_GREY, 1, ' ');

    bool bWon = false;

    bool bPlayMusic = true;

    olc::Sprite* sprCutie = nullptr;
    olc::Decal* decCutie = nullptr;

public:
    Cute_Robot()
    {
        sAppName = "Cute Robot";
    }

    ~Cute_Robot()
    {
    }

    void MakeChoices(int p)
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

    bool OnUserCreate() override
    {
        sprCutie = new olc::Sprite("./cutie.png");
        decCutie = new olc::Decal(sprCutie);

        aTitle.push_back(ProgramTile(300, 300, 150, 50, olc::CYAN, 4, 'p'));
        aTitle.push_back(ProgramTile(300, 360, 150, 50, olc::CYAN, 4, 'x'));
        //aTitle.push_back(ProgramTile(300, 360, 150, 50, olc::CYAN, 4, 'e'));

        aLevels.push_back(L"");
        aLevels.at(0) += L"................";
        aLevels.at(0) += L".E++C++C++C+++c.";
        aLevels.at(0) += L"......ww........";
        aLevels.at(0) += L"................";
        aLevels.at(0) += L"................";
        aLevels.at(0) += L"................";
        aLevels.at(0) += L"................";
        aLevels.at(0) += L"................";

        aLevels.push_back(L"");
        aLevels.at(1) += L"................";
        aLevels.at(1) += L".++++++S+++++++.";
        aLevels.at(1) += L"................";
        aLevels.at(1) += L".++++++.+++++++.";
        aLevels.at(1) += L"................";
        aLevels.at(1) += L".++++++++++++++.";
        aLevels.at(1) += L".......c........";
        aLevels.at(1) += L"................";

        aLevels.push_back(L"");
        aLevels.at(2) += L"................";
        aLevels.at(2) += L".E+++++.+++.....";
        aLevels.at(2) += L"......+.+.+.....";
        aLevels.at(2) += L"......+++.+.....";
        aLevels.at(2) += L"..........c.....";
        aLevels.at(2) += L"................";
        aLevels.at(2) += L"................";
        aLevels.at(2) += L"................";

        aLevels.push_back(L"");
        aLevels.at(3) += L"................";
        aLevels.at(3) += L"................";
        aLevels.at(3) += L"................";
        aLevels.at(3) += L"..+C++C++C++C+..";
        aLevels.at(3) += L".E++++++++++++c.";
        aLevels.at(3) += L"................";
        aLevels.at(3) += L"................";
        aLevels.at(3) += L"................";

        aLevels.push_back(L"");
        aLevels.at(4) += L"................";
        aLevels.at(4) += L"................";
        aLevels.at(4) += L"..wwwwwwwwwww...";
        aLevels.at(4) += L"..+C+wC++C+wC+..";
        aLevels.at(4) += L".E+w++++w+++++c.";
        aLevels.at(4) += L"..wwwwwwwwwww...";
        aLevels.at(4) += L"................";
        aLevels.at(4) += L"................";

        aLevels.push_back(L"");
        aLevels.at(5) += L"................";
        aLevels.at(5) += L".++++++S+++++++.";
        aLevels.at(5) += L"......www.......";
        aLevels.at(5) += L".+++++...++++++.";
        aLevels.at(5) += L"...www...www....";
        aLevels.at(5) += L".++++++++++++++.";
        aLevels.at(5) += L"......wcw.......";
        aLevels.at(5) += L".......w........";

        int px, py = 74;
        for (int i = 0; i < 10; ++i)
        {
            aMain.push_back(ProgramTile(64 + (96 + 30) * 0, py + 16*i, 96, 12, olc::DARK_BLUE, 1, 'M'));
            aSubA.push_back(ProgramTile(64 + (96 + 30) * 1, py + 16*i, 96, 12, olc::DARK_BLUE, 1, '~'));
            aSubB.push_back(ProgramTile(64 + (96 + 30) * 2, py + 16*i, 96, 12, olc::DARK_BLUE, 1, '~'));
        }

        strIntro.push_back("");
        strIntro.push_back("");
        strIntro.push_back("");
        strIntro.push_back("");
        strIntro.push_back("");
        strIntro.push_back("");
        strIntro.push_back("");
        strIntro.push_back("");
        strIntro.push_back("");

        strIntro.at(0) += ("     Thank you for purchasing your new");
        strIntro.at(1) += ("    CutieBot 5000 (tm)!  This high-tech");
        strIntro.at(2) += ("    robot can be programmed to complete");
        strIntro.at(3) += ("   complex tasks.  We are confident that");
        strIntro.at(4) += (" you will find it is a very Great Machine!");
        strIntro.at(5) += ("  ");
        strIntro.at(6) += ("Pick up all coins and stop on the gold goal.");
        strIntro.at(7) += ("  Cut the dark green tall grass to pass.");
        strIntro.at(8) += (" Avoid water.  Try using the least power.");

        Restart();

        return true;
    }

    void DrawWorld()
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

                    switch (sActiveLevel[y * nWorldTilesX + x])
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

                switch (sActiveLevel[y * nWorldTilesX + x])
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

    void DrawShadowText(int x, int y, string s, olc::Pixel p, int scale)
    {
        DrawString(x - scale, y + scale, s, olc::Pixel(40, 40, 40), scale);
        DrawString(x, y, s, p, scale);
    }

    void DrawPrograms()
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

    void DrawChoices()
    {
        FillRect(nSelectedTile->nPosX + nSelectedTile->nWidth, aChoices.at(0).nPosY + 4, 10, 3, olc::YELLOW);
        FillRect(aChoices.at(0).nPosX - 3, aChoices.at(0).nPosY - 3, 103, 16 * aChoices.size() + 3, olc::YELLOW);

        for (int i = 0; i < aChoices.size(); ++i)
        {
            DrawTile(aChoices.at(i));
        }
    }

    void UpdateChoices()
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

    void DrawTile(ProgramTile tile)
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

    void DrawTileHighlight(ProgramTile tile)
    {
        olc::Pixel color = olc::Pixel(50, 50, 180);
        DrawRect(tile.nPosX - 1, tile.nPosY - 1, tile.nWidth + 2, tile.nHeight + 2, color);
        DrawRect(tile.nPosX - 2, tile.nPosY - 2, tile.nWidth + 4, tile.nHeight + 4, color);
        DrawRect(tile.nPosX - 3, tile.nPosY - 3, tile.nWidth + 6, tile.nHeight + 6, color);
    }

    void DrawCutie(int x, int y, char f, olc::vf2d scale)
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

    void DrawCutie(int x, int y, char f)
    {
        nCutiePosX = x;
        nCutiePosY = y;
        cCutieFacing = f;

        DrawCutie();
    }

    void DrawCutie()
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

    void CutieCut()
    {
        switch (cCutieFacing)
        {
        case 'N':
            if (nCutieTileY != 0)
                if (sActiveLevel[(nCutieTileY - 1) * nWorldTilesX + nCutieTileX] == L'.')
                    sActiveLevel[(nCutieTileY - 1) * nWorldTilesX + nCutieTileX] = L'+';
            break;
        case 'S':
            if (nCutieTileY < nWorldTilesY-1)
                if (sActiveLevel[(nCutieTileY + 1) * nWorldTilesX + nCutieTileX] == L'.')
                    sActiveLevel[(nCutieTileY + 1) * nWorldTilesX + nCutieTileX] = L'+';
            break;
        case 'W':
            if (nCutieTileX != 0)
                if (sActiveLevel[nCutieTileY * nWorldTilesX + (nCutieTileX - 1)] == L'.')
                    sActiveLevel[nCutieTileY * nWorldTilesX + (nCutieTileX - 1)] = L'+';
            break;
        case 'E':
            if (nCutieTileX < nWorldTilesX-1)
                if (sActiveLevel[nCutieTileY * nWorldTilesX + (nCutieTileX + 1)] == L'.')
                    sActiveLevel[nCutieTileY * nWorldTilesX + (nCutieTileX + 1)] = L'+';
            break;
        default:
            break;
        }
    }

    void CutieMove()
    {
        switch (cCutieFacing)
        {
        case 'N':
            if (nCutieTileY != 0)
                if (sActiveLevel[(nCutieTileY - 1) * nWorldTilesX + nCutieTileX] != '.'
                 && sActiveLevel[(nCutieTileY - 1) * nWorldTilesX + nCutieTileX] != 'B'
                 && sActiveLevel[(nCutieTileY - 1) * nWorldTilesX + nCutieTileX] != 'C')
                    nCutieTileY--;
            break;
        case 'S':
            if (nCutieTileY < nWorldTilesY-1)
                if (sActiveLevel[(nCutieTileY + 1) * nWorldTilesX + nCutieTileX] != '.'
                 && sActiveLevel[(nCutieTileY - 1) * nWorldTilesX + nCutieTileX] != 'B'
                 && sActiveLevel[(nCutieTileY - 1) * nWorldTilesX + nCutieTileX] != 'C')
                    nCutieTileY++;
            break;
        case 'W':
            if (nCutieTileX != 0)
                if (sActiveLevel[(nCutieTileY) * nWorldTilesX + (nCutieTileX - 1)] != '.'
                 && sActiveLevel[(nCutieTileY) * nWorldTilesX + (nCutieTileX - 1)] != 'B'
                 && sActiveLevel[(nCutieTileY) * nWorldTilesX + (nCutieTileX - 1)] != 'C')
                    nCutieTileX--;
            break;
        case 'E':
            if (nCutieTileX < nWorldTilesX-1)
                if (sActiveLevel[(nCutieTileY) * nWorldTilesX + (nCutieTileX + 1)] != '.'
                 && sActiveLevel[(nCutieTileY) * nWorldTilesX + (nCutieTileX + 1)] != 'B'
                 && sActiveLevel[(nCutieTileY) * nWorldTilesX + (nCutieTileX + 1)] != 'C')
                    nCutieTileX++;
            break;
        default:
            break;
        }

        if (sActiveLevel[(nCutieTileY) * nWorldTilesX + nCutieTileX] == 'w')
        {
            nBatteryCharge = 0;
            nMainProcess = -1;
            nSubAProcess = -1;
            nSubBProcess = -1;
            bRunningMainProgram = false;
        }
    }

    void CutieTurnLeft()
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

    void CutieTurnRight()
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

    void CutiePickUp()
    {
        switch (cCutieFacing)
        {
        case 'N':
            if (nCutieTileY != 0)
                if (sActiveLevel[(nCutieTileY - 1) * nWorldTilesX + nCutieTileX] == L'C')
                    sActiveLevel[(nCutieTileY - 1) * nWorldTilesX + nCutieTileX] = L'+';
            break;
        case 'S':
            if (nCutieTileY < nWorldTilesY-1)
                if (sActiveLevel[(nCutieTileY + 1) * nWorldTilesX + nCutieTileX] == L'C')
                    sActiveLevel[(nCutieTileY + 1) * nWorldTilesX + nCutieTileX] = L'+';
            break;
        case 'W':
            if (nCutieTileX != 0)
                if (sActiveLevel[nCutieTileY * nWorldTilesX + (nCutieTileX - 1)] == L'C')
                    sActiveLevel[nCutieTileY * nWorldTilesX + (nCutieTileX - 1)] = L'+';
            break;
        case 'E':
            if (nCutieTileX < nWorldTilesX-1)
                if (sActiveLevel[nCutieTileY * nWorldTilesX + (nCutieTileX + 1)] == L'C')
                    sActiveLevel[nCutieTileY * nWorldTilesX + (nCutieTileX + 1)] = L'+';
            break;
        default:
            break;
        }
    }

    void RunProgram(vector<ProgramTile> list, int process)
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

    void SetDelay(vector<ProgramTile> list, int process)
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

    bool IterMain()
    {
        if (nMainProcess == -1)
        {
            nMainProcess = 0;
            return false;
        }

        if (aMain.at(nMainProcess).type == 'A')
        {
            if (IterSubA()) 
            {
                nMainProcess++;
                return IterMain();
            } else {
                return false;;
            }
        } else
        if (aMain.at(nMainProcess).type == 'B')
        {
            if (IterSubB()) 
            {
                nMainProcess++;
                return IterMain();
            } else {
                return false;;
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

    bool IterSubA()
    {
        if (nSubAProcess == -1) 
        {
            nSubAProcess = 0;
            return false;
        }

        if (aSubA.at(nSubAProcess).type == 'B')
        {
            if (IterSubB()) 
            {
                nSubAProcess++;
                return IterSubA();
            } else {
                return false;;
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

    bool IterSubB()
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

    void UpdateTitleScreen()
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

    void DrawBattery(int x, int y)
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

    void DrawWinScreen()
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

    void ProcessWinScreen()
    {
        int mx = GetMouseX();
        int my = GetMouseY();
        bool pressed =  GetMouse(0).bPressed;

        if (pressed) {
            if (tNextLevel.bContains(mx, my))
            {
                if (nActiveLevel == aLevels.size() - 1)
                {
                    nActiveLevel = 0;
                } else {
                    nActiveLevel++;
                }

                Restart();
                bWon = false;
                bUsingWinScreen = false;
            } else if (tPrevLevel.bContains(mx, my))
            {
                if (nActiveLevel == 0)
                {
                    nActiveLevel = aLevels.size() - 1;
                } else {
                    nActiveLevel--;
                }

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

    void DrawPauseScreen()
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
    
    void ProcessPauseScreen()
    {
        int mx = GetMouseX();
        int my = GetMouseY();
        bool pressed =  GetMouse(0).bPressed;

        if (pressed) {
            if (tNextLevel.bContains(mx, my))
            {
                if (nActiveLevel == aLevels.size() - 1)
                {
                    nActiveLevel = 0;
                } else {
                    nActiveLevel++;
                }

                Restart();
                bWon = false;
                bUsingPauseScreen = false;
            } else if (tPrevLevel.bContains(mx, my))
            {
                if (nActiveLevel == 0)
                {
                    nActiveLevel = aLevels.size() - 1;
                } else {
                    nActiveLevel--;
                }

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

    void Restart()
    {
        nBatteryCharge = 3;
        sActiveLevel = aLevels.at(nActiveLevel);

        for (int x = 0; x < nWorldTilesX; ++x)
        {
            for (int y = 0; y < nWorldTilesY; ++y)
            {
                switch (sActiveLevel.at(y * nWorldTilesX + x))
                {
                case 'S':
                    cCutieFacing = 'S';
                    nCutieTileX = x;
                    nCutieTileY = y;
                    sActiveLevel.at(y * nWorldTilesX + x) = '+';
                    break;
                case 'N':
                    cCutieFacing = 'N';
                    nCutieTileX = x;
                    nCutieTileY = y;
                    sActiveLevel.at(y * nWorldTilesX + x) = '+';
                    break;
                case 'E':
                    cCutieFacing = 'E';
                    nCutieTileX = x;
                    nCutieTileY = y;
                    sActiveLevel.at(y * nWorldTilesX + x) = '+';
                    break;
                case 'W':
                    cCutieFacing = 'W';
                    nCutieTileX = x;
                    nCutieTileY = y;
                    sActiveLevel.at(y * nWorldTilesX + x) = '+';
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

    void Refuel()
    {
        nBatteryCharge--;
        tPlayPause.type = '=';
        bRunningMainProgram = false;

        if (sActiveLevel.at(nCutieTileX + nCutieTileY * nWorldTilesX) == 'c')
        {
            int coins = 0;
            for (int x = 0; x < nWorldTilesX; ++x)
            {
                for (int y = 0; y < nWorldTilesY; ++y)
                {
                    if (sActiveLevel.at(y * nWorldTilesX + x) == 'C') coins++;
                }
            }
            if (coins == 0) bWon = true;
        }
    }

    bool OnUserUpdate(float fElapsedTime) override
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
            DrawString(10, 10, to_string(GetMouseX()) + ", " + to_string(GetMouseY()));

            if (bDragging && false)
            {
                DrawString(10, 30, "Dragging");
                DrawString(10, 50, to_string(nDraggedPosX - (nPressedPosX - GetMouseX())) + " - " + to_string(nPressedPosX));
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
};

int main()
{
    Cute_Robot cutie;

    if (cutie.Construct(800, 600, 1, 1)) cutie.Start();

    return 0;
}

#ifndef GAME_H
#define GAME_H

#define _USE_MATH_DEFINES
#include "olcPixelGameEngine.h"
#include "g2d.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

#include "ProgramTile.h"
#include "Level.h"

class Game : public olc::PixelGameEngine
{
private:
    bool bDoneIntro = false;
    bool bInLevel = false;
    bool bShowingTutorial = false;
    bool bQuit = false;

    std::vector<std::string> strIntro;

    std::vector<Level *> aLevels;
    Level * pActiveLevel;
    int nLevel = 0;

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

    std::vector<ProgramTile> aMain;
    std::vector<ProgramTile> aSubA;
    std::vector<ProgramTile> aSubB;
    std::vector<ProgramTile> aSubC;

    std::vector<ProgramTile> aChoices;

    std::vector<ProgramTile> aTitle;

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
    Game();
    ~Game();

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

    void MakeChoices(int p);

    void DrawWorld();
    void DrawShadowText(int x, int y, std::string s, olc::Pixel p, int scale);
    void DrawPrograms();
    void DrawChoices();
    void DrawTile(ProgramTile tile);
    void DrawTileHighlight(ProgramTile tile);
    void DrawCutie(int x, int y, char f, olc::vf2d scale);
    void DrawCutie(int x, int y, char f);
    void DrawBattery(int x, int y);
    void DrawWinScreen();
    void DrawPauseScreen();

    void UpdateChoices();
    void ProcessWinScreen();
    void ProcessPauseScreen();

    void DrawCutie();
    void CutieCut();
    void CutieMove();
    void CutieTurnLeft();
    void CutieTurnRight();
    void CutiePickUp();

    void RunProgram(std::vector<ProgramTile> list, int process);
    void SetDelay(std::vector<ProgramTile> list, int process);
    bool IterMain();
    bool IterSubA();
    bool IterSubB();

    void UpdateTitleScreen();

    void Restart();
    void Refuel();
};

#endif // GAME_H

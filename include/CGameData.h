/**************************************************************************************
Filename: CGameData.h Date: December, 2008
Purpose: Manages all game data.
**************************************************************************************/
#ifndef CGAMEDATA_H
#define CGAMEDATA_H

#include <string>
#include <vector>
#include <windows.h>
#include "CLog.h"
#include "CFileReader.h"
#include "CCSVReader.h"
#include "CGraphics.h"
#include "CTile.h"

class CGraphics;
class CTile;
class CConfigData;

class CGameData{
public:

  CGameData();
  ~CGameData();
  bool LoadConfigFile(std::string filename);
  bool SaveLevelFile(int fileNum);
  void Update(double timeDifference);
  void IncrementCredits(float health);

  float m_FPS;
  HWND m_hWnd;

  float m_posX;//position within world...used for scrolling through world
  float m_posY;

  bool m_bLeftMouseDown;
  bool m_bRightMouseDown;

  //bool LoadGameLevel(std::string filename);
  bool OpenLevel (int level, CGraphics &con);
  bool AddObject(CGraphics &con, std::string objectName, int x, int y);
  bool AddGraphicDataToLevelData(CGraphics &con);

  //variables loaded from level1.dat file
  int m_screenColorRed;  //background color
  int m_screenColorGreen;
  int m_screenColorBlue;
  int m_textureColorRed;  //sets texture/alpha color
  int m_textureColorGreen;
  int m_textureColorBlue;
  long m_worldLeft;
  long m_worldRight;
  long m_worldTop;
  long m_worldBottom;
  float m_scale;
  bool m_bGameOver;
  bool m_bPaused;
  int m_gameLevel;
  

private:
//CTile m_gameTile;
};

#endif
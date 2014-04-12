#ifndef GAME_STATE_PLAY1_H
#define GAME_STATE_PLAY1_H

#include <iomanip>
#include <sstream>
#include <string>
#include "CGameStateObject.h"
#include "CGraphics.h"
#include "CTimer.h"
#include "keystatus.h"
#include "CGameData.h"
#include "CLog.h"
//#include "//CEffect.h"
#include "CObject.h"
#include "CFizzix.h"
//#include "CAudioManager.h"
#include "CUserInterface.h"
#include "CLevel.h"
#include "CObjectManager.h"


class CGameStatePlay1 :  public CGameStateObject{
public:
  CGameStatePlay1(void);
  ~CGameStatePlay1(void);
  void Initialize();
  void Activate(CGameData &data, CConfigData &cfg, CGraphics &con);
  CGameStateObject* Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con);
  int IsLost(CGraphics &con, CConfigData &cfg);
  int Render(CGraphics &con,  CGameData &data, CConfigData &cfg);
  void ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con);
  void Resume();
  void Pause();
  void Save();
  void AddTransitionEvent(int event, CGameStateObject* p_Next);
  //static const int MAX_ROWS = 50;
  //static const int MAX_COLS = 50;

private:

  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  CTimer m_timer;
  int m_event;
  CMouse m_mouse;
  CUserInterface m_UI;
  int m_UIstate;
  //members for this game
  //bool m_bMapOn;
  //bool m_bGridOn;
  
  //************** old methods and members

  //added for BFK
  //float m_zoom;  //0.125, 0.25, 0.5, 1.0
  float m_zoomValue[4];
  int m_zoomIndex;
  //MANAGES pyro effects
  //CEffect m_effect;
  DWORD m_startTime;
  int m_effectNumber;
  int m_effectMax;

  //time experiment
 // bool m_bReadyToFire;
 // bool m_bReadyToRender;
 // bool m_bPlayerHasFired;
 // GRAPHIC_IMAGE giMarker;
  bool m_bPaused;
//  int m_visibleObjectCount;
//  int m_visibleTerrainCount;
//  bool m_bMouseTarget; //true if mouse cursor is target

  //isotest functions
  //float m_posX;//position within world...used for scrolling through world
  //float m_posY;
  
  int m_keyMode; //0 = NEWS, 1 = diag

  
  
  int m_currentGroupID;

  //file toolbar interface at top of program
  bool m_bChanged;
  std::string m_filenames[23];
  int m_fileLevel; //1..22 levels
  int m_minRatingLevel;
  //interface bottom
  int m_iconTileSelectIndex;//position of left most icon displayed in a frame
  int m_iconSelectPosition; //controls scrolling of icon selection
  size_t m_iconTotalTiles;     //size of tiles
  float m_scale;
  
  //manages game level landscape tiles
  CLevel m_tileLevel; //stores all things related to the level tiles

  //manages all things in game level
  CObjectManager m_objectManager;

  int m_addTarget; // 0 = tiles, 1 = things, 2 = sounds
};

#endif

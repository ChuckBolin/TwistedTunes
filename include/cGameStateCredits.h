#ifndef GAME_STATE_CREDITS_H
#define GAME_STATE_CREDITS_H
#include <iostream>
using std::cout;
using std::endl;

#include "CGameStateObject.h"
#include "CGraphics.h"
#include "CTimer.h"
#include <sstream>
#include <string>
#include "keystatus.h"
#include "CGameData.h"
#include "CFileReader.h"
#include "CLog.h"
#include "CConfigData.h"

struct SCROLL_MSG{
  std::string msg;
  int red;
  int green;
  int blue;
  int type; //0=text, 1=sprite
  int spriteID;
  float x;
  float y;
  int font;
  GRAPHIC_IMAGE gi;
};

class CGameStateCredits :  public CGameStateObject{
public:
  CGameStateCredits(void);
  ~CGameStateCredits(void);
  void Initialize();
  void Activate(CGameData &data, CConfigData &cfg, CGraphics &con);
  CGameStateObject* Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con);
  int Render(CGraphics &con,  CGameData &data, CConfigData &cfg);
  void ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con);
  void Resume();
  void Pause();
  void Save();
  void AddTransitionEvent(int event, CGameStateObject* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  CTimer m_timer;
  int m_event;
 
  //loaded from file
  std::vector<SCROLL_MSG> m_msg;
  int m_scrollValue;
  int m_scrollSpeed;
  double m_scrollTime;
  int m_backRed;
  int m_backGreen;
  int m_backBlue;
  int m_timerValue;
  
  CMouse m_mouse;

  //used to flip through images
  CTimer m_secondTimer;
  int m_secondCount;

  //loaded from file
 
  //used for fading
  CTimer m_fadeTimer;  
  int m_red;
  int m_green;
  int m_blue;

  //Message helper functions
  SCROLL_MSG SetMessage(float x, int font, int red, int green, int blue, std::string s);
  SCROLL_MSG SetGraphic(float x, int id);
  SCROLL_MSG ClearMessage();
};

#endif

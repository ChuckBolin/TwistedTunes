#ifndef GAME_STATE_HELP_H
#define GAME_STATE_HELP_H
#include <iostream>

#include "CGameStateObject.h"
#include "CGraphics.h"
#include "CTimer.h"
#include <sstream>
#include <string>
#include "keystatus.h"
#include "CGameData.h"
#include "CLog.h"
#include "CConfigData.h"

class CGameStateHelp :  public CGameStateObject{
public:
  CGameStateHelp(void);
  ~CGameStateHelp(void);
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
  CMouse m_mouse;

  int m_screenNumber;
  int m_arrowX;
  int m_arrowY;
};

#endif

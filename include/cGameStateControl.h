#ifndef GAME_STATE_CONTROL_H
#define GAME_STATE_CONTROL_H

#include <iomanip>
#include <sstream>
#include <string>
#include "CGameStateObject.h"
#include "CGraphics.h"
#include "CTimer.h"
#include "keystatus.h"
#include "CGameData.h"
#include "CLog.h"

class CGraphics;

class CGameStateControl :  public CGameStateObject{
public:
  CGameStateControl(void);
  ~CGameStateControl(void);
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

};

#endif

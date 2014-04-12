#ifndef GAME_STATE_QUIT_H
#define GAME_STATE_QUIT_H

#include "CGameStateObject.h"
#include "CGraphics.h"
#include "CTimer.h"
#include <sstream>
#include <string>
#include "keystatus.h"
#include "CGameData.h"
#include "CConfigData.h"

class CGameStateQuit :  public CGameStateObject{
public:
  CGameStateQuit(void);
  ~CGameStateQuit(void);
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
  int m_event;
};

#endif

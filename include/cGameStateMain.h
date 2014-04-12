#ifndef GAME_STATE_MAIN_H
#define GAME_STATE_MAIN_H

#include "CGameStateObject.h"
#include "CTimer.h"
#include "CGraphics.h"
#include <iostream>
#include <mmsystem.h>
#include <windows.h>
#include "CGameData.h"
#include "keystatus.h"
#include "CConfigData.h"

class CGameStateMain :  public CGameStateObject{
public:
  CGameStateMain(void);
  ~CGameStateMain(void);
  void Initialize();
  void Activate(CGameData &data, CConfigData &cfg, CGraphics &con);
  CGameStateObject* Update(double timeDifference, CGameData &data, CConfigData &cfg, CGraphics &con);
  int Render(CGraphics &con, CGameData &data, CConfigData &cfg);
  void ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con);
  void Resume();
  void Pause();
  void Save();
  void AddTransitionEvent(int event, CGameStateObject* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;
  CTimer m_timer;
  int m_selection;

  //used for fading
  int m_red;
  int m_green;
  int m_blue;
  
  //map selection variables
  bool m_bMoveLeft;
  bool m_bMoveRight;
  bool m_bMoveStop;
  int m_movePos;

  //prevents ENTER key bleed through to other states
  bool m_bActivated;
  CTimer m_enableTimer;
  CMouse m_mouse;
  float m_spikeAngle;
  float m_targetAngle;
  bool m_pauseSpike;

};

#endif
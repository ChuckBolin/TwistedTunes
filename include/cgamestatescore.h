//CGameStateScore.h
#ifndef GAME_STATE_SCORE_H
#define GAME_STATE_SCORE_H

#include "CGameStateObject.h"
#include "CTimer.h"
#include "CGraphics.h"
#include <iostream>
#include <windows.h>
#include "CGameData.h"
#include "keystatus.h"
#include "CConfigData.h"

struct HIGH_SCORE{
  std::string name;
  int score;
};

class CGameStateScore :  public CGameStateObject{
public:
  CGameStateScore(void);
  ~CGameStateScore(void);
  void Initialize();
  void Activate();
  CGameStateObject* Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con);
  int Render(CGraphics &con,  CGameData &data, CConfigData &cfg);
  void Deactivate();
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
  
  //prevents ENTER key bleed through to other states
  bool m_bActivated;
  CTimer m_enableTimer;
  CMouse m_mouse;

  int m_width;
  int m_height;
  std::vector<HIGH_SCORE> m_HighScore;
  std::string convertInteger(int num);

};

#endif

//CGameStateObject.h
//base class...all other game states are derived from here
#ifndef GAME_STATE_OBJECT_H
#define GAME_STATE_OBJECT_H

#include <windows.h>
#include <vector>
#include "CGraphics.h"
#include "CGameData.h"
#include "CConfigData.h"

//constants for game state transition events
const int EVENT_GO_NO_WHERE = 0;
const int EVENT_GO_INTRO = 1;
const int EVENT_GO_MAIN = 2;
const int EVENT_GO_CREDITS = 3;
const int EVENT_GO_CONTROL = 4;
const int EVENT_GO_HELP = 5;
const int EVENT_GO_QUIT = 6;
const int EVENT_GO_HIGH_SCORE = 7;
const int EVENT_GO_PLAY1 = 8;
const int EVENT_GO_PLAY2 = 9;
const int EVENT_GO_PLAY3 = 10;
const int EVENT_GO_PLAY4 = 11;

class CGameStateObject;//forward initialization

struct TRANSITION_EVENT{
  int event;
  CGameStateObject* p_gso;
};

class CGameStateObject{
public:
  CGameStateObject();
  virtual ~CGameStateObject();
  virtual void Initialize();
  virtual void Activate(CGameData &data, CConfigData &cfg, CGraphics &con);
  virtual CGameStateObject* Update(double timeDifference, CGameData &data, CConfigData &cfg, CGraphics &con);
  virtual int IsLost(CGraphics &con, CConfigData &cfg);
  virtual int Render(CGraphics &con, CGameData &data, CConfigData &cfg);
  virtual void ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  virtual void Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con);
  virtual void Resume();
  virtual void Pause();
  virtual void Save();
  virtual void AddTransitionEvent(int event, CGameStateObject* p_Next);

protected:

private:
  
  

};

#endif //GAME_STATE_OBJECT_H

/********************************************************************
  Class: CUserInterface
  Purpose: Manages processing of mouse position and mouse clicks
           involving user interface panels. Returns UI event codes.
  Date:  November 23, 2009
********************************************************************/
#ifndef C_USER_INTERFACE_H
#define C_USER_INTERFACE_H

#include "CGraphics.h"
#include "CGameData.h"
#include "CLog.h"

//UI constants
const int UI_NONE = 0;
const int UI_SEND_WAVE = 1;
const int UI_QUIT = 2;
const int UI_PAUSE_PLAY = 3;
const int UI_TIMES1 = 4;
const int UI_TIMES2 = 5;
const int UI_HELP = 6;
const int UI_DEF1 = 7;
const int UI_DEF2 = 8;
const int UI_DEF3 = 9;
const int UI_DEF4 = 10;
const int UI_DEF5 = 11;
const int UI_DEF6 = 12;
const int UI_DEF7 = 13;
const int UI_DEF8 = 14;

//defines rect type
struct UI_RECT{
  int x1;
  int y1;
  int x2;
  int y2;
};

class CUserInterface{
public:
  CUserInterface();
  ~CUserInterface();
  void Update(double timeDifference, int mx, int my, CGameData &data);
  void Render(CGraphics &con, CGameData &data);         //render sprites
  void RenderOutlines(CGraphics &con, CGameData &data); //render outlines..rect
  int GetUIHoverState();
  int GetUIState(){return m_UIstate;}
  UI_RECT GetUIInfoOff(int index);
  UI_RECT GetUIInfoDef(int index);
  void Reset();
  void ResetState(){m_UIstate = 0;}  //call this after responding to GetUIState() in play state
  
private:
  int m_UIstate;

  //mouse button state
  bool m_bLeftButtonDown;
  bool m_bRightButtonDown;
  bool m_bLastLeftButtonDown;
  bool m_bLastRightButtonDown;

  //mouse position
  int m_mouseX;
  int m_mouseY;
  int m_lastMouseX;
  int m_lastMouseY;
  
  //stores 1..4 to indicate status of hovering over panels DOCS
  int m_hoverState;

  //display
  UI_RECT m_waveCount;
  UI_RECT m_score;
  UI_RECT m_credits;

  //buttons
  UI_RECT m_sendWaveButton;  
  UI_RECT m_quit;
  UI_RECT m_pausePlay;
  UI_RECT m_times1;
  UI_RECT m_times2;  
  UI_RECT m_def1;
  UI_RECT m_def2;
  UI_RECT m_def3;
  UI_RECT m_def4;
  UI_RECT m_def5;
  UI_RECT m_def6;
  UI_RECT m_def7;
  UI_RECT m_def8;
  UI_RECT m_help;
  UI_RECT m_sound;
  UI_RECT m_music;

  //utility
  UI_RECT SetUIRect(int x1, int y1, int x2, int y2);  //adds 2 corners
  UI_RECT SetUIRectWH(int x1, int y1, int w, int h);  //adds w & h to corner
  bool IsInsideRect(int x1, int y1, int x2, int y2);
  bool IsInsideRect(UI_RECT ui);
  UI_RECT GetAbsRect(UI_RECT rect1, UI_RECT rect2);

};

#endif
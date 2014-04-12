//Keystatus.h
#ifndef KEYSTATUS_H
#define KEYSTATUS_H

#include <windows.h>

class CMouse{
public:
  CMouse(HWND hWnd);
  CMouse();
  void SetHandle(HWND hWnd);
  float MouseX();
  float MouseY();
  bool LeftButtonDown(){return m_leftDown;}
  bool RightButtonDown(){return m_rightDown;}
  void SetLeftButtonDown(bool value){m_leftDown = value;}
  void SetRightButtonDown(bool value){m_rightDown = value;}

private:
  HWND m_hWnd;
  bool m_leftDown;
  bool m_rightDown;
};


//function prototypes
bool keyDown(int key);

#endif

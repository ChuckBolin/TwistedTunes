#include "..\include\CUserInterface.h"

CUserInterface::CUserInterface(){
  CLog *pLog = CLog::Instance();
  Reset();

  //displays
  m_waveCount = SetUIRect(86, 687, 140, 710);
  m_score = SetUIRect(134, 733, 239, 755);
  m_credits = SetUIRect(856, 731, 952, 755);

  //buttons
  m_sendWaveButton = SetUIRect(157, 686, 284, 712);  
  m_quit = SetUIRect(535, 644, 600, 670);
  m_pausePlay = SetUIRect(385, 644, 448, 671);
  m_times1 = SetUIRect(461, 644, 487, 671);
  m_times2 = SetUIRect(498, 644, 525, 678);  
  m_def1 = SetUIRect(313, 675, 361, 727);
  m_def2 = SetUIRect(361, 675, 410, 727);
  m_def3 = SetUIRect(410, 675, 460, 727);
  m_def4 = SetUIRect(460, 675, 510, 727);
  m_def5 = SetUIRect(510, 675, 559, 727);
  m_def6 = SetUIRect(559, 675, 609, 727);
  m_def7 = SetUIRect(609, 675, 659, 727);
  m_def8 = SetUIRect(659, 675, 707, 727);
  m_help = SetUIRect(612, 643, 639, 691);
  m_sound = SetUIRect(960, 728, 990, 758);
  m_music = SetUIRect(960, 728, 990, 758);

}
 
CUserInterface::~CUserInterface(){

}

void CUserInterface::Reset(){
  //mouse button state
  m_bLeftButtonDown = false;
  m_bRightButtonDown = false;
//  m_bMoveFirstTime = false;

  //mouse position
  m_mouseX = -1;
  m_mouseY = -1;

}

//private utility to convert 4 params into struct type
UI_RECT CUserInterface::SetUIRect(int x1, int y1, int x2, int y2){
  UI_RECT ui;
 
  ui.x1 = x1;
  ui.y1 = y1;
  ui.x2 = x2;
  ui.y2 = y2;

  return ui;
}

UI_RECT CUserInterface::SetUIRectWH(int x1, int y1, int w, int h){
  UI_RECT ui;
 
  ui.x1 = x1;
  ui.y1 = y1;
  ui.x2 = ui.x1 + w;
  ui.y2 = ui.y1 + h;

  return ui;
}

bool CUserInterface::IsInsideRect(int x1, int y1, int x2, int y2){
  if(m_mouseX > x1 && m_mouseX < x2 && m_mouseY > y1 && m_mouseY < y2)
    return true;
  else
    return false;
}

bool CUserInterface::IsInsideRect(UI_RECT ui){
  if(m_mouseX > ui.x1 && m_mouseX < ui.x2 && m_mouseY > ui.y1 && m_mouseY < ui.y2)
    return true;
  else
    return false;
}

//rect1 top-left corner is in mouse coordinates
UI_RECT CUserInterface::GetAbsRect(UI_RECT rect1, UI_RECT rect2){
  UI_RECT sum;
  sum.x1 = rect1.x1 + rect2.x1;
  sum.y1 = rect1.y1 + rect2.y1;
  sum.x2 = rect1.x1 + rect2.x2;
  sum.y2 = rect1.y1 + rect2.y2;

  return sum;
}

//**********************************************************************************  U P D A T E 
//************************************************************************************************
void CUserInterface::Update(double timeDifference, int mx, int my, CGameData &data){
  CLog *pLog = CLog::Instance();

  
  static double pauseTime; 
  pauseTime += timeDifference;
  static double keyTime;
  keyTime += timeDifference;
  static double scaleTime;
  scaleTime += timeDifference;

  //set mouse variables
  //********************************
  m_mouseX = mx;
  m_mouseY = my;
  
  if(data.m_bLeftMouseDown == true)
    m_bLeftButtonDown = true;
  else
    m_bLeftButtonDown = false;

  if(data.m_bRightMouseDown == true)
    m_bRightButtonDown = true;
  else
    m_bRightButtonDown = false;

  if(m_bLeftButtonDown == true && IsInsideRect(m_pausePlay) == true && pauseTime > 0.4){
    pauseTime = 0.0;
    data.m_bPaused = !data.m_bPaused;
    m_UIstate = UI_PAUSE_PLAY;
  }
 
  if(m_bLeftButtonDown == true && keyTime > 0.4){
    keyTime = 0;
    
    if(IsInsideRect(m_sendWaveButton) == true)
      m_UIstate = UI_SEND_WAVE;
    else if(IsInsideRect(m_quit) == true)
      m_UIstate = UI_QUIT;
    else if(IsInsideRect(m_times1) == true)
      m_UIstate = UI_TIMES1;
    else if(IsInsideRect(m_times2) == true)
      m_UIstate = UI_TIMES2;
    else if(IsInsideRect(m_help) == true)
      m_UIstate = UI_HELP;
    else if(IsInsideRect(m_def1) == true)
      m_UIstate = UI_DEF1;
    else if(IsInsideRect(m_def2) == true)
      m_UIstate = UI_DEF2;
    else if(IsInsideRect(m_def3) == true)
      m_UIstate = UI_DEF3;
    else if(IsInsideRect(m_def4) == true)
      m_UIstate = UI_DEF4;
    else if(IsInsideRect(m_def5) == true)
      m_UIstate = UI_DEF5;
    else if(IsInsideRect(m_def6) == true)
      m_UIstate = UI_NONE;
    else if(IsInsideRect(m_def7) == true)
      m_UIstate = UI_NONE;
    else if(IsInsideRect(m_def8) == true)
      m_UIstate = UI_NONE;
    else
      m_UIstate = UI_NONE; 

  }

  //save last mouse position and data
  m_lastMouseX = m_mouseX;
  m_lastMouseY = m_mouseY;
  m_bLastLeftButtonDown = m_bLeftButtonDown;
  m_bLastRightButtonDown = m_bRightButtonDown;

  if(m_UIstate == UI_HELP){
//    data.m_bDisplayHelp = !data.m_bDisplayHelp;
//    data.m_bPaused = data.m_bDisplayHelp;
  }

  //data.m_score++;
  //if(data.m_score > 999999)
  //  data.m_score = 0;

  //data.m_credits++;
  //if(data.m_credits > 999999)
  //  data.m_credits = 0;

}

//************************************************************************** R E N D E R
//**************************************************************************************
void CUserInterface::Render(CGraphics &con,  CGameData &data){
  GRAPHIC_IMAGE gi;
  CLog *pLog = CLog::Instance();

  //renders main
  gi = con.GetSprite(1500);
  con.RenderSprite(gi, 0, 623, 255, 255, 255);

  //renders grayed out icons on UI if there is not enough credits for them
  std::string parameter;
  float cost = 0;
  
  for(int i = 0; i < 6; ++i){
    //parameter = data.m_catalog.GetTerm(i, 11);
    //cost = atof(parameter.c_str());
   // if(i == 6){
   //   pLog->Log("cost",data.m_credits, cost);
   // }

  }

   //   gi = con.GetSprite(1605);
   //   con.RenderSprite(gi, 562, 676, 255, 255, 255);

  //pause graphic
  if(data.m_bPaused == true){
    gi = con.GetSprite(1502);
    con.RenderSprite(gi, 385, 643, 255, 255, 255);
  }

  //render x1 x2 time scale
  /*
  if(data.m_timeScale == 1){
    gi = con.GetSprite(1506);
    con.RenderSprite(gi, 457, 641, 255, 255, 255);
    gi = con.GetSprite(1507);
    con.RenderSprite(gi, 499, 645, 255, 255, 255);
  }
  */

  //render yellow outline around selected defensive unit
  if(m_UIstate >= 7 && m_UIstate <= 14){
 //   gi = con.GetSprite(1505);
 //   con.RenderSprite(gi, 313 + (50 * (m_UIstate - 7)), 675, 255, 255, 255);
  }

  //renders score data.m_score
  //*******************************************
  /*
  int posX = 135;//m_panelS.x1 + 17;
  int posY = 733;//m_panelS.y1 + 10;
  std::string sRef = "0123456789";
  std::ostringstream oss;
  oss << data.m_score;
  std::string sNum = oss.str();
  std::string sPrefix;
  if(sNum.length() < 6)
    sPrefix.append(6 - sNum.length(), '0');
  std::string sScore = sPrefix + sNum;

  for(int i = 0; i < sScore.size();++i){
    for(int j = 0; j < sRef.size(); ++j){
      if(sScore.at(i) == sRef.at(j)){
        gi = con.GetSprite(1200 + j);
        con.RenderSprite(gi, posX, posY, 255, 255, 255);
        posX += gi.width;
        break;
      }
    }
  }
*/
  //renders sound and music icons
  //if(data.m_playMusic == true)
  //  gi = con.GetSprite(1511);
  //else
  //  gi = con.GetSprite(1510);
  //con.RenderSprite(gi, 960, 728, 255, 255, 255);
}

void CUserInterface::RenderOutlines(CGraphics &con, CGameData &data){
/*
  for(int i = 0; i <  8; ++i){
    if(m_bDefIcons[i] == true && m_bDMaximized == true)
      con.DrawRect(m_panelD.x1 + m_DefIcons[i].x1, m_panelD.y1 + m_DefIcons[i].y1,
                   m_panelD.x1 + m_DefIcons[i].x2, m_panelD.y1 + m_DefIcons[i].y2, 255, 255, 0);


    if(m_bOffIcons[i] == true && m_bOMaximized == true)
      con.DrawRect(m_panelO.x1 + m_OffIcons[i].x1, m_panelO.y1 + m_OffIcons[i].y1,
                   m_panelO.x1 + m_OffIcons[i].x2, m_panelO.y1 + m_OffIcons[i].y2, 255, 255, 0);


  }
*/
}

int CUserInterface::GetUIHoverState(){


  return -1;
}

UI_RECT CUserInterface::GetUIInfoOff(int index){
  UI_RECT ui;
  ui.x1 = 0;
  ui.y1 = 0;
  ui.x2 = 0;
  ui.y2 = 0;
 
  return ui;

}

UI_RECT CUserInterface::GetUIInfoDef(int index){
  UI_RECT ui;
  ui.x1 = 0;
  ui.y1 = 0;
  ui.x2 = 0;
  ui.y2 = 0; 
  return ui;

}

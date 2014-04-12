/*
  CGameStateMain.cpp - Controls the user interface and display for the main menu. 

*/
#include "..\include\CGameStateMain.h"
#include "..\include\keystatus.h"

CGameStateMain::CGameStateMain(void)
{
}

CGameStateMain::~CGameStateMain(void)
{
	m_TE.clear();
}

void CGameStateMain::Initialize(){
  m_selection = 0; 
}

void CGameStateMain::Activate(CGameData &data, CConfigData &cfg, CGraphics &con){
 m_bMoveLeft = false;
}

CGameStateObject* CGameStateMain::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;

  //CAudioManager *pAudio = CAudioManager::Instance();
  m_mouse.SetHandle(data.m_hWnd);
  con.m_textureRed = 55;//data.m_textureColorRed;
  con.m_textureGreen = 55;//data.m_textureColorGreen;
  con.m_textureBlue = 55;//data.m_textureColorBlue;

  //process keys
  static double timeDiff = 0;
  timeDiff += timeDifference;
  
  if(::GetActiveWindow() == data.m_hWnd && timeDiff > 0.5){
    if(data.m_bLeftMouseDown == true)
      m_event = EVENT_GO_PLAY1;


  } 

  //mouse clicking
  //*********************************************************************************************
  //if(::GetActiveWindow() == data.m_hWnd && timeDiff > 0.4 ){//data.m_bLeftMouseDown == true && 
  //  timeDiff = 0.0;

  //  if(keyDown('Q') == true)
  //    m_event = EVENT_GO_CREDITS;
  //  else if(keyDown('P') == true)
  //    m_event = EVENT_GO_PLAY1;

  //  //left arrow clicked
  //  if(m_mouse.MouseX() > 190 && m_mouse.MouseX() < 223 && m_mouse.MouseY() > 349 && m_mouse.MouseY() < 449){
  //  }

  //}

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
        return m_TE[i].p_gso;
    }
  }

  return 0;
}

int CGameStateMain::Render(CGraphics &con,  CGameData &data, CConfigData &cfg){
  CLog *pLog = CLog::Instance();
  static bool bLostDevice = false;

  ::ShowCursor(true);

  //test to see if graphic device is lost, if so, deal with it
  //**********************************************************
  HRESULT hr;
  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    
    //device is lost...cannot be reset...exit Render() function
    if(hr == D3DERR_DEVICELOST){
      if(bLostDevice == false){
        bLostDevice = true;
        con.DeleteLost();    //delete one time per failure
      }
      return 0;
    }

    //device was lost...but now can be RESET
    else if( hr == D3DERR_DEVICENOTRESET ){
      hr = con.m_d3ddev->Reset(&con.GetDPP());      
      if(SUCCEEDED(hr)){
        con.LoadAssetFile(cfg.GamePlayAssetFile);
        con.LoadAssetFile(cfg.FrameworkAssetFile);     
        con.ReloadLost();
        bLostDevice = false;
      }
      return 0;
    }
  }

  // clear the window to a deep blue
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene
  con.BeginRender();

  GRAPHIC_IMAGE gi;
  gi = con.GetSprite(2001);
  con.RenderSprite(gi, 0, 0, 255, 255, 255);  
  
  //instructions
//if(cfg.DisplayDebugInfo == true){
//  con.Print("FPS", data.m_FPS, F_A12, 10, 10, 0, 0, 0, 255);
//  con.Print("Mouse X", m_mouse.MouseX(), F_A12, 10, 30, 255, 255, 255, 255);
//  con.Print("Mouse Y", m_mouse.MouseY(), F_A12, 10, 50, 255, 255, 255, 255);
//}
  con.EndRender();

  con.Print("INSTRUCTIONS",F_V20, 420, 30, 255, 255, 255, 255);
  
  con.Print("There are five levels. Each level contains 4 parts to a song. There are several songs per level.",F_V20, 100, 62, 255, 255, 0, 255);
  con.Print("Your goal is to move the 4 related parts of a song to the top assembly area. The program will",F_V20, 100, 90, 255, 255, 0, 255);
  con.Print("assemble the parts automatically. Points are scored for each song correctly assembled.",F_V20, 100, 118, 255, 255, 0, 255);

  con.Print(" Level 1 -  4 songs,  40 pts/song  Bonus if completed within  80 seconds ( 40 pts per second under)", F_V20, 100, 150, 0, 255, 0, 255);
  con.Print(" Level 2 -  6 songs,  60 pts/song  Bonus if completed within 120 seconds ( 60 pts per second under)", F_V20, 100, 180, 0, 255, 0, 255);
  con.Print(" Level 3 -  8 songs,  80 pts/song  Bonus if completed within 160 seconds ( 80 pts per second under)", F_V20, 100, 210, 0, 255, 0, 255);
  con.Print(" Level 4 - 10 songs, 100 pts/song  Bonus if completed within 200 seconds (100 pts per second under)", F_V20, 100, 240, 0, 255, 0, 255);
  con.Print(" Level 5 - 20 songs, 200 pts/song  Bonus if completed within 400 seconds (200 pts per second under)", F_V20, 100, 270, 0, 255, 0, 255);
  //con.Print("                                      Level 1 -  40 points per song",F_V20, 100, 150, 255, 255, 0, 255);
  //con.Print("                                      Level 2 -  80 points per song",F_V20, 100, 180, 255, 255, 0, 255);
  //con.Print("                                      Level 3 -  120 points per song",F_V20, 100, 210, 255, 255, 0, 255);
  //con.Print("                                      Level 4 -  160 points per song",F_V20, 100, 240, 255, 255, 0, 255);
  //con.Print("                                      Level 5 -  200 points per song",F_V20, 100, 270, 255, 255, 0, 255);

  con.Print("Left click and hold to drag song parts to assembly area. Right click a song part in the assembly",F_V20, 100, 330, 255, 255, 0, 255);
  con.Print("area to remove it. Right click sparingingly. You will lose points for each tile you remove.",F_V20, 100, 360, 255, 255, 0, 255);
  con.Print("In addition you will lose points for each incorrect guess.",F_V20, 100, 390, 255, 255, 0, 255);
  con.Print(" Level 1 - Lose  40 pts per removed tile", F_V20, 100, 420, 255, 0, 0, 255);
  con.Print(" Level 2 - Lose  60 pts per removed tile", F_V20, 100, 450, 255, 0, 0, 255);
  con.Print(" Level 3 - Lose  80 pts per removed tile", F_V20, 100, 480, 255, 0, 0, 255);
  con.Print(" Level 4 - Lose 100 pts per removed tile", F_V20, 100, 510, 255, 0, 0, 255);
  con.Print(" Level 5 - Lose 200 pts per removed tile", F_V20, 100, 540, 255, 0, 0, 255);

  con.Print("Be careful moving tiles. They may be stacked upon one another.",F_V20, 100, 570, 255, 255, 255, 255);
  con.Print("Press ESC at any time to quit. The game is not saved.",F_V20, 100, 600, 255, 255, 255, 255);
  con.Print("There are 48 songs in total. Good Luck!",F_V20, 100, 630, 255, 255, 255, 255);
  
  
  con.Print("Click to continue",F_V20, 420, 730, 255, 255, 255, 255);
  //draw mouse cursor
  //draw mouse
  //con.DrawCircle((float)m_mouse.MouseX(), (float)m_mouse.MouseY(), 255, 255, 255, 5);  
  //con.DrawRect(m_mouse.MouseX() - 16, m_mouse.MouseY() - 16, m_mouse.MouseX() + 16, m_mouse.MouseY() + 16, 255, 255, 255);

  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

  return 0;
}

void CGameStateMain::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void CGameStateMain::Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con){

}
void CGameStateMain::Resume(){
}
void CGameStateMain::Pause(){
}
void CGameStateMain::Save(){
}

void CGameStateMain::AddTransitionEvent(int event, CGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
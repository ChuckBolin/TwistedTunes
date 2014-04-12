#include "..\include\CGameStateHelp.h"

CGameStateHelp::CGameStateHelp(){
}

CGameStateHelp::~CGameStateHelp()
{
	m_TE.clear();
}

void CGameStateHelp::Initialize(){

}

void CGameStateHelp::Activate(CGameData &data, CConfigData &cfg, CGraphics &con){
  m_screenNumber = 0;
}

CGameStateObject* CGameStateHelp::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;

//  CAudioManager *pAudio = CAudioManager::Instance();

  //play background music
 // if(cfg.PlayMusic == true && pAudio->IsPlaying(4) == false)
  //  pAudio->PlayAudioClip(4, 1);

  //first time
  static bool bFirstTime = true;
  if(bFirstTime == true){
    bFirstTime = false;
    m_screenNumber = 0;
    m_arrowX = 150;
    m_arrowY = 150;
  }


  //CAudioManager *pAudio = CAudioManager::Instance();
  m_mouse.SetHandle(data.m_hWnd);


  static double screenTime = 0;
  screenTime += timeDifference;
  
  if(::GetActiveWindow() == data.m_hWnd && screenTime > 0.4 && data.m_bLeftMouseDown == true){
    
    //previous button
    if(m_mouse.MouseX() > 212 && m_mouse.MouseX() < 312 && 
      m_mouse.MouseY() > 650 && m_mouse.MouseY() < 700){
        m_screenNumber--;
        if(m_screenNumber < 0)
          m_screenNumber = 0;
    }
    //next button
    else if(m_mouse.MouseX() > 712 && m_mouse.MouseX() < 812 && 
      m_mouse.MouseY() > 650 && m_mouse.MouseY() < 700){
        m_screenNumber++;
        if(m_screenNumber > 9)
          m_screenNumber = 9;
    }
    
    screenTime = 0;
  }
  
  //animate arrow
  static double arrowTime = 0;
  arrowTime += timeDifference;
  if(arrowTime > 0.05 && m_screenNumber == 1){
    m_arrowX += 10;
    m_arrowY += 5;

    if(m_arrowX > 750){
      m_arrowX = 150;
      m_arrowY = 150;
    }    
    arrowTime = 0;
  }

  if(::GetActiveWindow() == data.m_hWnd && data.m_bLeftMouseDown == true){
    //main menu
   // if(m_mouse.MouseX() > 812 && m_mouse.MouseX() < 1012 && 
   //   m_mouse.MouseY() > 50 && m_mouse.MouseY() < 100){
      m_event = EVENT_GO_MAIN;    
   // }
  }


  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

int CGameStateHelp::Render(CGraphics &con,  CGameData &data, CConfigData &cfg){
  CLog *pLog = CLog::Instance();
  static bool bLostDevice = false;

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
  con.m_d3ddev->BeginScene();   
  con.BeginRender();

  GRAPHIC_IMAGE gi;

    //gi = con.GetSprite(135);//title
    //con.RenderSprite(gi, 270, 100, 255, 255, 255);
  //mouse cursor
  //gi = con.GetSprite(108);  
  //con.RenderSprite(gi, m_mouse.MouseX() - gi.width/2,m_mouse.MouseY() - gi.height/2, 255, 255, 255);
  
  con.EndRender();
  con.Print("Help Screen", m_screenNumber, F_V20B, cfg.ScreenWidth/2 - 120, 50, 255, 255, 255, 255);
  

  if(cfg.DisplayDebugInfo == true){
    con.Print("Version", cfg.ProgramVersion, F_V16, 40, 730,  255, 255, 255, 255);
    con.Print("FPS", data.m_FPS , F_V16, 930, 730,  255, 255, 255, 255);
  }  

  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
  return 0;
}

void CGameStateHelp::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void CGameStateHelp::Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con){
//  CAudioManager *pAudio = CAudioManager::Instance();
 // pAudio->KillAll();

}
void CGameStateHelp::Resume(){
}
void CGameStateHelp::Pause(){
}
void CGameStateHelp::Save(){
}

void CGameStateHelp::AddTransitionEvent(int event, CGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

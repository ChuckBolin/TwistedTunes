//CGameStateScore.cpp
#include "..\include\CGameStateScore.h"

CGameStateScore::CGameStateScore(void)
{
}

CGameStateScore::~CGameStateScore(void)
{
}

void CGameStateScore::Initialize(){

}

void CGameStateScore::Activate(){
}


CGameStateObject* CGameStateScore::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;

  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_ESCAPE)){
    m_event = EVENT_GO_MAIN;
  }

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event)
      return m_TE[i].p_gso;
  }
  return 0;
}


std::string CGameStateScore::convertInteger(int num){
  std::ostringstream oss;
  std::string s;
  oss.str("");
  oss << num;
  s = oss.str();
  return s;
}

void CGameStateScore::Render(CGraphics &con,  CGameData &data, CConfigData &cfg){ 

  static bLostDevice = false;
 
 
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
      return;
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
      return;
    }
  }

  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 100, 100), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene
 
  GRAPHIC_IMAGE gi;
  gi = con.GetSprite(0);
  con.RenderGraphicModulate(gi, 20, 20, 255, 255, 255); //gi.width * gi.scale/2, 75, 255, 255, 255);
 
  //player prompt
  con.Print("High Score", F_V20B, cfg.ScreenWidth/2 - 60, 100, 255, 255, 255, 255);
  con.Print("Press ESCAPE to return to the Main Menu!", F_V20B, cfg.ScreenWidth/2 - 200, cfg.ScreenHeight - 100, 255, 255, 255, 255);

  if(cfg.DisplayDebugInfo == true){
    con.Print("Version", cfg.ProgramVersion, F_V16, 40, 730,  255, 255, 255, 255);
    con.Print("FPS", data.m_FPS , F_V16, 930, 730,  255, 255, 255, 255);
  }  

  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

}

void CGameStateScore::Deactivate(){
}

void CGameStateScore::Resume(){
}

void CGameStateScore::Pause(){
}

void CGameStateScore::Save(){
}

void CGameStateScore::AddTransitionEvent(int event, CGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
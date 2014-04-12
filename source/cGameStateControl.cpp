#include "..\include\CGameStateControl.h"
 
CGameStateControl::CGameStateControl(){
}

CGameStateControl::~CGameStateControl()
{
	m_TE.clear();
}

void CGameStateControl::Initialize(){

}

void CGameStateControl::Activate(CGameData &data, CConfigData &cfg, CGraphics &con){

}
//***************************************************************
//                         U P D A T E
//***************************************************************
CGameStateObject* CGameStateControl::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  CLog *pLog = CLog::Instance();

  static bool bStop = false;
  
  //perform this code only once
  //****************************
  static bool bFirstTime = true;
  if(bFirstTime == true){
    bFirstTime = false;

    //data.LoadGameLevel("assets\\data\\level1.dat"); //missing sprite data
    //data.AddGraphicDataToLevelData(con);            //update graphic data
  }

  
  m_mouse.SetHandle(data.m_hWnd);//needed for mouse

  if(::GetActiveWindow() == data.m_hWnd && keyDown(VK_ESCAPE))//quit this mode
    m_event = EVENT_GO_MAIN;
   
  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

//***************************************************************
//                         R E N D E R 
//***************************************************************
int CGameStateControl::Render(CGraphics &con,  CGameData &data, CConfigData &cfg){
  //::ShowCursor(true);
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
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(50, 50, 50), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene
 
  GRAPHIC_IMAGE gi;
  gi = con.GetSprite(0);
  con.RenderSprite(gi, 20, 20, 255, 255, 255); 


  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

  return 0;
}

void CGameStateControl::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void CGameStateControl::Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con){
//  CAudioManager *pAudio = CAudioManager::Instance();


  
}
void CGameStateControl::Resume(){
}
void CGameStateControl::Pause(){
}
void CGameStateControl::Save(){
}

void CGameStateControl::AddTransitionEvent(int event, CGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

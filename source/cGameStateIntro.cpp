#include "..\include\CGameStateIntro.h"

CGameStateIntro::CGameStateIntro(void){
}

CGameStateIntro::~CGameStateIntro(void)
{
	m_TE.clear();
}

void CGameStateIntro::Initialize(){
}

void CGameStateIntro::Activate(CGameData &data, CConfigData &cfg, CGraphics &con){
  //CLog *pLog = CLog::Instance();
  //test
  m_scale = 1.0;
  m_posX = 0;
  m_posY = 386;
  m_angle = 0;

  //pyro setup
  //**********************
  //m_effect.Init(con.m_d3ddev);
  //m_effect.LoadFile(cfg.EffectsFileI);//"assets\\data\\effects_i.dat");
  //m_effectNumber = 0;
  //m_effectMax = m_effect.GetNumberEffects();
  //pLog->Log("******* Number of Intro Screen effects loaded", m_effectMax);
}

CGameStateObject* CGameStateIntro::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  m_mouse.SetHandle(data.m_hWnd);//needed for mouse
  ::ShowCursor(true);
  //static double counterTime = 0;
  //counterTime += timeDifference;
  con.m_textureRed = 255;//data.m_textureColorRed;
  con.m_textureGreen = 255;//data.m_textureColorGreen;
  con.m_textureBlue = 255;//data.m_textureColorBlue;

  /*
  CAudioManager *pAudio = CAudioManager::Instance();
  if(cfg.PlayMusic == true && pAudio->IsPlaying(0) == false)
    pAudio->PlayAudioClip(0, 1);
  */


  //m_scale = 1.0;
  //m_posX = 512;
  //m_posY = 386;
  //m_angle = 0;
  static double keyTime = 0;
  keyTime += timeDifference;
  float speed = 25.0f;
  static bool dir = false;
  float rate = 0.1f;

  //if(dir == true)
  //  m_angle += static_cast<float>(rate * timeDifference);
  //else
  //  m_angle -= static_cast<float>(rate * timeDifference);
  //if(m_angle > 0.1f)
  //  dir = false;
  //if(m_angle < -0.1f)
  //  dir = true;  

  // if(keyDown('Q') == true && keyTime > 0.1f){
  //    keyTime = 0.0f;
  //    m_scale += 0.01f;
  // }
  // if(keyDown('W') == true && keyTime > 0.1f){
  //    keyTime = 0.0f;
  //    m_scale -= 0.01f;
  // }



    //if(keyDown(VK_DOWN) == true){// && keyTime > 0.3f){
    //  keyTime = 0.0f;
    //  m_posY -= static_cast<float>(speed * timeDifference);
    //}
    //if(keyDown(VK_UP) == true){// && keyTime > 0.3f){
    //  keyTime = 0.0f;
    //  m_posY += static_cast<float>(speed * timeDifference);
    //}
    //if(keyDown(VK_LEFT) == true){// && keyTime > 0.3f){
    //  keyTime = 0.0f;
    //  m_posX += static_cast<float>(speed * timeDifference);
    //}
    //if(keyDown(VK_RIGHT) == true){// && keyTime > 0.3f){
    //  keyTime = 0.0f;
    //  m_posX -= static_cast<float>(speed * timeDifference);
    //}  
  //left click to continue
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< bypassed
  if(::GetActiveWindow() == data.m_hWnd){
    if(data.m_bLeftMouseDown == true)
      m_event = EVENT_GO_MAIN;
  }     

  //m_effect.Update(m_startTime);
  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

int CGameStateIntro::Render(CGraphics &con, CGameData &data, CConfigData &cfg){
  ::ShowCursor(false);
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
        //m_effect.Delete();
        //pLog->Log("Deleted");
      }
      return 0;
    }

    //device was lost...but now can be RESET
    else if( hr == D3DERR_DEVICENOTRESET ){
      hr = con.m_d3ddev->Reset(&con.GetDPP());      
      if(SUCCEEDED(hr)){
        //m_effect.Init(con.m_d3ddev);
        //m_effect.LoadFile(cfg.EffectsFileI);
        //m_effectNumber = 0;
        //m_effectMax = m_effect.GetNumberEffects();
        //pLog->Log("******* Number of effects reloaded", m_effectMax);
        con.LoadAssetFile(cfg.GamePlayAssetFile);
        con.LoadAssetFile(cfg.FrameworkAssetFile);     
        con.ReloadLost();
        bLostDevice = false;
      }
      return 0;
    }
  }

  // clear the window to a deep blue
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);//   0, 111, 255), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene

  GRAPHIC_IMAGE gi;  
  con.BeginRender();
  //gi = con.GetSprite(1002);
  //gi.scale = 4.5;
  //con.RenderSprite(gi, 0, 0, 255, 255, 255);
  
  //gi = con.GetSprite(100);
  //gi.angle = 1.57f;
  //con.RenderSprite(gi, 392, 248, 0.5, 1, 120, 120);
  gi = con.GetSprite(2000);
  //gi.alpha = 255;
  //gi.angle = 1.57f;
  //gi.angle = m_angle;
  con.RenderSprite(gi, 0, 0, 255, 255,255);// m_posX, m_posY, m_scale, m_scale, 512, 10);

  con.EndRender();

  //m_effect.Render(); 

 // con.DrawRect(0, 0, 599, 449, 255, 0, 0);
 // con.DrawRect(601, 0, 1023, 449, 255, 0, 0);
 // con.DrawRect(0, 451, 1023, 766, 255, 0, 0);
  //con.DrawLine(0, 384, 1024, 384, 0, 0, 255);
  //con.DrawLine(512, 0, 512, 768, 0, 0, 255);

  //mouse cursor
  //con.DrawCircle(m_mouse.MouseX(), m_mouse.MouseY(), 255, 255, 255, 10);

  //*********************************************
  //test of vector
  /*
  std::vector<float> vecNum;
  for(int i = 0; i < 50; ++i)
    vecNum.push_back(3.14159345f);
  con.PrintVector(vecNum, F_V12, 5, 5, 0, 0, 0, 255);
  */
  //*********************************************

  //*********************************************
  //THIS IS MORE EFFICIENT THAN
  //con.PrintTest("FPS", data.m_FPS, F_A12, 10, 10, 0, 0, 0, 255);

  //THIS.....................
  /*
  for(int i = 0; i < 50;++i){
    for(int j = 0; j < 10; ++j){
      con.Print("FPS", data.m_FPS, F_A12, 10 + (j * 100), 10 + (i * 15), i * 5, i * 5, i * 5, 255);
    }  
  }
  */
  //*********************************************

  con.Print("Click to continue",F_V20, 420, 730, 0, 0, 0, 255);

  ////con.Print("
  //if(cfg.DisplayDebugInfo == true){
  //  con.Print("FPS", data.m_FPS, F_A14, 500, 680, 255, 255, 0, 255);
  //  con.Print("Mouse X", m_mouse.MouseX(), F_V12, 510, 330, 255, 255, 255, 255);
  //  con.Print("Mouse Y", m_mouse.MouseY(), F_V12, 510, 350, 255, 255, 255, 255);
  //  

  //}

  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
 
  return 0;
}

void CGameStateIntro::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void CGameStateIntro::Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con){
}
void CGameStateIntro::Resume(){
}
void CGameStateIntro::Pause(){
}
void CGameStateIntro::Save(){
}

void CGameStateIntro::AddTransitionEvent(int event, CGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
#include "..\include\CGameStateCredits.h"

CGameStateCredits::CGameStateCredits(void){
}

CGameStateCredits::~CGameStateCredits(void)
{
	m_TE.clear();
}

SCROLL_MSG CGameStateCredits::SetMessage(float x, int font, int red, int green, int blue, std::string s){
  SCROLL_MSG msg;
  msg.x = x;
  msg.spriteID = -1;
  msg.type = 0; //font
  msg.font = font;
  msg.red = red;
  msg.green = green;
  msg.blue = blue;
  msg.msg = s;
  return msg;
}


SCROLL_MSG CGameStateCredits::SetGraphic(float x, int id){
  SCROLL_MSG msg;
  msg.type = 1;//graphic
  msg.x = x;
  msg.spriteID = id;
  msg.font = F_V20;
  msg.red = 0;
  msg.green = 0;
  msg.blue = 0;
  msg.msg = "";
  return msg;
}

SCROLL_MSG CGameStateCredits::ClearMessage(){
  SCROLL_MSG msg;
  msg.type = 0; //font
  msg.font = F_V20;
  msg.red = 0;
  msg.green = 0;
  msg.blue = 0;
  msg.spriteID = -1;
  msg.msg = "";
  msg.x = -1;
  return msg;
}


void CGameStateCredits::Initialize(){
  CLog *pLog = CLog::Instance();
  SCROLL_MSG msg;
  CFileReader cfr;
  cfr.SupressSpaceRemoval(true);
  cfr.LoadFile("assets\\data\\credits.dat");
  int numberLines = cfr.GetNumberOfLines();
  std::string lineType = "";
  std::string p1, p2, p3, p4, p5, p6, p7;
  
  //default parameters may be overwritten by file
  m_scrollSpeed = 2;
  m_scrollTime = 0.025;
  m_backRed = 102;//;
  m_backGreen = 153;//0;
  m_backBlue = 255;//0;  
  m_timerValue = 40;
  
  for(int i = 0; i < numberLines; ++i){
    p1 = ""; p2 = ""; p3 = ""; p4 = ""; p5 = ""; p6 = ""; p7 = "";
    if(cfr.GetNumberOfTermsFromLine(i) > 0){
      lineType = cfr.GetTerm(cfr.GetLineFromFile(i), 1);

      //draw sprite
      if(lineType == "sprite" && cfr.GetNumberOfTermsFromLine(i) == 3){
        //pLog->Log("credit sprite");
        p2 = cfr.GetTerm(cfr.GetLineFromFile(i), 2);
        p3 = cfr.GetTerm(cfr.GetLineFromFile(i), 3);
        m_msg.push_back(SetGraphic(atof(p2.c_str()), (atoi)(p3.c_str())));
      }

      //add blank vertical space
      else if(lineType == "vspace"){
        //pLog->Log("credit vspace");
        m_msg.push_back(ClearMessage());
      }

      //draw text
      else if(lineType == "text"){
        //pLog->Log("credit text");
        p2 = cfr.GetTerm(cfr.GetLineFromFile(i), 2);
        p3 = cfr.GetTerm(cfr.GetLineFromFile(i), 3);
        p4 = cfr.GetTerm(cfr.GetLineFromFile(i), 4);
        p5 = cfr.GetTerm(cfr.GetLineFromFile(i), 5);
        p6 = cfr.GetTerm(cfr.GetLineFromFile(i), 6);
        p7 = cfr.GetTerm(cfr.GetLineFromFile(i), 7);
        m_msg.push_back(SetMessage(atof(p2.c_str()), atoi(p3.c_str()), 
                                   atoi(p4.c_str()), atoi(p5.c_str()), atoi(p6.c_str()),
                                   p7));
      }

      //loads parameters 
      else if(lineType == "scrollspeed" && cfr.GetNumberOfTermsFromLine(i) == 2){
        p2 = cfr.GetTerm(cfr.GetLineFromFile(i), 2);
        m_scrollSpeed = atoi(p2.c_str());
        //pLog->Log("scrollspeed", m_scrollSpeed, p2);
      }

      else if(lineType == "scrolltime" && cfr.GetNumberOfTermsFromLine(i) == 2){
        p2 = cfr.GetTerm(cfr.GetLineFromFile(i), 2);
        m_scrollTime = (double)(atof(p2.c_str()));
        //pLog->Log("scrolltime", m_scrollTime, p2);
      }

      else if(lineType == "backcolor" && cfr.GetNumberOfTermsFromLine(i) == 4){
        p2 = cfr.GetTerm(cfr.GetLineFromFile(i), 2);
        p3 = cfr.GetTerm(cfr.GetLineFromFile(i), 3);
        p4 = cfr.GetTerm(cfr.GetLineFromFile(i), 4);
        m_backRed = atoi(p2.c_str());
        m_backGreen = atoi(p3.c_str());
        m_backBlue = atoi(p4.c_str());
      }

      else if(lineType == "timer" && cfr.GetNumberOfTermsFromLine(i) == 2){
        p2 = cfr.GetTerm(cfr.GetLineFromFile(i), 2);
        m_timerValue = atoi(p2.c_str());
        //pLog->Log("scrollspeed", m_scrollSpeed, p2);
      }
    }
  }

  m_timer.Initialize(); 
  m_scrollValue = 0;

  //used to show several images in the background
  m_secondTimer.Initialize();
  m_secondCount = 0;

  //used for fading
  m_red = 255;//255;
  m_green =255;//255;
  m_blue = 255;//255;
  m_fadeTimer.Initialize();
  
}

void CGameStateCredits::Activate(CGameData &data, CConfigData &cfg, CGraphics &con){

}

CGameStateObject* CGameStateCredits::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  static double timeDiffTotal = 0;
  GRAPHIC_IMAGE gi;

  //CAudioManager *pAudio = CAudioManager::Instance();
  m_mouse.SetHandle(data.m_hWnd);
 // CAudioManager *pAudio = CAudioManager::Instance();
  con.m_textureRed = 55;//data.m_textureColorRed;
  con.m_textureGreen = 55;//data.m_textureColorGreen;
  con.m_textureBlue = 55;//data.m_textureColorBlue;
  //play background music
  //if(cfg.PlayMusic == true && pAudio->IsPlaying(4) == false)
  //  pAudio->PlayAudioClip(4, 1);

  //one time code
  static bool bFirstTime = false;
  int width = 0;
  size_t letterCount = 0;

  //setup message scrolling parameters
  if(bFirstTime == false){

    for(unsigned int i = 0; i < m_msg.size(); ++i){

      //graphic image
      if (m_msg[i].type == 1){
        m_msg[i].gi = con.GetSprite(m_msg[i].spriteID);
        if(m_msg[i].x == -1) //center sprite
          m_msg[i].x = (float)(con.GetScreenWidth()/2 - m_msg[i].gi.width/2);
      }

      //font size - attempt to center text
      else {
        letterCount = m_msg[i].msg.size();
        width = 9;
        if(m_msg[i].font == 0 || m_msg[i].font == 1 || m_msg[i].font == 6 || m_msg[i].font == 7)
          width = 5;
        else if(m_msg[i].font == 2 || m_msg[i].font == 3 || m_msg[i].font == 8 || m_msg[i].font == 9)
          width = 6;
        else if(m_msg[i].font == 4 || m_msg[i].font == 5 || m_msg[i].font == 10 || m_msg[i].font == 11)
          width = 7;
        else if(m_msg[i].font == 12 || m_msg[i].font == 13)
          width = 9;
        if(m_msg[i].x == -1) //center font
          m_msg[i].x = (float)(con.GetScreenWidth()/2 - (letterCount * width)/2);
      }
    }
    bFirstTime = true;
  }

  static double mouseEntryTime = 0;
  mouseEntryTime += timeDifference;
  if(::GetActiveWindow() == data.m_hWnd && mouseEntryTime > 1.0 && (data.m_bLeftMouseDown || keyDown(VK_ESCAPE)))
    m_event = EVENT_GO_QUIT;

  //terminate program after scrolling greater than 15 seconds
  if(m_secondCount > m_timerValue)// 20)
    m_event = EVENT_GO_QUIT;

  //update counter for displaying images
  if(m_secondTimer.secondTimer() == true){
    m_secondCount++;
  }

  //tracks time
  timeDiffTotal += timeDifference;
  if(timeDiffTotal > m_scrollTime){
    timeDiffTotal = 0;
    m_scrollValue += m_scrollSpeed;  
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

int CGameStateCredits::Render(CGraphics &con,  CGameData &data, CConfigData &cfg){

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
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(m_backRed, m_backGreen, m_backBlue), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene
  con.BeginRender();
  
  GRAPHIC_IMAGE gi; 
  gi = con.GetSprite(2000);
  con.RenderSprite(gi, 0, 0, 255, 255,255);// m_posX, m_posY, m_scale, m_scale, 512, 10);

  //render credits
  //GRAPHIC_IMAGE gi;
  //gi = con.GetSprite(2);
  //con.RenderSprite(gi, cfg.ScreenWidth - 240, 40, 255, 255, 255);
  con.EndRender();
  //scrolls message
  size_t posY = 0;
  
  for(size_t i = 0; i < m_msg.size(); i++){
    posY = m_scrollValue + ((int)m_msg.size() - i * 20);
    if(m_msg[i].type == 0)  //font
      con.Print(m_msg[i].msg, m_msg[i].font , m_msg[i].x, con.GetScreenHeight() - posY, m_msg[i].red, m_msg[i].green, m_msg[i].blue,255);
    else{//graphic
      con.RenderSprite(m_msg[i].gi, m_msg[i].x, con.GetScreenHeight() - posY, 255, 255, 255);
    }
  }

  //con.Print("Control Screen", F_V20B, cfg.ScreenWidth/2 - 60, 100, 255, 255, 255, 255);
  //con.Print("Press ESCAPE to Continue!", F_V20B, cfg.ScreenWidth/2 - 130, cfg.ScreenHeight - 100, 255, 255, 0, 255);

  //if(cfg.DisplayDebugInfo == true){
  //  con.Print("Version", cfg.ProgramVersion, F_V16, 40, 730,  255, 255, 255, 255);
  //  con.Print("FPS", data.m_FPS , F_V16, 930, 730,  255, 255, 255, 255);    
  //}  

  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

  return 0;
}

void CGameStateCredits::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void CGameStateCredits::Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con){
//  CAudioManager *pAudio = CAudioManager::Instance();
//  pAudio->KillAll();

}
void CGameStateCredits::Resume(){
}
void CGameStateCredits::Pause(){
}
void CGameStateCredits::Save(){
}

void CGameStateCredits::AddTransitionEvent(int event, CGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

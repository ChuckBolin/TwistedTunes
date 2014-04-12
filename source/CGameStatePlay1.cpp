#include "..\include\CGameStatePlay1.h"
 
CGameStatePlay1::CGameStatePlay1(){
}

CGameStatePlay1::~CGameStatePlay1()
{
	m_TE.clear();
}

void CGameStatePlay1::Initialize(){

}

void CGameStatePlay1::Activate(CGameData &data, CConfigData &cfg, CGraphics &con){
  CLog *pLog = CLog::Instance();



  //file toolbar
  //m_bChanged = false;
  m_fileLevel = 0;//
  //m_gameTile.LoadGameLevel("assets\\data\\level1.dat");
  //pAudio->LoadAudioData("assets\\data\\sounds.dat");

  //if(data.OpenLevel(m_fileLevel, con) == true){
  //}

}

void CGameStatePlay1::Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con){

}

//***************************************************************
//                         U P D A T E
//***************************************************************
CGameStateObject* CGameStatePlay1::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  CLog *pLog = CLog::Instance();
  ::ShowCursor(true);
  //CFizzix *pFizzix = CFizzix::Instance();  
   
  if(::GetActiveWindow() != data.m_hWnd)
    return NULL;

  m_mouse.SetHandle(data.m_hWnd);//needed for mouse
  //temporarily used to allow suer control of colors...keys UIOJKL
  con.m_textureRed = data.m_textureColorRed;
  con.m_textureGreen = data.m_textureColorGreen;
  con.m_textureBlue = data.m_textureColorBlue;


  //manages time
//  timeDifference *= data.m_timeScale; //multiply by 1 or 2
  static double keyTime = 0;
  keyTime += timeDifference;
  static double pauseTime = 0;
  pauseTime += timeDifference;


  CAudioManager *pAudio = CAudioManager::Instance();
  pAudio->SetPlayerPosition(m_mouse.MouseX(), m_mouse.MouseY(), m_mouse.MouseY());

  //pass mouse information to tile so user may move it
  if(data.m_bPaused == false){
    if(data.m_bLeftMouseDown == true)
      m_gameTile.MouseDown(m_mouse.MouseX(), m_mouse.MouseY());
    else
      m_gameTile.MouseUp(m_mouse);

    if(data.m_bRightMouseDown == true)
      m_gameTile.RightMouseDown(m_mouse.MouseX(), m_mouse.MouseY());
    else
      m_gameTile.RightMouseUp(m_mouse);
    
    m_gameTile.Update(timeDifference, data, m_mouse);
  }
  //if mouse button is down and inside a tile, capture dx,dy

  //pLog->Log("Before");
  //pAudio->PlaySounds(1);

    /*
    pAudio->PlaySounds(3);
    pAudio->PlaySounds(4);
    pAudio->PlaySounds(5);
    pAudio->PlaySounds(6);   
    */
  //pLog->Log("After");
  pAudio->Update(timeDifference, data.m_bPaused);
    //pAudio->PlaySounds(12);
  static double fileTime = 0;
  fileTime += timeDifference;

  //******************************************************* PROCESS INPUT
  if(::GetActiveWindow() == data.m_hWnd){// && data.m_bPaused == false){
    
    if(m_gameTile.GetTotalActiveSongs() == 0){
      m_fileLevel++;
      
      m_gameTile.SetLevel(m_fileLevel);

    //if(keyDown('1') == true){
      if(m_fileLevel == 1){//|| keyDown('1') == true){
        data.m_bGameOver = false;
        pAudio->LoadAudioData("assets\\data\\sound1.dat");
        m_gameTile.LoadGameLevel("assets\\data\\level1.dat");      
      }
    //else if(keyDown('2') == true){

      else if(m_fileLevel == 2){// || keyDown('2') == true){
        pAudio->LoadAudioData("assets\\data\\sound2.dat");
        m_gameTile.LoadGameLevel("assets\\data\\level2.dat");      
      }
    //else if(keyDown('3') == true){
      else if(m_fileLevel == 3){// || keyDown('3') == true){
        pAudio->LoadAudioData("assets\\data\\sound3.dat");
        m_gameTile.LoadGameLevel("assets\\data\\level3.dat");      
      }
    //else if(keyDown('4') == true){
      else if(m_fileLevel == 4 ){//|| keyDown('4') == true){
        pAudio->LoadAudioData("assets\\data\\sound4.dat");
        m_gameTile.LoadGameLevel("assets\\data\\level4.dat");      
      }
    //else if(keyDown('5') == true){
      else if(m_fileLevel == 5){// || keyDown('5') == true){
        pAudio->LoadAudioData("assets\\data\\sound5.dat");
        m_gameTile.LoadGameLevel("assets\\data\\level5.dat");      
      }
      else{
        if(m_fileLevel > 5){
          data.m_bGameOver = true;    
        }
      }

      
    }

    //pause
    if(keyDown(VK_SPACE) == true && pauseTime > 0.5){
      pauseTime = 0;
      data.m_bPaused = !data.m_bPaused;
      //if(data.m_bPaused == true)
      //  pAudio->PauseAllSounds();
      //else
      //  pAudio->UnPauseAllSounds();
    }

    //if(data.m_bLeftMouseDown)
    if(keyDown(VK_ESCAPE) == true && keyTime > 0.5)
      //m_event = EVENT_GO_MAIN;
      m_event = EVENT_GO_CREDITS;

    //if(keyDown(VK_UP) == true && keyTime > 0.3){
    //  keyTime = 0;
    //  //m_tileLevel.IncTileID();
    //}
    //if(keyDown(VK_DOWN) == true && keyTime > 0.3){
    //  keyTime = 0;
    //  //m_tileLevel.DecTileID();
    //}
    
    //clear assembly area..testing
    if(keyDown('C') == true && keyTime > 0.5){
      keyTime = 0;
      m_gameTile.ResetAssemblyArea();
      //m_tileLevel.ToggleGrid();
    }
  }
  

  //*******************************************************  
//  m_tileLevel.Update(data.m_posX, data.m_posY, m_mouse.MouseX(), m_mouse.MouseY(),timeDifference, data);
//  m_objectManager.Update(timeDifference, data, cfg, con);

/*
  if(keyDown(VK_LSHIFT) == true && m_addTarget == ADD_OBJECT && data.m_bShowMap == false && data.m_bLeftMouseDown == true && m_mouse.MouseY() > 56 && m_mouse.MouseY() < 690 && keyTime > 0.3){
    keyTime = 0;    
    m_objectManager.AddObject(data.m_posX - m_mouse.MouseX(), data.m_posY - m_mouse.MouseY(), 
                              m_tileLevel.GetTileYOffset(data.m_posX - m_mouse.MouseX(), data.m_posY - m_mouse.MouseY()), con);
  }
*/
  //add thing to tiles
  /*
  if(m_addTarget == ADD_OBJECT && data.m_bShowMap == false && data.m_bLeftMouseDown == true && m_mouse.MouseY() > 56 && m_mouse.MouseY() < 690 && keyTime > 0.3){
    keyTime = 0;    
    long objectRow = m_tileLevel.CalcIsoRow(data.m_posX - m_mouse.MouseX(), data.m_posY - m_mouse.MouseY(), data.m_viewMode);
    long objectCol = m_tileLevel.CalcIsoCol(data.m_posX - m_mouse.MouseX(), data.m_posY - m_mouse.MouseY(), data.m_viewMode);
    m_objectManager.AddObject(m_tileLevel.GetIsoX(objectCol, objectRow, data.m_viewMode),
                              m_tileLevel.GetIsoY(objectCol, objectRow, data.m_viewMode),
                              m_tileLevel.GetTileYOffset(data.m_posX - m_mouse.MouseX(), data.m_posY - m_mouse.MouseY()),
                              objectRow,
                              objectCol,
                              con);
  }*/

  // data.m_gameClock.Update();
  if(data.m_bPaused == false && data.m_bGameOver == false)
    data.Update(timeDifference);  


  

  //manage game state changes
  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

int CGameStatePlay1::IsLost(CGraphics &con, CConfigData &cfg){
	HRESULT hr 				= 0;
	static bool bLostDevice = false;
	CLog *pLog 				= CLog::Instance();
	
	if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
		if(hr == D3DERR_DEVICELOST){
			if(bLostDevice == false){
				pLog->Log("Lost: IDirect3DDevice9. cleaning up.");
				con.DeleteLost();
				bLostDevice = true;
			}

	    return 1;
  	}
    else if(hr == D3DERR_DEVICENOTRESET){
			if(bLostDevice == false){
				pLog->Log("Reset: IDirect3DDevice9. cleaning up.");
				con.DeleteLost();
				bLostDevice = true;
			}
		
			hr = con.m_d3ddev->Reset(&con.GetDPP());      
		
			if(SUCCEEDED(hr)){
				pLog->Log("Reloading: IDirect3DDevice9::Reset() successful.");
				con.LoadAssetFile(cfg.GamePlayAssetFile);
				con.LoadAssetFile(cfg.FrameworkAssetFile);     
				con.ReloadLost();
				bLostDevice = false;
			}
			else if(FAILED(hr)){
				pLog->Log("Critical Error: IDirect3DDevice9::Reset() Failed to reset. HResult", hr);
				return 2;
			}
		}
	}
	return 0;
}

//***************************************************************
//                         R E N D E R 
//***************************************************************
int CGameStatePlay1::Render(CGraphics &con,  CGameData &data, CConfigData &cfg){
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
        //pLog->Log("Lost");
        con.DeleteLost();    //delete one time per failure

        //pLog->Log("Deleted");
      }
      return 0;
    }

    //device was lost...but now can be RESET
    else if( hr == D3DERR_DEVICENOTRESET ){
      hr = con.m_d3ddev->Reset(&con.GetDPP());      
      if(SUCCEEDED(hr)){
        //pLog->Log("Loss of focus...reloading!");

        con.LoadAssetFile(cfg.GamePlayAssetFile);
        con.LoadAssetFile(cfg.FrameworkAssetFile);     
        con.ReloadLost();
        bLostDevice = false;
      }
      return 0;
    }
  }
  
  // clear the window to a deep blue
  //con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(data.m_screenColorRed, data.m_screenColorGreen, data.m_screenColorBlue), 1.0f, 0);//0, 111, 255
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);//0, 111, 255
  con.m_d3ddev->BeginScene();  
  con.BeginRender();

  GRAPHIC_IMAGE gi;

  //current system
  //m_tileLevel.Render(con, data);//render all tiles
  //m_objectManager.Render(con, data, cfg);//render all things
/*
  //render visible tiles and objects
  for(int i = 0; i < m_tileLevel.MAX_ROWS; ++i){
    for(int j = 0; j < m_tileLevel.MAX_COLS; ++j){
      m_tileLevel.Render(con, data, j, i);
      m_objectManager.Render(con, data, cfg, i, j);

    }
  }
*/


  //render map
  //gi = con.GetSprite(data.m_mapSpriteNumber);//103
  //con.RenderSprite(gi, 0, 0, 255, 255, 255);
  //renders all game objects
  //for(int layer = 0; layer < 11; ++layer){
  //  for(unsigned int i = 0; i < data.m_pObject.size(); ++i){
  //    if(data.m_pObject[i]->GetLayer() == layer)
  //        data.m_pObject[i]->Render(con, data);
  //  }
  //}
  

  //render UI panels
  //m_UI.Render(con, data);

  //paused
  //if(data.m_bPaused == true){
  //  gi = con.GetSprite(1011);
  //  con.RenderSprite(gi, con.GetScreenCenterX() - gi.width/2, con.GetScreenCenterY() - gi.height/2, 255, 255, 255);
  //}

  //render background image
  if(m_fileLevel == 1)
    gi = con.GetSprite(101);
  else if(m_fileLevel == 2)
    gi = con.GetSprite(102);
  else if(m_fileLevel == 3)
    gi = con.GetSprite(103);
  else if(m_fileLevel == 4)
    gi = con.GetSprite(104);
  else if(m_fileLevel == 5)
    gi = con.GetSprite(105);

  //int gw = gi.width;
  //int gh = gi.height;
  //float gs = gi.scale;
  //int sprite = 106 + (rand() % 5);
  //con.RenderSprite(gi, con.GetScreenCenterX() - gi.width/2, con.GetScreenCenterY() - gi.height/2, 255, 255, 255);
  gi.alpha = 160;
  con.RenderSprite(gi, 0, 0, 255, 255, 255);
  gi = con.GetSprite(111);
  gi.alpha = 180;
  con.RenderSprite(gi, 0, -16, 255, 255, 255);

  gi = con.GetSprite(121);
  con.RenderSprite(gi, 412, 51, 255, 255, 255);

  gi = con.GetSprite(120);
  con.RenderSprite(gi, 156, 114, 255, 255, 255);


  //render user interface
  //gi = con.GetSprite(111);
  //con.RenderSprite(gi, 260, 97, 255, 255, 255);//move left
  //gi = con.GetSprite(112);
  //con.RenderSprite(gi, 330, 97, 255, 255, 255);//move right
  //gi = con.GetSprite(116);
  //con.RenderSprite(gi, 448, 97, 255, 255, 255);//background
 // gi = con.GetSprite(113);
  //con.RenderSprite(gi, 648, 97, 255, 255, 255);  

  //render tiles
  if(data.m_bGameOver == false)
    m_gameTile.Render(con, data,m_mouse);
  
  /*
  for(int x = 0; x < 20; ++x){
    for(int y = 0; y < 15; ++y){
      con.RenderSprite(gi, 186 + (x * 32), 148 + (y * 32), 255, 255, 255);
    }
  }
  */

  //file toolbar UI
  //***********************************************************
  //gi = con.GetSprite(2010);
/*
  gi = con.GetSprite(2011);//open
  con.RenderSprite(gi, 3, 3, 255, 255, 255);
*/
  //***********************************************************
  
/*
  //show map
  if(data.m_bShowMap == true){
    m_tileLevel.RenderMap(con, cfg);
  }
*/
  con.EndRender();  

  if(data.m_bGameOver == false){
    con.Print("Bonus Timer", m_gameTile.GetTimer(),  F_V20, 170, 30, 255, 255, 255, 255);
    con.Print("Level", m_fileLevel, F_V20, 470, 30, 0, 255, 255, 255);
  }

  if(data.m_bGameOver == false){
    con.Print("Score", m_gameTile.GetScore(), F_V20, 750, 30, 0, 255, 255, 255);
  }

  con.Print("Press ESC to Quit!",F_V20, 420, 730, 255, 255, 255, 255);

  if(data.m_bPaused == true && data.m_bGameOver == false){    
    con.Print("PAUSED",F_V20, 464, 360, 255, 255, 255, 255);
  }

  if(data.m_bGameOver == true){
    con.Print("GAME OVER",F_V20, 444, 360, 255, 255, 255, 255);
    con.Print("Score", m_gameTile.GetScore(), F_V20, 440, 390, 0, 255, 255, 255);
  }  

  //renders UI panel outlines
//  m_UI.RenderOutlines(con, data);


  //m_tileLevel.DrawGrid(con);

  //draw mouse horizontal lines
  //con.DrawLine(m_mouse.MouseX(), 0, m_mouse.MouseX(), 768, 0, 255, 255);
  //con.DrawLine(0, m_mouse.MouseY(), 1024, m_mouse.MouseY(), 0, 255, 255);

  //draw mouse diagonal lines
  //con.DrawLine(m_mouse.MouseX(), m_mouse.MouseY(), 0, m_yIntercept1, 255, 255, 0);  
  //con.DrawLine(m_mouse.MouseX(), m_mouse.MouseY(), 0, m_yIntercept2, 255, 255, 255);
  //con.DrawRect(m_tileX, m_tileY, m_tileX + 86.6, m_tileY + 50, 255, 255, 0);

  //display filename
  //con.Print(m_filenames[m_fileLevel],F_V16B, 114, 8, 255, 255, 0, 255);

  //  con.DrawRect(299, 5, 313, 27, 255, 255, 255);

  
  if(cfg.DisplayDebugInfo == true){
    
    //con.Print("FPS", data.m_FPS, F_V20, 7, 37, 255, 255, 255, 255);
    //con.Print("W", gw, F_V20, 7, 57, 255, 255, 255, 255);
    //con.Print("H", gh, F_V20, 7, 77, 255, 255, 255, 255);
    //con.Print("S", gs, F_V20, 7, 97, 255, 255, 255, 255);

   // con.Print("Mouse X", m_mouse.MouseX(), F_V20, 860, 700, 255, 255, 255, 255);
   // con.Print("Mouse Y", m_mouse.MouseY(), F_V20, 860, 720, 255, 255, 255, 255);
    //con.Print("Score", m_gameTile.GetScore(), F_V20, 860, 43, 0, 255, 255, 255);
    /*
    con.Print("IsoRow", m_tileLevel.CalcIsoRow(data.m_posX - m_mouse.MouseX(),data.m_posY - m_mouse.MouseY(), m_tileLevel.GetViewMode()), F_V20, 860, 81, 255, 255, 255, 255);
    con.Print("IsoCol", m_tileLevel.CalcIsoCol(data.m_posX - m_mouse.MouseX(),data.m_posY - m_mouse.MouseY(),  m_tileLevel.GetViewMode()), F_V20, 860, 103, 255, 255, 255, 255);
    con.Print("Cursor X", (int)data.m_posX, F_V20, 860, 125, 255, 255, 255, 255);
    con.Print("Cursor Y", (int)data.m_posY, F_V20, 860, 147, 255, 255, 255, 255);
    */
  }

  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
  
  return 0;
}
  
void CGameStatePlay1::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

}
void CGameStatePlay1::Resume(){
}
void CGameStatePlay1::Pause(){
}
void CGameStatePlay1::Save(){
}

void CGameStatePlay1::AddTransitionEvent(int event, CGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}


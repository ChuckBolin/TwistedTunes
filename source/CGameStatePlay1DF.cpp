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
  
  //pyro setup
  //**********************
  //m_effect.Init(con.m_d3ddev);
  //m_effect.LoadFile(cfg.EffectsFile);//"assets\\data\\effects.dat");
  //m_effectNumber = 0;
  //m_effectMax = m_effect.GetNumberEffects();
  //pLog->Log("******* Number of effects loaded", m_effectMax);

  //audio setup
  /*
  CAudioManager *pAudio = CAudioManager::Instance();
  if(cfg.PlayMusic == 1)
    data.m_playMusic = true;
  else
    data.m_playMusic = false;

  if(cfg.PlaySounds == 1)
    data.m_playSound = true;
  else
    data.m_playSound = false;
*/

  //tiles setup
  //data.m_posX = -2536; //50X50...CENTERS SCREEN
  //data.m_posY = - 2800;
  m_tileLevel.LoadTileDefinitions(con, cfg); 

  //things setup
  m_objectManager.LoadObjectDefinitions(con, cfg); 

  //file toolbar
  m_bChanged = false;
  m_fileLevel = 1;
  m_minRatingLevel = 1;
  m_filenames[1] = "level1.dat";
  m_filenames[2] = "level2.dat";
  m_filenames[3] = "level3.dat";
  m_filenames[4] = "level4.dat";
  m_filenames[5] = "level5.dat";
  m_filenames[6] = "level6.dat";
  m_filenames[7] = "level7.dat";
  m_filenames[8] = "level8.dat";
  m_filenames[9] = "level9.dat";
  m_filenames[10] = "level10.dat";
  m_filenames[11] = "level11.dat";
  m_filenames[12] = "level12.dat";
  m_filenames[13] = "level13.dat";
  m_filenames[14] = "level14.dat";
  m_filenames[15] = "level15.dat";
  m_filenames[16] = "level16.dat";
  m_filenames[17] = "level17.dat";
  m_filenames[18] = "level18.dat";
  m_filenames[19] = "level19.dat";
  m_filenames[20] = "level20.dat";
  m_filenames[21] = "level21.dat";
  m_filenames[22] = "level22.dat";

  //tile select interface
  m_iconTileSelectIndex = 0;  //position of left most icon displayed in a frame
  m_iconSelectPosition = 0;  //controls scrolling of icon selection
  m_iconTotalTiles = m_tileLevel.GetTileDefSize(); 

  m_keyMode = 0;//move orthogoanlly
  m_scale = 1.0;
  m_currentGroupID = 0;

  m_addTarget = 0; // 0 = tiles, 1 = things, 2 = sounds
}

void CGameStatePlay1::Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con){
  //pyro housekeeping
  //********************************
  //m_effect.Delete();
}

//***************************************************************
//                         U P D A T E
//***************************************************************
CGameStateObject* CGameStatePlay1::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
  CLog *pLog = CLog::Instance();
//  CAudioManager *pAudio = CAudioManager::Instance();
  CFizzix *pFizzix = CFizzix::Instance();  
   
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
  static double effectTime = 0;
  effectTime += timeDifference;
  static double fileTime = 0;
  fileTime += timeDifference;

  //******************************************************* PROCESS INPUT
  if(::GetActiveWindow() == data.m_hWnd){// && data.m_bPaused == false){
  }
    //if(data.m_bLeftMouseDown)
    if(keyDown(VK_ESCAPE) == true)
      //m_event = EVENT_GO_MAIN;
      m_event = EVENT_GO_QUIT;
    if(keyDown(VK_UP) == true && keyTime > 0.3){
      keyTime = 0;
      m_tileLevel.IncTileID();
    }
    if(keyDown(VK_DOWN) == true && keyTime > 0.3){
      keyTime = 0;
      m_tileLevel.DecTileID();
    }

    //display grid
    if(keyDown('G') == true && keyTime > 0.5){
      keyTime = 0;
      m_tileLevel.ToggleGrid();
    }

    //clears all tiles
    if(keyDown('C') == true && keyTime > 0.5){
      keyTime = 0;
      m_tileLevel.ClearTiles();      
    }

    if(keyDown('E') == true && keyTime > 0.2){
      keyTime = 0;
      //m_scale -= 0.01;
      //m_tileLevel.SetLevelScale(m_scale);
    }
    if(keyDown('R') == true && keyTime > 0.2){
      keyTime = 0;
      //m_scale += 0.01;
      //m_tileLevel.SetLevelScale(m_scale);
    }

    if(keyDown('Z') == true && keyTime > 0.2){
      keyTime = 0;
      if(m_addTarget == 0)
        m_addTarget = 1;   //add things
      else
        m_addTarget = 0;   //add tiles
    }

    //toggles direction of movement of WASD keys
    if(keyDown('Q') == true && keyTime > 0.5){
      if(m_keyMode == 0)
        m_keyMode = 1;
      else
        m_keyMode = 0;
      keyTime = 0;
    }

    //move around in iso world
    if(keyDown('W') == true){
      if(m_keyMode == 0)
        data.m_posY += (float)( 750 * timeDifference);
      else{
        data.m_posY += (float)(750 * timeDifference);
        data.m_posX -=  (float)(1500 * timeDifference);
      }
    }
    if(keyDown('A') == true){
      if(m_keyMode == 0)
        data.m_posX += (float)( 750 * timeDifference);
      else{
        data.m_posY +=  (float)(750 * timeDifference);
        data.m_posX +=  (float)(1500 * timeDifference);
      }
    }
    if(keyDown('S') == true){
      if(m_keyMode == 0)
        data.m_posY -=  (float)(750 * timeDifference);
      else{
        data.m_posY -=  (float)(750 * timeDifference);
        data.m_posX +=  (float)(1500 * timeDifference);
      }
    }
    if(keyDown('D') == true){
      if(m_keyMode == 0)
        data.m_posX -=  (float)(750 * timeDifference);
      else{
        data.m_posX -=  (float)(1500 * timeDifference);
        data.m_posY -=  (float)(750 * timeDifference);
      }
    }

    if(keyDown('M') == true && keyTime > 0.3){
      keyTime = 0;
      data.m_bShowMap = !data.m_bShowMap;
    }
    
    if(keyDown('V') == true && keyTime > 0.3){
      keyTime = 0;
      m_currentGroupID--;
      if(m_currentGroupID < 0)
        m_currentGroupID = m_tileLevel.GetTileDefSize() - 1;

      m_tileLevel.SetGroupNumber(m_currentGroupID);
      m_iconTileSelectIndex = 0;
    }
    if(keyDown('F') == true && keyTime > 0.3){
      keyTime = 0;
      m_currentGroupID++;
      if(m_currentGroupID > m_tileLevel.GetTileDefSize() - 1)
        m_currentGroupID = 0;

      m_tileLevel.SetGroupNumber(m_currentGroupID);
      m_iconTileSelectIndex = 0;
    }

    if(keyDown('B') == true && keyTime > 0.3){
      keyTime = 0;
      if(data.m_brushSize == 1)
        data.m_brushSize = 2;
      else
        data.m_brushSize = 1;
    }

    if(keyDown('I') == true){
      data.m_posX = -2604;
      data.m_posY = - 1279;
    }
    if(keyDown('J') == true){
      data.m_posX = 385;
      data.m_posY = -2882;
    }
    if(keyDown('K') == true){
      data.m_posX = -2570;
      data.m_posY = -4394;
    }
    if(keyDown('L') == true){
      data.m_posX = -5803;
      data.m_posY = -2818;
    }

    if(keyDown('1') == true && keyTime > 1.0){
      keyTime = 0;
      m_tileLevel.SetViewMode(0);
      //m_viewMode = 0;
    }
    if(keyDown('2') == true && keyTime > 1.0){
      keyTime = 1;
      m_tileLevel.SetViewMode(1);
      //m_viewMode = 1;
    }


  //*******************************************************  
  m_tileLevel.Update(data.m_posX, data.m_posY, m_mouse.MouseX(), m_mouse.MouseY(),timeDifference, data);

  //add tile
  if(m_addTarget == 0 && data.m_bShowMap == false && data.m_bLeftMouseDown == true && m_mouse.MouseY() > 56 && m_mouse.MouseY() < 690){
    m_tileLevel.AddTile();
  }
  //add tile via map
  if(m_addTarget == 0 && data.m_bShowMap == true && data.m_bLeftMouseDown == true && m_mouse.MouseY() > 56 && m_mouse.MouseY() < 690){
    m_tileLevel.AddTileViaMap(m_mouse.MouseX(), m_mouse.MouseY(), data.m_brushSize);
  }

  //add thing
  if(m_addTarget == 1 && data.m_bShowMap == false && data.m_bLeftMouseDown == true && m_mouse.MouseY() > 56 && m_mouse.MouseY() < 690 && keyTime > 0.3){
    keyTime = 0;
    
    m_objectManager.AddObject(0, data.m_posX - m_mouse.MouseX(), data.m_posY - m_mouse.MouseY(), 0, con);
  }


  //icon select at bottom of screen
  //***************************************************************
  else if(data.m_bLeftMouseDown == true && m_mouse.MouseY() > 690){
    
    //scroll list left
    if(m_mouse.MouseX() > 0 && m_mouse.MouseX() < 34){
      m_iconSelectPosition += 400 * timeDifference;
      if(m_iconSelectPosition > 0)
        m_iconSelectPosition = 0;
    }
    //scroll list right
    else if(m_mouse.MouseX() > 990 && m_mouse.MouseX() < 1024){ //upper limit
      m_iconSelectPosition -= 400 * timeDifference;
      //if(m_iconSelectPosition < -70 * m_tileLevel.GetTileDefSize())//lower limit
      //  m_iconSelectPosition = -70 * m_tileLevel.GetTileDefSize();
      if(m_iconSelectPosition < -70 * m_tileLevel.GetGroupSize())//lower limit
        m_iconSelectPosition = -70 * m_tileLevel.GetGroupSize();
      
    }
    else{//could be clicking on an icon
      int iconPosX = -m_iconSelectPosition + 32 + m_mouse.MouseX();
      iconPosX = iconPosX/70;
      if(iconPosX > 0 && iconPosX < m_tileLevel.GetTileDefSize() + 1){
        m_iconTileSelectIndex = iconPosX - 1; 
        m_tileLevel.SetGroupTileID(m_iconTileSelectIndex);
        //m_tileLevel.SetTileID(m_iconTileSelectIndex);
      }
    }
    /*
    else{//could be clicking on an icon
      int iconPosX = -m_iconSelectPosition + 32 + m_mouse.MouseX();
      int iconIndex = iconPosX/70;
      int iconInvIndex = (-m_iconSelectPosition + 32)/70;

      int mousePosX = abs((iconPosX - 64)/70) + (m_iconSelectPosition/-64); //<<<<<<<<<<<< fix this
      mousePosX -= iconInvIndex;
      
      if(mousePosX > -1 && mousePosX < m_tileLevel.GetTileDefSize()){
        m_iconTileSelectIndex = mousePosX; 
        m_tileLevel.SetTileID(m_iconTileSelectIndex);
      }
    }
    */

  }
 
  //file toolbar user interface at top of game
  //**************************************************************
  else if(data.m_bLeftMouseDown == true && m_mouse.MouseY() < 33 + data.m_windowedYOffset && fileTime > 0.2){
    fileTime = 0;
    if(m_mouse.MouseX() > 221 && m_mouse.MouseX() < 233){ //increment file name
      if(m_mouse.MouseY() < 17 + data.m_windowedYOffset){
        m_fileLevel++;
        if(m_fileLevel > 22)
          m_fileLevel = 22;
      }
      else if(m_mouse.MouseY() > 17 + data.m_windowedYOffset){//decrement file name
        m_fileLevel--;
        if(m_fileLevel < 1)
          m_fileLevel = 1;
      }
    }
    else if(m_mouse.MouseX() > 6 && m_mouse.MouseX() < 56){// open file
      m_tileLevel.LoadLevelFile(cfg.RootPath + m_filenames[m_fileLevel]);
      m_objectManager.LoadLevelFile(cfg.RootPath + m_filenames[m_fileLevel]);
    }
    else if(m_mouse.MouseX() > 59 && m_mouse.MouseX() < 107){// save file
      m_tileLevel.SaveLevelFile(cfg.RootPath + m_filenames[m_fileLevel]); //rewrites old file if it exists
      m_objectManager.SaveLevelFile(cfg.RootPath + m_filenames[m_fileLevel]); //appends file

    }
    else if(m_mouse.MouseX() > 238 && m_mouse.MouseX() < 296){// clear level
      m_tileLevel.ClearTiles();   
    }
    else if(m_mouse.MouseX() > 964 && m_mouse.MouseX() < 990){//toggle grid
      m_tileLevel.ToggleGrid();
    }
    else if(m_mouse.MouseX() > 994 && m_mouse.MouseX() < 1018){//exit
      //m_event = EVENT_GO_MAIN;
      m_event = EVENT_GO_QUIT;
    }
    else if(m_mouse.MouseX() > 299 && m_mouse.MouseX() < 313){//rating 1
      m_minRatingLevel = 1;
      m_tileLevel.SetCurrentRating(m_minRatingLevel);
    }
    else if(m_mouse.MouseX() > 313 && m_mouse.MouseX() < 325){//rating 2
      m_minRatingLevel = 2;
      m_tileLevel.SetCurrentRating(m_minRatingLevel);
    }
    else if(m_mouse.MouseX() > 325 && m_mouse.MouseX() < 337){//rating 3
      m_minRatingLevel = 3;
      m_tileLevel.SetCurrentRating(m_minRatingLevel);
    }
    else if(m_mouse.MouseX() > 337 && m_mouse.MouseX() < 349){//rating 4
      m_minRatingLevel = 4;
      m_tileLevel.SetCurrentRating(m_minRatingLevel);
    }
    else if(m_mouse.MouseX() > 349 && m_mouse.MouseX() < 363){//rating 5
      m_minRatingLevel = 5;
      m_tileLevel.SetCurrentRating(m_minRatingLevel);
    }
    else if(m_mouse.MouseX() > 365 && m_mouse.MouseX() < 411){//map
      data.m_bShowMap = !data.m_bShowMap;
    }
    else if(m_mouse.MouseX() > 415 && m_mouse.MouseX() < 443){//toggle brush size
      if(data.m_brushSize == 1)
        data.m_brushSize = 2;
      else
        data.m_brushSize = 1;     
    }
    else if(m_mouse.MouseX() > 446 && m_mouse.MouseX() < 474){//left isometric
      m_tileLevel.SetViewMode(0);
    }
    else if(m_mouse.MouseX() > 474 && m_mouse.MouseX() < 504){//right isometric
      m_tileLevel.SetViewMode(1);
    }
    else if(m_mouse.MouseX() > 504 && m_mouse.MouseX() < 535){//decrease group
      m_currentGroupID--;
      if(m_currentGroupID < 0)
        m_currentGroupID = m_tileLevel.GetTileDefSize() - 1;

      m_tileLevel.SetGroupNumber(m_currentGroupID);
      m_iconTileSelectIndex = 0;
    }
    else if(m_mouse.MouseX() > 535 && m_mouse.MouseX() < 563){//increase group
      m_currentGroupID++;
      if(m_currentGroupID > m_tileLevel.GetTileDefSize() - 1)
        m_currentGroupID = 0;

      m_tileLevel.SetGroupNumber(m_currentGroupID);
      m_iconTileSelectIndex = 0;
    }

  }

  //determine selected icon tile
  //con.RenderSprite(gi, m_iconSelectPosition + 32 + (i * 70) + 34 - (gi.scale * gi.width/2), 696 + 34 - (gi.scale * gi.height/2), 255, 255, 255);//black square
  //int m_iconTileSelectIndex;//position of left most icon displayed in a frame
  //int m_iconSelectPosition; //controls scrolling of icon selection
  //int m_iconTotalTiles;     //size of tiles



  //remove tile
  //*******************************************************
  if(m_addTarget == 0 && data.m_bShowMap == false && data.m_bRightMouseDown == true && m_mouse.MouseY() > 56 && m_mouse.MouseY() < 690){
    m_tileLevel.RemoveTile();
  }
  else if(m_addTarget == 0 && data.m_bShowMap == true && data.m_bRightMouseDown == true && m_mouse.MouseY() > 56 && m_mouse.MouseY() < 690){
    m_tileLevel.RemoveTileViaMap(m_mouse.MouseX(), m_mouse.MouseY(), data.m_brushSize);
  }


  //********************* do if not paused
  if(data.m_bPaused == false && data.m_bGameOver == false){
  }


  //update all objects
  //************************
  /*
  if(data.m_bPaused == false){// && data.m_bGameOver == false){
    for(unsigned int i = 0; i < data.m_pObject.size(); ++i){
      if(data.m_pObject.empty() == false){
        data.m_pObject[i]->Update(timeDifference, data);
      }
    }
  }
  */

  
  //pyro updating
  //*************************
  //if(data.m_bPaused == false)  //allow for special effects if HQ is destroyed
  //  m_effect.Update(m_startTime);

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

int CGameStatePlay1::IsLost(CGraphics &con, CConfigData &cfg)
{
	HRESULT hr 				= 0;
	static bool bLostDevice = false;
	CLog *pLog 				= CLog::Instance();
	
	if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel()))
	{
		if(hr == D3DERR_DEVICELOST)
		{
			if(bLostDevice == false)
			{
				pLog->Log("Lost: IDirect3DDevice9. cleaning up.");

				con.DeleteLost();

				bLostDevice = true;
			}

			return 1;
    	}
    	else if(hr == D3DERR_DEVICENOTRESET)
		{
			if(bLostDevice == false)
	  		{
				pLog->Log("Reset: IDirect3DDevice9. cleaning up.");

				con.DeleteLost();

				bLostDevice = true;
			}
		
			hr = con.m_d3ddev->Reset(&con.GetDPP());      
		
			if(SUCCEEDED(hr))
			{
				pLog->Log("Reloading: IDirect3DDevice9::Reset() successful.");

				con.LoadAssetFile(cfg.GamePlayAssetFile);
				con.LoadAssetFile(cfg.FrameworkAssetFile);     
				con.ReloadLost();

				bLostDevice = false;
			}
			else if(FAILED(hr))
			{
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
  
  //if(m_mouse.MouseY() < 33 + data.m_windowedYOffset)
  //  ::ShowCursor(true);
  //else
  //  ::ShowCursor(false);

  //update sounds
//  CAudioManager *pAudio = CAudioManager::Instance();
//  pAudio->Update();

  // clear the window to a deep blue
  //con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(data.m_screenColorRed, data.m_screenColorGreen, data.m_screenColorBlue), 1.0f, 0);//0, 111, 255
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);//0, 111, 255
  con.m_d3ddev->BeginScene();  
  con.BeginRender();

  GRAPHIC_IMAGE gi;

  m_tileLevel.Render(con, data);//render all tiles

  m_objectManager.Render(con, data, cfg);//render all things

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

  //file toolbar UI
  //***********************************************************
  gi = con.GetSprite(2010);
  con.RenderSprite(gi, 0, 0, 255, 255, 255);
  if(m_fileLevel > 1){//+
    gi = con.GetSprite(2014);
    con.RenderSprite(gi, 221, 3, 255, 255, 255);
  }
  if(m_fileLevel < 22){//-
    gi = con.GetSprite(2015);
    con.RenderSprite(gi, 221, 17, 255, 255, 255);
  }
  gi = con.GetSprite(2011);//open
  con.RenderSprite(gi, 3, 3, 255, 255, 255);
  gi = con.GetSprite(2012);//save
  con.RenderSprite(gi, 56, 3, 255, 255, 255);
  gi = con.GetSprite(2013);//clear
  con.RenderSprite(gi, 235, 3, 255, 255, 255);
  //***********************************************************


  //tile selection interface
  //***********************************************************
  gi = con.GetSprite(2001);
  con.RenderSprite(gi, 30, 694, 255, 255, 255);//gray background
  
  //render black squares  
  //for(int i = 0; i < m_tileLevel.GetTileDefSize();++i){   
  for(int i = 0; i < m_tileLevel.GetGroupSize();++i){   
    gi = con.GetSprite(2002);  
    con.RenderSprite(gi, m_iconSelectPosition + 32 + (i * 70), 696, 255, 255, 255);//black square
    
    gi = con.GetSprite(m_tileLevel.GetGroupTileSprite(i));  
    gi.scale = 0.33f;
    con.RenderSprite(gi, m_iconSelectPosition + 32 + (i * 70) + 34 - (gi.scale * gi.width/2), 696 + 34 - (gi.scale * gi.height/2), 255, 255, 255);//black square
  } 
  
  //highlighted square
  gi = con.GetSprite(2003);  
  con.RenderSprite(gi, m_iconSelectPosition + 32 + (m_iconTileSelectIndex * 70), 695, 255, 255, 255);//black square

  gi = con.GetSprite(2000);
  con.RenderSprite(gi, 0, 690, 255, 255, 255); //border overlay
  //************************************************************

  //show map
  if(data.m_bShowMap == true){
    m_tileLevel.RenderMap(con, cfg);
  }


  con.EndRender();  
  
  //pyro rendering
  /*
        dy = cos(data.m_pObject[i]->m_go.m_angle) * data.m_pObject[i]->m_go.m_weaponRadius;
        dx = sin(data.m_pObject[i]->m_go.m_angle) * data.m_pObject[i]->m_go.m_weaponRadius;
          smokeRadius = data.m_pObject[i]->m_go.m_weaponRadius * 0.001;
          fireRadius = data.m_pObject[i]->m_go.m_weaponRadius * 0.005;
          m_effectNumber = data.m_pObject[i]->m_go.m_weaponSmoke; 
          m_effect.Add(m_effectNumber, static_cast<float>(data.m_pObject[i]->m_go.data.m_posX) + dx,static_cast<float>(data.m_pObject[i]->m_go.data.m_posY) - dy, smokeRadius);
          m_effectNumber = data.m_pObject[i]->m_go.m_weaponEffect;
          m_effect.Add(m_effectNumber, static_cast<float>(data.m_pObject[i]->m_go.data.m_posX) + dx,static_cast<float>(data.m_pObject[i]->m_go.data.m_posY) - dy, fireRadius);
  */

  //renders UI panel outlines
//  m_UI.RenderOutlines(con, data);


  m_tileLevel.DrawGrid(con);

  //draw mouse horizontal lines
  //con.DrawLine(m_mouse.MouseX(), 0, m_mouse.MouseX(), 768, 0, 255, 255);
  //con.DrawLine(0, m_mouse.MouseY(), 1024, m_mouse.MouseY(), 0, 255, 255);

  //draw mouse diagonal lines
  //con.DrawLine(m_mouse.MouseX(), m_mouse.MouseY(), 0, m_yIntercept1, 255, 255, 0);  
  //con.DrawLine(m_mouse.MouseX(), m_mouse.MouseY(), 0, m_yIntercept2, 255, 255, 255);
  //con.DrawRect(m_tileX, m_tileY, m_tileX + 86.6, m_tileY + 50, 255, 255, 0);

  //display filename
  con.Print(m_filenames[m_fileLevel],F_V16B, 114, 8, 255, 255, 0, 255);

  //print row col
  con.Print("Row", m_tileLevel.GetIsoRow(), F_V16B, 830, 8, 255, 255, 0, 255);
  con.Print("Col", m_tileLevel.GetIsoCol(), F_V16B, 900, 8, 255, 255, 0, 255);
  con.Print("Group No.", m_currentGroupID, F_V16B, 569, 8, 255, 255, 255, 255);

  //draw rect around rating number in top file panel
  switch(m_minRatingLevel){
  case 1:
    con.DrawRect(299, 5, 313, 27, 255, 255, 255);
    break;
  case 2:
    con.DrawRect(313, 5, 325, 27, 255, 255, 255);
    break;
  case 3:
    con.DrawRect(325, 5, 337, 27, 255, 255, 255);
    break;
  case 4:
    con.DrawRect(337, 5, 349, 27, 255, 255, 255);
    break;
  case 5:
    con.DrawRect(349, 5, 363, 27, 255, 255, 255);
    break;
  }


  //display tile rating on bottom tile selection
  for(int i = 0; i < m_tileLevel.GetGroupSize();++i){        
    con.Print(m_tileLevel.GetGroupTileRating(i), F_V20, m_iconSelectPosition + 32 + (i * 70) + 25, 713, 255, 255, 0, 255);
    con.Print(m_tileLevel.GetGroupTileYOffset(i), F_V20, m_iconSelectPosition + 32 + (i * 70) + 25, 733, 0, 255, 255, 255);
  } 
  //for(int i = 0; i < m_tileLevel.GetTileDefSize();++i){        
  //  con.Print(m_tileLevel.GetTileRating(i), F_V20, m_iconSelectPosition + 32 + (i * 70) + 25, 713, 255, 255, 0, 255);
  //  con.Print(m_tileLevel.GetTileYOffset(i), F_V20, m_iconSelectPosition + 32 + (i * 70) + 25, 733, 0, 255, 255, 255);
  //} 

  //con.Print("Play State", F_V16, 450, 720, 255, 255, 255, 255);
  con.Print("Press ESC to Return to Main Menu", F_V16, 450, 672, 255, 255, 255, 255);
  
  //POINT pt;  
  //if(::GetCursorPos(&pt) == true){  
  //  con.Print("PX", pt.x, F_V16, 450, 684, 255, 255, 255, 255);
  //  con.Print("PY", pt.y, F_V16, 550, 684, 255, 255, 255, 255);
 // }
  
  if(cfg.DisplayDebugInfo == true){
    con.Print("FPS", data.m_FPS, F_V20, 7, 37, 255, 255, 255, 255);
    con.Print("Mouse X", m_mouse.MouseX(), F_V20, 860, 37, 255, 255, 255, 255);
    con.Print("Mouse Y", m_mouse.MouseY(), F_V20, 860, 59, 255, 255, 255, 255);
    con.Print("IsoRow", m_tileLevel.CalcIsoRow(data.m_posX - m_mouse.MouseX(),data.m_posY - m_mouse.MouseY(), m_tileLevel.GetViewMode()), F_V20, 860, 81, 255, 255, 255, 255);
    con.Print("IsoCol", m_tileLevel.CalcIsoCol(data.m_posX - m_mouse.MouseX(),data.m_posY - m_mouse.MouseY(),  m_tileLevel.GetViewMode()), F_V20, 860, 103, 255, 255, 255, 255);
    con.Print("Cursor X", (int)data.m_posX, F_V20, 860, 125, 255, 255, 255, 255);
    con.Print("Cursor Y", (int)data.m_posY, F_V20, 860, 147, 255, 255, 255, 255);
    //con.Print("IsoX", m_tileLevel.GetIsoX( .CalcIsoRow(data.m_posX - m_mouse.MouseX(),data.m_posY - m_mouse.MouseY(), m_tileLevel.GetViewMode()), F_V20, 860, 81, 255, 255, 255, 255);
    //con.Print("IsoY", m_tileLevel.CalcIsoCol(data.m_posX - m_mouse.MouseX(),data.m_posY - m_mouse.MouseY(),  m_tileLevel.GetViewMode()), F_V20, 860, 103, 255, 255, 255, 255);

  }

  con.m_d3ddev->EndScene();    // ends the 3D scene
  con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

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


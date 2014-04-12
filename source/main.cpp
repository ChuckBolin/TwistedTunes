/**************************************************************************************
Filename: main.cpp 
Date: December 17, 2008 (original)
Purpose: Entry point and initial setup for standard 2D game
Updated: January 18, 2009

**************************************************************************************/

// include the basic windows header files and the Direct3D header file
#define _WIN32_WINNT 0x0500 //needed for TrackMouseEvent
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <string>
#include "..\include\CLog.h"
#include "..\include\CGraphics.h"
#include "..\include\CTimer.h"
#include "..\include\CGameData.h"
#include "..\include\Keystatus.h"
#include "..\include\CGameStateObject.h"
#include "..\include\CGameStateControl.h"
#include "..\include\CGameStateIntro.h"
#include "..\include\CGameStateMain.h"
#include "..\include\CGameStateHelp.h"
#include "..\include\CGameStateCredits.h"
#include "..\include\CGameStateQuit.h"
#include "..\include\CGameStateScore.h"
#include "..\include\CGameStatePlay1.h"
#include "..\include\CConfigData.h"
#include "..\resource.h"
//#include "..\include\CAudioManager.h"

// global declarations
LPDIRECT3D9 d3d; // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev; // the pointer to the device class

// function prototypes
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Shutdown();

//create game state pointers
CGameStateObject* g_pStateControl = new CGameStateControl;
CGameStateObject* g_pStateMain = new CGameStateMain;
CGameStateObject* g_pStateIntro = new CGameStateIntro;
CGameStateObject* g_pStateCredits = new CGameStateCredits;
CGameStateObject* g_pStateHelp = new CGameStateHelp;
CGameStateObject* g_pStatePlay1 = new CGameStatePlay1;
CGameStateObject* g_pCurrent = 0;
CGameStateObject* g_pNext = 0;
CGameStateObject* g_pLast = 0;
CGameStateObject* g_pStateQuit = new CGameStateQuit;

CMouse mouse;
CTimer gTimerKey;
CTimer gAudioTimer;

/***********************************************************************************
WinMain - Entry point for program.
***********************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    
    CLog *pLog = CLog::Instance();
    pLog->Log(" ");
    pLog->Log("***************************************");
    pLog->Log("Program Start"); 
    pLog->LogDate();   
    pLog->Log("***************************************");
    
    

    //request player choose fullscreen or windowed mode
    CConfigData cfg;
    cfg.LoadConfigFile("assets//data//config.cfg");
    bool bFullscreen = false;
    
    bFullscreen = cfg.FullScreen;
    int msgReturn = ::MessageBox(NULL, "Fullscreen? (Y/N)", "Select Display Option", MB_YESNO);
    if(msgReturn == IDYES)
      bFullscreen = true;

    //variable declarations
    CGameData gameData;
    CTimer gTimerFPS;
    int gLoopCount = 0;
    int gSecondCount = 0;
    bool g_bRunning = true;
    bool gExitProgram = false; //this is set true with ESC, for rendering to stop properly
    //if(bFullscreen == true)
    //  gameData.m_windowedYOffset = 0;
    //else
    //  gameData.m_windowedYOffset = 21;

    //determine if we play new or saved game
    /*
    gameData.m_playNewGame = true;
    HRESULT hr = ::MessageBox(0, "Play new game?", "New or saved game!", MB_YESNO);
    if(hr == IDYES)
      gameData.m_playNewGame = true;
    else
      gameData.m_playNewGame = false;
    */
    //gameData.m_playNewGame = true;

    //setup game data    

    pLog->Log("Program Name", cfg.ProgramName);
    pLog->Log("Version", cfg.ProgramVersion);

    //create window
    HWND hWnd;
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "WindowClass";
    wc.hIconSm = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1)); 
    RegisterClassEx(&wc);
        
    //screen data - need at least 800x600 

    if(!bFullscreen){
		  if(cfg.ScreenWidth < 800){
  			cfg.ScreenWidth = 800;   
	  	}
    	if(cfg.ScreenHeight < 600){
		  	cfg.ScreenHeight = 600;    
		  }
	  }
	  cfg.ScreenLeft = 0;
    cfg.ScreenTop = 0;

    //create window
    std::string sCaption = cfg.ProgramName + " - " + cfg.ProgramVersion;
    hWnd = CreateWindowEx(NULL,
                          "WindowClass",
                          sCaption.c_str(), //cfg.ProgramVersion.c_str(),  
                          bFullscreen == true ? WS_EX_TOPMOST | WS_POPUP : WS_BORDER | WS_CAPTION | WS_SYSMENU,  
                          cfg.ScreenLeft, cfg.ScreenTop, 
                          bFullscreen == true ? CW_USEDEFAULT : cfg.ScreenWidth, 
                          bFullscreen == true ? CW_USEDEFAULT : cfg.ScreenHeight,
                          NULL,NULL,hInstance,NULL);
    ShowWindow(hWnd, nCmdShow);
    pLog->Log("Window Loaded and Displayed!");
    gameData.m_hWnd = hWnd;

    // set up and initialize Direct3D
	  CGraphics con(hWnd, cfg.ScreenWidth, cfg.ScreenHeight, cfg.RefreshRate, (D3DFORMAT)cfg.Format, cfg.Adapter, bFullscreen);

    if(con.InitializeDirectX() == false){
        ::MessageBox(hWnd, "Failed to Create IDirect3D9 Interface.", "Error", 0);
        return 0;
    }
	  if(con.IsDisplayModeSupported() == false){
	  //	::MessageBox(hWnd, "Display mode not supported.", "Error", 0);
    //    	return 0;
	  }
	  if(con.InitializeDevice() == false){
        ::MessageBox(hWnd, "Could not create IDirect3DDevice9 Device.", "Error", 0);
        return 0;
    }

    //load framework assets
    if(con.LoadAssetFile(cfg.FrameworkAssetFile) == false){
      pLog->Log("Failure loading " + cfg.FrameworkAssetFile);
      ::MessageBox(hWnd,"Failed to load editor.dat file", "Error", 0);
      return 0;
    }
    else
      pLog->Log(cfg.FrameworkAssetFile + " (frame graphics) was loaded successfully!");

    //load game play assets
    if(con.LoadAssetFile(cfg.GamePlayAssetFile) == false){
      pLog->Log("Failure loading " + cfg.GamePlayAssetFile);
      ::MessageBox(hWnd,"Failed to load assets.dat file", "Error", 0);
      return 0;
    }
    else
      pLog->Log(cfg.GamePlayAssetFile + " (game play graphics) was loaded successfully!");

    /*
    //load objects
    //***************************************************************************
    if(gameData.LoadObjectFile(cfg.GameObjectFile) == false){
      pLog->Log("Failure loading " + cfg.GameObjectFile);
    }
    else{
      pLog->Log(cfg.GameObjectFile + " (objects file) was loaded successfully!");
    }
    */

    gTimerKey.Initialize();
    gTimerKey.ResetTimer();
    mouse.SetHandle(hWnd);
    gTimerFPS.Initialize();
    
    //game timer for update
    CTimer timerGame;
    timerGame.Initialize();

    //define events for changing game states
    //*************************************************************************
    //g_pStateIntro->AddTransitionEvent(EVENT_GO_PLAY1, g_pStatePlay1);
    //g_pStatePlay1->AddTransitionEvent(EVENT_GO_CREDITS, g_pStateCredits);   
    g_pStateIntro->AddTransitionEvent(EVENT_GO_MAIN, g_pStateMain);
    g_pStateMain->AddTransitionEvent(EVENT_GO_PLAY1, g_pStatePlay1);
    //g_pStateMain->AddTransitionEvent(EVENT_GO_HELP, g_pStateHelp);
    //g_pStateMain->AddTransitionEvent(EVENT_GO_CREDITS, g_pStateCredits);
    g_pStatePlay1->AddTransitionEvent(EVENT_GO_CREDITS, g_pStateCredits);
    //g_pStatePlay1->AddTransitionEvent(EVENT_GO_QUIT, g_pStateQuit);
    //g_pStatePlay1->AddTransitionEvent(EVENT_GO_MAIN, g_pStateMain);

    g_pStateHelp->AddTransitionEvent(EVENT_GO_MAIN, g_pStateMain);
    g_pStateCredits->AddTransitionEvent(EVENT_GO_QUIT, g_pStateQuit);    
    g_pCurrent =  g_pStateIntro;//g_pStatePlay1;

    //************************************** S O U N D ************************
    //initialize sound manager
    //audio setup
    CAudioManager *pAudio = CAudioManager::Instance();

  /*  
    if(pAudio->LoadAudioData("assets\\data\\sounds.dat") == true)
      pLog->Log("Audio files have loaded successfully!");
    else
      pLog->Log("Audio files have failed to load!");
*/
    pAudio->SetPlayerPosition(1000, 0, 1000); 
    pAudio->SetSystemFactors(1, 1, 1);
    /*
    pAudio->AddNoiseSource(0, 0, 10, 0, 41, 0, 32, -1, 0.25);
    pAudio->AddNoiseSource(1, 1, 14, 0, 35, 0, 32, -1, 0.25);
    pAudio->AddNoiseSource(2, 2, 10, 0, 30, 0, 32, -1, 0.25);
    pAudio->AddNoiseSource(3, 3, 20, 0, 15, 0, 32, -1, 0.25);
    pAudio->AddNoiseSource(4, 4, 29, 0, 15, 0, 32, -1, 0.25);
    pAudio->AddNoiseSource(5, 5, 30, 0, 5, 0, 32, -1, 0.25);
    pAudio->AddNoiseSource(6, 6, 40, 0, 10, 0, 32, -1, 0.25);  
    pAudio->AddNoiseSource(7, 7, 35, 0, 35, 0, 32, -1, 0.25);  
    pAudio->AddNoiseSource(8, 8, 27, 0, 27, 0, 32, -1, 0.25);
    pAudio->AddNoiseSource(9, 9, 26, 0, 26, 0, 32, -1, 0.25);
    pAudio->AddNoiseSource(10, 10, 26, 0, 26, 0, 250, -1, 10.0);
    pAudio->AddNoiseSource(11, 11, 26, 0, 26, 0, 250, -1, 0.25);
    pAudio->AddNoiseSource(12, 12, 45, 0, 45, 0, 500, -1, 0.25);
*/

    // enter the main loop
    //************************************** M A I N  L O O P *****************
    MSG msg;
    pLog->Log("Entering Main Control Loop");
    float timeDiff = 0.0f;
    g_pCurrent->Activate(gameData, cfg, con);

	int iLost = 0;
    //*********************
    // PYRO
    //con.InitializePyro();
    while(g_bRunning)
    {
      DWORD starting_point = GetTickCount();
      ::Sleep(1);
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
          if (msg.message == WM_QUIT)
              break;

          TranslateMessage(&msg);
          DispatchMessage(&msg);
      }

      //manage frame count determination
      gLoopCount++;
      if(gTimerFPS.getTimer(1.0) == true){
        gameData.m_FPS = static_cast<float>(gLoopCount);
        gLoopCount = 0;   
        gSecondCount++;
        if(gSecondCount > 30){ //log every 30 seconds
          gSecondCount = 0;
          if(cfg.LogDebugInfo == true)
            pLog->Log("FPS",gameData.m_FPS);
        }
      }

      //stores mouse button status for use in other classes
      gameData.m_bLeftMouseDown =  mouse.LeftButtonDown();
      gameData.m_bRightMouseDown = mouse.RightButtonDown();
      
      //update
      g_pLast = g_pCurrent;
      g_pNext = g_pCurrent->Update(timerGame.getTimeDifference(), gameData, cfg, con);

      if(g_pNext == g_pStateQuit)
        g_bRunning = false;
      else if(NULL != g_pNext)
	    {
        if(g_pNext != g_pLast){
          g_pLast->Deactivate(gameData, cfg, con);
          g_pCurrent = g_pNext;
          g_pCurrent->Activate(gameData, cfg, con);
        }
      }  
      
      iLost = g_pCurrent->IsLost(con, cfg);

      if(iLost == 0){
	      g_pCurrent->Render(con, gameData, cfg);
      }
      else if(iLost == 2){
	      g_bRunning = false;
      }

      // check the 'escape' key
      if(g_bRunning == false){
        gExitProgram = true;
        PostMessage(hWnd, WM_DESTROY, 0, 0);
      }
    }
    pLog->Log("Exited main loop");
    
    //**************************
    // PYRO
    //con.ClosePyro();
    
    // clean up DirectX and COM
    con.CleanupDirectX();
    Shutdown();

    pLog->Log("DirectX Cleaned Up");
    pLog->Log("Shutdown complete!");
    pLog->Log("***************************************");
    pLog->Log(" Program Terminated Normally");
    pLog->Log("***************************************");

    return static_cast<int>(msg.wParam);
}

/***********************************************************************************
  WindowProc - Main message handler
***********************************************************************************/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static TRACKMOUSEEVENT tme;

  switch(message)
  {
  case WM_CREATE:
    g_pStateIntro->Initialize();
    g_pStateMain->Initialize();
    g_pStateControl->Initialize();
    g_pStateCredits->Initialize();
    g_pStatePlay1->Initialize();

    //#define _WIN32_WINNT 0x0500 added before <windows.h> in order to get this to work
    tme.cbSize = sizeof(tme);
    tme.dwFlags = 0x00000002;//TME_LEAVE;
    tme.hwndTrack = hWnd;
    TrackMouseEvent(&tme);
    return 0;

  case WM_MOUSEMOVE:
	  TrackMouseEvent(&tme);
	  return 0;

  case WM_LBUTTONDBLCLK:
    return 0;

  case WM_LBUTTONDOWN:
    mouse.SetLeftButtonDown(true);
    return 0;

  case WM_LBUTTONUP:
    mouse.SetLeftButtonDown(false);
    return 0;

  case WM_RBUTTONDOWN:
    mouse.SetRightButtonDown(true);
    return 0;

  case WM_RBUTTONUP:
    mouse.SetRightButtonDown(false);
    return 0;

  case WM_MOUSELEAVE:
    mouse.SetLeftButtonDown(false);
    mouse.SetRightButtonDown(false);
    return 0;

  case WM_DESTROY:
      {
          PostQuitMessage(0);
          return 0;
      } break;
  }

  return DefWindowProc (hWnd, message, wParam, lParam);
}

//Shutdown everything NOT DirectX
void Shutdown(){
  delete g_pStateControl;
  delete g_pStateMain;
  delete g_pStateIntro;
  delete g_pStateCredits;
  delete g_pStateHelp;
  delete g_pStateQuit;
  delete g_pStatePlay1;
}
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
    if(bFullscreen == true)
      gameData.m_windowedYOffset = 0;
    else
      gameData.m_windowedYOffset = 21;

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
      
    /*
    //screen data - need at least 800x600 
    int cxScreen = ::GetSystemMetrics(SM_CXFULLSCREEN);
    int cyScreen = ::GetSystemMetrics(SM_CYFULLSCREEN);

    if(cfg.ScreenWidth < 800 || cfg.ScreenWidth > cxScreen)
      cfg.ScreenWidth = cxScreen;   
    if(cfg.ScreenHeight < 600 || cfg.ScreenHeight > cyScreen)
      cfg.ScreenHeight = cyScreen;    
    cfg.ScreenLeft = cxScreen/2 - cfg.ScreenWidth/2;
    cfg.ScreenTop = cyScreen/2 - cfg.ScreenHeight/2;
    */

    if(!bFullscreen)
       {
          if(cfg.ScreenWidth < 800)
          {
             cfg.ScreenWidth = 800; 
          }
                   if(cfg.ScreenHeight < 600)
          {
             cfg.ScreenHeight = 600;   
          }
       }
    cfg.ScreenLeft = 0;
    cfg.ScreenTop = 0;

/*
    // set up and initialize Direct3D
    CGraphics con();// cfg.FullScreen);
    int screenWidth = 0;
    int screenHeight = 0;
    if(con.InitializeDirectX(screenWidth, screenHeight, bFullscreen) == false){
      pLog->Log("Failure initializing DirectX!");
      ::MessageBox(0,"Failed to initialize DirectX", "Error", 0);
      return 0;
    }
    pLog->Log("DirectX Initialized");
*/

    int cxScreen = ::GetSystemMetrics(SM_CXFULLSCREEN);
    int cyScreen = ::GetSystemMetrics(SM_CYFULLSCREEN);
    if(cfg.ScreenWidth < 800 || cfg.ScreenWidth > cxScreen)
      cfg.ScreenWidth = cxScreen;   
    if(cfg.ScreenHeight < 600 || cfg.ScreenHeight > cyScreen)
      cfg.ScreenHeight = cyScreen;    
    cfg.ScreenLeft = cxScreen/2 - cfg.ScreenWidth/2;
    cfg.ScreenTop = cyScreen/2 - cfg.ScreenHeight/2;

    //create window
    std::string sCaption = cfg.ProgramName + " - " + cfg.ProgramVersion;
    hWnd = CreateWindowEx(NULL,
                          "WindowClass",
                          sCaption.c_str(), //cfg.ProgramVersion.c_str(),  
                          bFullscreen == true ? WS_EX_TOPMOST | WS_POPUP : WS_BORDER | WS_CAPTION | WS_SYSMENU,  
                          cfg.ScreenLeft, cfg.ScreenTop, 
                          /*0, 0, screenWidth, screenHeight,*/
                          
                          bFullscreen == true ? CW_USEDEFAULT : cfg.ScreenWidth, 
                          bFullscreen == true ? CW_USEDEFAULT : cfg.ScreenHeight,
                          
                          NULL,NULL,hInstance,NULL);
    ShowWindow(hWnd, nCmdShow);
    pLog->Log("Window Loaded and Displayed!");
    gameData.m_hWnd = hWnd;
/*
    // set up and initialize Direct3D
    CGraphics con(hWnd, cfg.ScreenWidth, cfg.ScreenHeight,bFullscreen);// cfg.FullScreen);
    if(con.InitializeDirectX() == false){
      pLog->Log("Failure initializing DirectX!");
      ::MessageBox(hWnd,"Failed to initialize DirectX", "Error", 0);
      return 0;
    }
    pLog->Log("DirectX Initialized");
 */
    CGraphics con(hWnd, cfg.ScreenWidth, cfg.ScreenHeight, bFullscreen);

    if(con.InitializeDirectX() == false)
    {
        ::MessageBox(hWnd, "Failed to Create IDirect3D9 Interface.", "Error", 0);
        return 0;
    }
    if(con.IsDisplayModeSupported() == false)
    {
        ::MessageBox(hWnd, "Display mode not supported.", "Error", 0);
        return 0;
    }
    if(con.InitializeDevice() == false)
    {
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

     //load objects
    //***************************************************************************
    if(gameData.LoadObjectFile(cfg.GameObjectFile) == false){
      pLog->Log("Failure loading " + cfg.GameObjectFile);
      //::MessageBox(hWnd,"Failed to load objects.dat file", "Error", 0);
     // return 0;
    }
    else{
      pLog->Log(cfg.GameObjectFile + " (objects file) was loaded successfully!");
     // for(int i = 0; i < gameData.m_catalog.GetTableSize();++i){
//        pLog->Log("object", gameData.m_catalog.GetTerm(i, 0), gameData.m_catalog.GetTerm(i, 2));
     // }
    }

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
    g_pStateMain->AddTransitionEvent(EVENT_GO_HELP, g_pStateHelp);
    g_pStateMain->AddTransitionEvent(EVENT_GO_CREDITS, g_pStateCredits);
    g_pStatePlay1->AddTransitionEvent(EVENT_GO_QUIT, g_pStateQuit);
    //g_pStatePlay1->AddTransitionEvent(EVENT_GO_MAIN, g_pStateMain);

    g_pStateHelp->AddTransitionEvent(EVENT_GO_MAIN, g_pStateMain);
    g_pStateCredits->AddTransitionEvent(EVENT_GO_QUIT, g_pStateQuit);    
    g_pCurrent = g_pStatePlay1;// g_pStateIntro;


    // enter the main loop
    //************************************** M A I N  L O O P *****************
    MSG msg;
    pLog->Log("Entering Main Control Loop");
    float timeDiff = 0.0f;
    g_pCurrent->Activate(gameData, cfg, con);


    while(g_bRunning)
    {
      DWORD starting_point = GetTickCount();
      ::Sleep(0);
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
      
      //render
      g_pCurrent->Render(con, gameData, cfg);

      // check the 'escape' key
      if(g_bRunning == false){
        gExitProgram = true;
        PostMessage(hWnd, WM_DESTROY, 0, 0);
      }
    }
    pLog->Log("Exited main loop");
    

    
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
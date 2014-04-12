/**************************************************************************************
Filename: CGraphics.h Date: June 1, 2008
Purpose: Manages all DirectX
Initial code: http://www.directxtutorial.com/Tutorial9/B-Direct3DBasics/dx9B2.aspx

**************************************************************************************/

#ifndef CGRAPHICS_H
#define CGRAPHICS_H
#include <windows.h>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <sstream>
#include "CLog.h"
#include <sstream>
#include "CFileReader.h"
#include <iomanip>
#include "CGameData.h"
#include "CCSVReader.h"
#include "CLog.h"
#include "Pyro.h"
#include "PyroD3D9.h"
#include "EmitterList.h"

const int F_A12 = 0;  //Arial
const int F_A12B = 1;
const int F_A14 = 2;
const int F_A14B = 3;
const int F_A16 = 4;
const int F_A16B = 5;
const int F_V12 = 6;  //Verdana
const int F_V12B = 7;
const int F_V14 = 8;
const int F_V14B = 9;
const int F_V16 = 10;
const int F_V16B = 11;
const int F_V20 = 12;
const int F_V20B = 13;
const int C_NUMBER_FONTS = 16;

struct GAME_OBJECT;

//used with all graphic images
struct GRAPHIC_IMAGE{
  int spriteID;
  int fileID;
  int sourceX;
  int sourceY;
  int width;
  int height;
  float scale;
  float angle;
  int alpha;
  int frameMax;
  int frameCount;
  double updateInterval;
  std::string description;  
  bool bValid;
  float screenScale;
  double animTime;
};

//*******************************************
//structures store textures for rendering
//*******************************************
struct S_TEXTURE{
  int id;
  IDirect3DTexture9* pBuffer;
};

//********************************************
//structures for various asset data
//this is loaded from file into vectors
struct GRAPHIC_FILE_DATA{
  int fileID;
  std::string filename;
  std::string description;
};

struct SPRITE_DATA{
  int spriteID;
  int fileID;
  float x, y;
  float width, height;
  float scale;
  float angle;
  int alpha;
  int numberFrames;
  int frameCount;
  float timeBetweenFrames;
  std::string description;
};

//***************************************************  
struct CUSTOMVERTEX {FLOAT X, Y, Z, RHW; DWORD COLOR;};
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class CGraphics{
public:
  CGraphics(HWND hWnd, int screenWidth, int screenHeight, bool fullScreen);
  bool InitializeDirectX();
  void CleanupDirectX();
  LPDIRECT3DDEVICE9 m_d3ddev; // the pointer to the device class
  D3DFORMAT FindDepthStencilFormat( ULONG AdapterOrdinal, D3DDISPLAYMODE Mode, D3DDEVTYPE DevType );
  template <typename T1> void Print(T1 value, int fontType, int posX, int posY, int red, int green, int blue, int alpha = 255);
  template <typename T1, typename T2> void Print(T1 value, T2 value2, int fontType, int posX, int posY, int red, int green, int blue, int alpha = 255);
  int GetScreenWidth(){return m_screenWidth;};
  int GetScreenHeight(){return m_screenHeight;};
  int GetScreenCenterX(){return m_screenWidth/2;};
  int GetScreenCenterY(){return m_screenHeight/2;};
  HWND GetWindowHandle(){return m_hWnd;};
  D3DPRESENT_PARAMETERS GetDPP(){return m_d3dpp;};
  void DrawLine(float x1, float y1,float x2, float y2, int red, int green, int blue);
  void DrawRect(float x1, float y1,float x2, float y2, int red, int green, int blue);
  void DrawCircle(float x, float y, int red, int green, int blue, float radius);
  void SetupStuff();
  void ReloadLost();
  void DeleteLost();
  void BeginRender();
  void EndRender();

  //2D sprite rendering
  void RenderGraphicModulate(GRAPHIC_IMAGE gi, int positionX, int positionY, int red, int green, int blue);
  bool LoadAssetFile(std::string filename);
  bool LoadTextureFile(int fileID, std::string filename);
  IDirect3DTexture9* GetTexture(int id);

  std::vector<GRAPHIC_FILE_DATA> m_assetFileData;
  std::vector<SPRITE_DATA> m_assetSpriteData;
  std::vector<S_TEXTURE> m_texture;
  std::vector<GRAPHIC_IMAGE> m_sprites;
  ID3DXSprite*				  m_pSprite;
  
  GRAPHIC_IMAGE GetSprite(int id);

  //pyro support
  //**************************
  HRESULT ScreenGrab(char *fileName);

  //color of screen
  int m_textureRed;
  int m_textureGreen;
  int m_textureBlue;

private:

  void InitializeFonts();
  LPDIRECT3D9 m_d3d; // the pointer to our Direct3D interface
  HWND m_hWnd;
  int m_screenWidth;
  int m_screenHeight;
  bool m_fullScreen;

  ID3DXFont *m_Font[C_NUMBER_FONTS];
  D3DPRESENT_PARAMETERS m_d3dpp;

  //pyro support
  //**************************
  PyroParticles::IPyroParticleLibrary *g_pParticleLibrary;
  PyroParticles::PyroGraphics::IDevice *pPyroGraphics;
  PyroParticles::CEmitterList *g_pEmitters;
  const char *pEmitterName;
  PyroParticles::IPyroFile *pInitialExplosionFile;
  PyroParticles::IPyroFile *pSmokeFile;
  bool m_bParticlesEnabled;

	float Time;
	float Phase;
  DWORD	g_StartTime;



};

template <typename T1, typename T2> void CGraphics::Print(T1 value, T2 value2, int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;
  else if(fontType == 12 || fontType == 13)
    fontHeight = 20;
  
  std::ostringstream oss;
  oss << value << ": " << std::setprecision(4) << value2;//std::setprecision(4) << value;
  width = (int)(oss.str().size() * fontHeight);
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  std::string output = oss.str();
  m_Font[fontType]->DrawText(NULL, output.c_str(), -1, &rct, 0, fontColor);  

  m_Font[fontType]->DrawText(NULL, output.c_str(), -1, &rct, 0, fontColor);  
}

template <typename T1> void CGraphics::Print(T1 value, int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;
  else if(fontType == 12 || fontType == 13)
    fontHeight = 20;
  
  std::ostringstream oss;
  oss << std::setprecision(4) << value;
  width = (int)(oss.str().size() * fontHeight);
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  std::string output = oss.str();
  m_Font[fontType]->DrawText(NULL, output.c_str(), -1, &rct, 0, fontColor);  

}



#endif
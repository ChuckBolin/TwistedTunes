#include "../include/CGraphics.h"

/*******************************************************************
  CGraphics constructor
*******************************************************************/
CGraphics::CGraphics(HWND hWnd, int screenWidth, int screenHeight, bool fullScreen){
  m_hWnd = hWnd;
  m_screenWidth = screenWidth;
  m_screenHeight = screenHeight;
  m_fullScreen = fullScreen;

  m_textureRed = 105; //255;
  m_textureGreen = 60; //255;
  m_textureBlue = 40; //255;
}
/*
CGraphics::CGraphics(){
  //m_hWnd = hWnd;
  //m_screenWidth = screenWidth;
  //m_screenHeight = screenHeight;
  //m_fullScreen = fullScreen;

  m_textureRed = 105; //255;
  m_textureGreen = 60; //255;
  m_textureBlue = 40; //255;
}
*/

/*******************************************************************
  Initialize DirectX.
*******************************************************************/
bool CGraphics::InitializeDirectX()
{
   CLog *pLog = CLog::Instance();

   m_d3d = Direct3DCreate9(D3D_SDK_VERSION);

   if(m_d3d)
   {
      pLog->Log("Successful: Created IDirect3D9 Interface.");

      return true;
   }

   pLog->Log("Critical Error: Failed to Create IDirect3D9 Interface.");

   return false;
}

bool CGraphics::IsDisplayModeSupported()
{
   CLog *pLog              = CLog::Instance();
   int AdapterModeCount    = 0;
   D3DDISPLAYMODE Mode;

   if(!m_fullScreen)
   {
      pLog->Log("Warning: CGraphics::IsDisplayModeSupported() Should only be called for a full screen application.");
      return true;
   }

   if(m_d3d)
   {
      AdapterModeCount = m_d3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);

      for(int Loop = 0; Loop < AdapterModeCount; Loop++)
      {
         if(FAILED(m_d3d->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, Loop, &Mode)))
         {
            pLog->Log("Error: IDirect3D9::EnumAdapterModes() Failed.");
            return false;
         }

         if(m_screenWidth == Mode.Width & m_screenHeight == Mode.Height)
         {
            return true;
         }
      }

      pLog->Log("Warning: Display mode not supported.");
   }
   else
   {
      pLog->Log("Critical Error: IDirect3D9 Interface not initialised.");
   }

   return false;
}

bool CGraphics::InitializeDevice()
{
   CLog *pLog = CLog::Instance();
   D3DDISPLAYMODE CurrentMode;

   if(!m_d3d)
   {
      pLog->Log("Critical Error: IDirect3D9 Interface not initialised.");
      return false;
   }
   
   ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
   
   m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &CurrentMode);
   
   m_d3dpp.Windowed                     = !m_fullScreen;
   m_d3dpp.BackBufferFormat                 = D3DFMT_X8R8G8B8;
   m_d3dpp.BackBufferWidth                 = m_screenWidth;
   m_d3dpp.BackBufferHeight               = m_screenHeight;
   m_d3dpp.SwapEffect                  = D3DSWAPEFFECT_DISCARD;
   m_d3dpp.hDeviceWindow               = m_hWnd;
   m_d3dpp.PresentationInterval          = D3DPRESENT_INTERVAL_IMMEDIATE;
   m_d3dpp.FullScreen_RefreshRateInHz   = m_fullScreen ? CurrentMode.RefreshRate : 0;
   m_d3dpp.EnableAutoDepthStencil       = true;
   m_d3dpp.AutoDepthStencilFormat       = FindDepthStencilFormat(D3DADAPTER_DEFAULT, CurrentMode, D3DDEVTYPE_HAL);
   
   if(FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_d3ddev)))
   {
      pLog->Log("Critical Error: Could not create IDirect3DDevice9 Device.");
      return false;
   }
   pLog->Log("Successful: Created IDirect3DDevice9 Device.");

   ReloadLost();

   return true;
}
/*
bool CGraphics::InitializeDirectX(){

  m_d3d = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface
  if(!m_d3d){
    ::MessageBox(0 , "DirectX failed!", "Fatal Error!", MB_OK | MB_ICONSTOP | MB_APPLMODAL);
    return false;
  }

  //D3DPRESENT_PARAMETERS d3dpp; // create a struct to hold various device information
  ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));    // clear out the struct for use
	D3DDISPLAYMODE        CurrentMode;
  
  // Select back buffer format etc
	m_d3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &CurrentMode);

  //controlled by config.txt
  m_d3dpp.Windowed = !m_fullScreen;  //windowed
  m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
  
  //David Flyman 2.23.2010
  m_d3dpp.BackBufferWidth =  m_screenWidth;    // set the width of the buffer
  m_d3dpp.BackBufferHeight = m_screenHeight;    // set the height of the buffer
  
  m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
  m_d3dpp.hDeviceWindow = m_hWnd;    // set the window to be used by Direct3D
  m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
  m_d3dpp.FullScreen_RefreshRateInHz = m_fullScreen ? CurrentMode.RefreshRate : 0;
  m_d3dpp.EnableAutoDepthStencil = TRUE;
  m_d3dpp.AutoDepthStencilFormat		=  FindDepthStencilFormat( D3DADAPTER_DEFAULT, CurrentMode, D3DDEVTYPE_HAL );

  // create a device class using this information and the info from the d3dpp stuct
 if(FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,m_hWnd,
   D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_d3ddev))){
    return false;  //David Flyman 2.23.2010
 }
  ReloadLost();
  return true;
}
*/

/*******************************************************************
  Clean up DirectX
*******************************************************************/
void CGraphics::CleanupDirectX(){
  CLog *pLog = CLog::Instance();
  pLog->Log("Cleanup");

  DeleteLost();
  pLog->Log("cleanup 1");
  m_d3ddev->Release(); // close and release the 3D device
  m_d3d->Release(); // close and release Direct3D
  
  pLog->Log("cleanup 2");
    return;
}

/*******************************************************************
  Define all fonts
*******************************************************************/
void CGraphics::InitializeFonts(){

  //load 12 different fonts for Arial and Verdana, 12-16, normal and bold
  D3DXCreateFont(m_d3ddev, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A12] );
  D3DXCreateFont(m_d3ddev, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A12B] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A14] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A14B] );
  D3DXCreateFont(m_d3ddev, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A16] );
  D3DXCreateFont(m_d3ddev, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A16B] );
  D3DXCreateFont(m_d3ddev, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V12] );
  D3DXCreateFont(m_d3ddev, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V12B] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V14] );
  D3DXCreateFont(m_d3ddev, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V14B] );
  D3DXCreateFont(m_d3ddev, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V16] );    
  D3DXCreateFont(m_d3ddev, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V16B] );
  D3DXCreateFont(m_d3ddev, 20, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V20] );
  D3DXCreateFont(m_d3ddev, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V20B] );
  //added 1.10.2010
  D3DXCreateFont(m_d3ddev, 8, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A8] );
    /*
  D3DXCreateFont(m_d3ddev, 9, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A9] );
  D3DXCreateFont(m_d3ddev, 10, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A10] );
  D3DXCreateFont(m_d3ddev, 11, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A11] );
*/


}

void CGraphics::ReloadLost(){
  CLog *pLog = CLog::Instance();
  pLog->Log("Reloading sprite and fonts");
  if(FAILED( D3DXCreateSprite(m_d3ddev, &m_pSprite)) )
  {
    ::MessageBox(m_hWnd, "Failed to create a sprite!", "Fatal Error!", 0);//MB_OK | MB_ICONSTOP | MB_APPLMODAL);
    return;
  }
  InitializeFonts();
  SetupStuff();

  //lighting stuff
  m_d3ddev->SetRenderState(D3DRS_LIGHTING, true);
  D3DLIGHT9 dir;
  ::ZeroMemory(&dir, sizeof(dir));
  dir.Type = D3DLIGHT_DIRECTIONAL;
  dir.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
  dir.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.3f;
  dir.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 2.6f;
  dir.Direction = D3DXVECTOR3(0.0f, -100.0f, 0.0f);

  m_d3ddev->SetLight(0, &dir);
  m_d3ddev->LightEnable(0, true);
  m_d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
  m_d3ddev->SetRenderState(D3DRS_SPECULARENABLE, true);
}

void CGraphics::DeleteLost(){
  CLog *pLog = CLog::Instance();
  //pLog->Log("DL1");
  for(int i = 0; i < C_NUMBER_FONTS; i++){
    m_Font[i]->Release();   //David Flyman 2.23.2010
    m_Font[i] = NULL; 
  }
  //pLog->Log("DL2");
  for(unsigned int i = 0; i < m_texture.size(); ++i){
    m_texture[i].pBuffer->Release();  //David Flyman 2.23.2010
    m_texture[i].pBuffer = NULL;
  }
  //pLog->Log("DL3");
  m_pSprite->Release();//added
  //pLog->Log("DL4");
  m_texture.clear();
  //pLog->Log("DL5");
  m_sprites.clear();
  //pLog->Log("DL6");
  m_assetSpriteData.clear();
  //pLog->Log("DL7");
  m_assetFileData.clear();
  //pLog->Log("DL8");
}

void CGraphics::SetupStuff(){
  CLog *pLog = CLog::Instance();
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,
			                       D3DX_PI * 0.25f, // 90 - degree
			                       (float)m_screenWidth / (float)m_screenHeight,                              
			                       0.1f,
			                       800.0f);
  
	m_d3ddev->SetTransform(D3DTS_PROJECTION, &proj);
  m_d3ddev->SetRenderState(D3DRS_ZENABLE, true);
	m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CGraphics::BeginRender(){
  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
  m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  //m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
  
}

void CGraphics::EndRender(){
 m_pSprite->End();
 //tests follow
 //m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
 //m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
 //m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
}

//prints a number in sprite format
//must be manually updated with sprite information
//0 to 999999
void CGraphics::PrintSpriteNumber(int x, int y, long num){
  if (num < 0 || num > 999999)
    return;

  GRAPHIC_IMAGE gi;

  int posX = x;
  int posY = y;
  std::string sRef = "0123456789";
  std::ostringstream oss;
  oss << num;
  std::string sNum = oss.str();
  std::string sPrefix;
  if(sNum.length() < 6)
    sPrefix.append(6 - sNum.length(), '0');
  std::string sScore = sPrefix + sNum;

  for(int i = 0; i < sScore.size();++i){
    for(int j = 0; j < sRef.size(); ++j){
      if(sScore.at(i) == sRef.at(j)){
        gi = GetSprite(1200 + j);
        RenderSprite(gi, posX, posY, 255, 255, 255);
        posX += gi.width;
        break;
      }
    }
  }
}

//*****************************************************************************
// Renders a sprite based upon graphic image sprite data
//*****************************************************************************
void CGraphics::RenderSprite(GRAPHIC_IMAGE gi, float positionX, float positionY, int red, int green, int blue){
  CLog *pLog = CLog::Instance();
  if(gi.bValid == false)
    return;
  
  RECT rect;
  if(gi.frameMax > 1){//animated frames
    rect.left = gi.sourceX + (gi.frameCount * gi.width);
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;//gi.sourceX + gi.width;// * 2;
    rect.bottom = gi.sourceY + gi.height;// * 2;
  }
  else{ //static frame
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;//gi.sourceX + gi.width;// * 2;
    rect.bottom = gi.sourceY + gi.height;// * 2;
  }
  if(NULL == m_pSprite)
    return;
  
  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
  m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

  //************************* EXPERIMENTATION *********************
  //sets the screen color
  
  m_d3ddev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB(m_textureRed, m_textureGreen, m_textureBlue)); 
	m_d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	m_d3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
  //***************************************************************

  D3DXVECTOR2 pos;
  pos.x = positionX;
  pos.y = positionY;
  D3DXVECTOR2 scaling;
  scaling.x = gi.scale * gi.screenScale;
  scaling.y = gi.scale * gi.screenScale;
  D3DXVECTOR2 rotcenter;
  rotcenter.x = (rect.right - rect.left)/2 * gi.screenScale;
  rotcenter.y = (rect.bottom - rect.top)/2 * gi.screenScale;
  D3DXMATRIX mat;

  D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, gi.angle, &pos);
  m_pSprite->SetTransform(&mat);  
  if(NULL != GetTexture(gi.fileID)){
    if(FAILED(m_pSprite->Draw(GetTexture(gi.fileID), &rect, NULL, NULL, D3DCOLOR_ARGB(gi.alpha , red, green, blue)) )){
    }
  }
  D3DXMatrixIdentity(&mat);
  m_pSprite->SetTransform(&mat);
}


//*****************************************************************************
// Renders a sprite based upon graphic image sprite data - more parameters
//*****************************************************************************

void CGraphics::RenderSprite(GRAPHIC_IMAGE gi, float positionX, float positionY, float sx, float sy, float rx, float ry){
  CLog *pLog = CLog::Instance();
  if(gi.bValid == false)
    return;
  
  RECT rect;
  if(gi.frameMax > 1){//animated frames
    rect.left = gi.sourceX + (gi.frameCount * gi.width);
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = gi.sourceY + gi.height;
  }
  else{ //static frame
    rect.left = gi.sourceX;
    rect.top = gi.sourceY;
    rect.right = rect.left + gi.width;
    rect.bottom = gi.sourceY + gi.height;
  }
  if(NULL == m_pSprite)
    return;
  
  m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
  m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

  //************************* EXPERIMENTATION *********************
  //sets the screen color
  
  m_d3ddev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB(m_textureRed, m_textureGreen, m_textureBlue)); 
	m_d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	m_d3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
  //***************************************************************

  D3DXVECTOR2 pos;
  pos.x = positionX;
  pos.y = positionY;
  D3DXVECTOR2 scaling;
  scaling.x = sx * gi.screenScale;
  scaling.y = sy * gi.screenScale;
  D3DXVECTOR2 rotcenter;
  rotcenter.x = rx * gi.screenScale;//'(rect.right - rect.left)/2 * gi.screenScale;
  rotcenter.y = ry * gi.screenScale;//(rect.bottom - rect.top)/2 * gi.screenScale;
  D3DXMATRIX mat;

  D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, gi.angle, &pos);
  m_pSprite->SetTransform(&mat);  
  if(NULL != GetTexture(gi.fileID)){
    if(FAILED(m_pSprite->Draw(GetTexture(gi.fileID), &rect, NULL, NULL, D3DCOLOR_ARGB(gi.alpha , 255, 255, 255)) )){//red, green, blue)) )){
    }
  }
  D3DXMatrixIdentity(&mat);
  m_pSprite->SetTransform(&mat);
}


/*****************************************************
 Reads and loads data from assets.dat file into one of
 the vectors below.

 std::vector<GRAPHIC_FILE_DATA> m_assetFileData;
 std::vector<SPRITE_DATA> m_assetSpriteData;
 std::vector<MESH_DATA> m_assetMeshData;
 std::vector<SOUND_FILE_DATA> m_assetSoundData;

 Parameters in cfr begin with 1 not 0
*****************************************************/
bool CGraphics::LoadAssetFile(std::string filename){
  CLog *pLog = CLog::Instance();

  //validate asset data and add to appropriate vectors
  std::string parameter;
  std::string assetType;
  GRAPHIC_FILE_DATA   tempGraphic;
  SPRITE_DATA         tempSprite;  

  CCSVReader csv;
  csv.LoadFile(filename);

  //new stuff
  for(int i = 0; i < csv.GetTableSize(); ++i){

    if(csv.GetNumberParameters(i) > 1){
      assetType = csv.GetTerm(i, 0);

      if(assetType == "texture" && csv.GetNumberParameters(i) == 4){
        parameter = csv.GetTerm(i, 1);//fileID
        tempGraphic.fileID = atoi(parameter.c_str());
        parameter = csv.GetTerm(i, 2);//filename
        tempGraphic.filename = parameter;
        parameter = csv.GetTerm(i, 3);//description
        tempGraphic.description = parameter;
        m_assetFileData.push_back(tempGraphic);
      }

      else if(assetType == "sprite" && csv.GetNumberParameters(i) == 14 ){// && csv.GetNumberParameters(i) == 14){
        parameter = csv.GetTerm(i, 1);//spriteID
        tempSprite.spriteID = atoi(parameter.c_str());
        parameter = csv.GetTerm(i, 2);//fileID
        tempSprite.fileID = atoi(parameter.c_str());
        parameter = csv.GetTerm(i, 3);//left
        tempSprite.x = static_cast<float>(atof(parameter.c_str()));
        parameter = csv.GetTerm(i, 4);//right
        tempSprite.y = static_cast<float>(atof(parameter.c_str()));
        parameter = csv.GetTerm(i, 5);//width
        tempSprite.width = static_cast<float>(atof(parameter.c_str()));
        parameter = csv.GetTerm(i, 6);//height
        tempSprite.height = static_cast<float>(atof(parameter.c_str()));
        parameter = csv.GetTerm(i, 7);//scale
        tempSprite.scale =  static_cast<float>(atof(parameter.c_str()));
        parameter = csv.GetTerm(i, 8);//angle
        tempSprite.angle =  static_cast<float>(atof(parameter.c_str()));
        parameter = csv.GetTerm(i, 9);//alpha
        tempSprite.alpha = atoi(parameter.c_str());
        parameter = csv.GetTerm(i, 10);//total number of frames
        tempSprite.numberFrames = atoi(parameter.c_str());
        parameter = csv.GetTerm(i, 11);//framecount, starting frame
        tempSprite.frameCount = atoi(parameter.c_str());        
        parameter = csv.GetTerm(i, 12);//time between frames
        tempSprite.timeBetweenFrames = atof(parameter.c_str());// static_cast<float>(atof(parameter.c_str()));
        parameter = csv.GetTerm(i, 13);//description
        tempSprite.description = parameter;
        m_assetSpriteData.push_back(tempSprite);
      }
    }
  }

  //load graphics files
  for(unsigned int i = 0; i < m_assetFileData.size(); ++i){
    LoadTextureFile(m_assetFileData[i].fileID, "assets//graphics//" + m_assetFileData[i].filename);
  }

  pLog->Log("Exit loading of files");

  //load sprite information
  GRAPHIC_IMAGE gi;
  for(unsigned int i = 0; i < m_assetSpriteData.size(); ++i){
    gi.spriteID = m_assetSpriteData[i].spriteID;
    gi.fileID = m_assetSpriteData[i].fileID;
    gi.sourceX = static_cast<int>(m_assetSpriteData[i].x);
    gi.sourceY = static_cast<int>(m_assetSpriteData[i].y);
    gi.width = static_cast<int>(m_assetSpriteData[i].width);
    gi.height = static_cast<int>(m_assetSpriteData[i].height);
    gi.scale = m_assetSpriteData[i].scale;    
    gi.alpha = m_assetSpriteData[i].alpha;
    gi.angle = m_assetSpriteData[i].angle;
    gi.frameMax = m_assetSpriteData[i].numberFrames;
    gi.updateInterval = m_assetSpriteData[i].timeBetweenFrames;
    gi.frameCount = 1;
    gi.animTime = 0;
    gi.screenScale = 1.0; //added 4.8.09 by CB used in passing screen scale to render function
    
    //ensure the fileID really exists
    gi.bValid = false; //assume file is invalid
    for(unsigned int j = 0; j < m_assetFileData.size(); ++j){
      if(gi.fileID == m_assetFileData[j].fileID){
        gi.bValid = true;
        m_sprites.push_back(gi);
        break;
      }
    }
  }


  return true;
}

/****************************************************
 Loads texture files and skins
****************************************************/
bool CGraphics::LoadTextureFile(int fileID, std::string filename){
  S_TEXTURE temp;

  CLog *pLog = CLog::Instance();
  HRESULT hr = D3DXCreateTextureFromFile(m_d3ddev, filename.c_str(),&temp.pBuffer);
  
  if(FAILED(hr)){
    pLog->Log("Failed to load texture file ", filename);
    return false;
  }
  else{
    pLog->Log("Texture loaded correctly!");
  }
  temp.id = fileID;
  m_texture.push_back(temp);
  return true;
}

/****************************************************
 Returns pointer to texture graphic...using file ID
****************************************************/
IDirect3DTexture9* CGraphics::GetTexture(int id){
  
  for (unsigned int i = 0; i < m_texture.size(); ++i){
    if(m_texture[i].id == id){
      return m_texture[i].pBuffer;
    }
  }
  return NULL;
}

/****************************************************
 Returns sprite data based upon sprite ID
****************************************************/
GRAPHIC_IMAGE CGraphics::GetSprite(int id){
  for(unsigned int i = 0; i < m_sprites.size(); ++i){
    if(m_sprites[i].spriteID == id)
      return m_sprites[i];
  }
  GRAPHIC_IMAGE gi;
  gi.bValid = false;
  return gi;
}

//-----------------------------------------------------------------------------
// Name : FindDepthStencilFormat ()		(private)
// Desc : This function simply determines the best depth format that is
//        available for the specified mode.
// Note : No tests for stencil active depth buffers are made.
//-----------------------------------------------------------------------------
D3DFORMAT CGraphics::FindDepthStencilFormat( ULONG AdapterOrdinal, D3DDISPLAYMODE Mode, D3DDEVTYPE DevType )
{

    // Test for 24 bith depth buffer
    if (SUCCEEDED( m_d3d->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D32 )))
    {
        if (SUCCEEDED( m_d3d->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D32 ))) return D3DFMT_D32;
    
    } // End if 32bpp Available

    // Test for 24 bit depth buffer
    if (SUCCEEDED( m_d3d->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D24X8 )))
    {
        if (SUCCEEDED( m_d3d->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D24X8 ))) return D3DFMT_D24X8;
    
    } // End if 24bpp Available

    // Test for 16 bit depth buffer
    if (SUCCEEDED( m_d3d->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D16 )))
    {
        if (SUCCEEDED( m_d3d->CheckDepthStencilMatch ( AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D16 ))) return D3DFMT_D16;
    
    } // End if 16bpp Available

    // No depth buffer supported
    return D3DFMT_UNKNOWN;

}


void CGraphics::DrawLine(float x1, float y1,float x2, float y2, int red, int green, int blue){

  const int NUMPOINTS = 2;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex line[NUMPOINTS + 1];
  const DWORD line_fvf = D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  

  line[0].x = x1-1 ;
  line[0].y = y1 ;
  line[0].z = 0;
  line[0].rhw = 1.0f;
  line[0].color = D3DCOLOR_XRGB(red, green, blue);
  
  line[1].x = x2-1;
  line[1].y = y2;
  line[1].z = 0;
  line[1].rhw = 1.0f;
  line[1].color = D3DCOLOR_XRGB(red, green, blue);


  m_d3ddev->SetFVF(line_fvf);
  m_d3ddev->SetTexture(0, NULL);
  m_d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &line[0],sizeof(line[0]));

}


void CGraphics::DrawRect(float x1, float y1,float x2, float y2, int red, int green, int blue){

  const int NUMPOINTS = 5;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex line[NUMPOINTS + 1];
  const DWORD line_fvf = D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  

  line[0].x = x1;
  line[0].y = y1;
  line[0].z = 0;
  line[0].rhw = 1.0f;
  line[0].color = D3DCOLOR_XRGB(red, green, blue);
  
  line[1].x = x2;
  line[1].y = y1;
  line[1].z = 0;
  line[1].rhw = 1.0f;
  line[1].color = D3DCOLOR_XRGB(red, green, blue);

  line[2].x = x2;
  line[2].y = y2;
  line[2].z = 0;
  line[2].rhw = 1.0f;
  line[2].color = D3DCOLOR_XRGB(red, green, blue);
  
  line[3].x = x1;
  line[3].y = y2;
  line[3].z = 0;
  line[3].rhw = 1.0f;
  line[3].color = D3DCOLOR_XRGB(red, green, blue);

  line[4].x = x1;
  line[4].y = y1;
  line[4].z = 0;
  line[4].rhw = 1.0f;
  line[4].color = D3DCOLOR_XRGB(red, green, blue);

  m_d3ddev->SetFVF(line_fvf);
  m_d3ddev->SetTexture(0, NULL);
  m_d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS - 1, &line[0],sizeof(line[0]));

}

//draws circle
void CGraphics::DrawCircle(float x, float y, int red, int green, int blue, float radius){

  const int NUMPOINTS = 48;//increased to 48 from 24 6.20.09
  const float PI = 3.14159f;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex circle[NUMPOINTS + 1];
  const DWORD line_fvf=D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  
  float theta;
  float wedgeAngle;
  wedgeAngle = (float)((2 * PI)/NUMPOINTS);
  for (int i = 0; i <= NUMPOINTS; i++){
    theta = i * wedgeAngle;
    circle[i].x = (float)(x + radius * cos(theta));
    circle[i].y = (float)(y - radius * sin(theta));
    circle[i].z = 0;
    circle[i].rhw = 1.0f;
    circle[i].color = D3DCOLOR_XRGB(red, green, blue);
  }

  m_d3ddev->SetFVF(line_fvf);
  m_d3ddev->SetTexture(0, NULL);
  m_d3ddev->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &circle[0],sizeof(circle[0]));
  
}

//screen capture
HRESULT CGraphics::ScreenGrab(char *fileName){
  HRESULT hr;

  ::D3DDISPLAYMODE mode;
  if(FAILED(hr = m_d3ddev->GetDisplayMode(0, &mode)))
    return 100;

  ::LPDIRECT3DSURFACE9 surf;
  if(FAILED(hr=m_d3ddev->CreateOffscreenPlainSurface(mode.Width, mode.Height,D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surf, NULL)))
    return 200;

  if(FAILED(hr=m_d3ddev->GetFrontBufferData(0, surf))){
    surf->Release();
    return 300;
  }

  hr = ::D3DXSaveSurfaceToFile(fileName, D3DXIFF_JPG, surf, NULL, NULL);
  surf->Release();
  return 0;
}

//prints a vector of 'std::string'
void CGraphics::PrintVector(std::vector<std::string> &vec, int fontType, float posX, float posY, int red, int green, int blue, int alpha){
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int fontHeight = 12; //default value
  
  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;
  else if(fontType == 12 || fontType == 13)
    fontHeight = 20;

  RECT rct;
  int width = 0;//(int)( oss.str().size() * fontHeight);

  for(unsigned int i = 0; i < vec.size(); ++i){
    width = (int)(vec[i].size() * fontHeight);
    rct.left = posX;
    rct.top = posY + (i * fontHeight + 2);
    rct.right = rct.left + width;
    rct.bottom = rct.top + fontHeight + 2;
    m_Font[fontType]->DrawText(NULL, vec[i].c_str(), -1, &rct, 0, fontColor);  
  }
}

//prints a vector of 'float'
void CGraphics::PrintVector(std::vector<float> &vec, int fontType, float posX, float posY, int red, int green, int blue, int alpha){

  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int fontHeight = 12; //default value
  
  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;
  else if(fontType == 12 || fontType == 13)
    fontHeight = 20;

  RECT rct;
  int width = 0;
  std::ostringstream oss;
  std::string output;
  
  for(unsigned int i = 0; i < vec.size(); ++i){
    oss.str("");
    oss << std::setprecision(4) << vec[i];
    output = oss.str();
    width = (int)(oss.str().size() * fontHeight);
    rct.left = posX;
    rct.top = posY + (i * fontHeight + 2);
    rct.right = rct.left + width;
    rct.bottom = rct.top + fontHeight + 2;
    m_Font[fontType]->DrawText(NULL, output.c_str(), -1, &rct, 0, fontColor);  
  }



}
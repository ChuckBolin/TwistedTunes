#include "..\include\CGameData.h"

//game data constructor
//set all initial game data variables here
CGameData::CGameData(){

  //new data
  m_bPaused = false;
  m_bGameOver = false;

  //default information...may be replaced by level data file
  m_screenColorRed = 0;
  m_screenColorGreen = 111;
  m_screenColorBlue = 255;  

  //m_worldX = 0;
  //m_worldY = 0;
  //m_worldLeft = 0;
  //m_worldRight = 72000;
  //m_worldTop = 0;
  //m_worldBottom = 48000;
  m_posX = -2536; //50X50...CENTERS SCREEN
  m_posY = - 2800;

  m_bLeftMouseDown = false;
  m_bRightMouseDown = false;
//  m_timeScale = 1.0;
//  m_windowedYOffset = 0; //windowed mode has 20 pixels extra for caption bar
 // m_bShowMap = false; //shows map when true
//  m_brushSize = 1;    // number of tiles to paint in map mode
//  m_viewMode = 0;     //left isometric view
}

CGameData::~CGameData(){

}

//need to add sprite data to level data
bool CGameData::AddGraphicDataToLevelData(CGraphics & con){
  CLog *pLog = CLog::Instance();
  pLog->Log("Completed the addition of graphic data to level data");
  return true;
}

bool CGameData::AddObject(CGraphics &con, std::string objectName, int x, int y){
  return true;
}


/*
bool CGameData::LoadObjectFile(std::string filename){
//  m_catalog.LoadFile(filename);
//  if(m_catalog.GetTableSize() > 0)
//    return true;
//  else
//    return false;
  return true;
}
*/

void CGameData::Update(double timeDifference){
  CLog *pLog = CLog::Instance();

  static double totalTime = 0;
  totalTime += timeDifference;
}

/*
bool CGameData::LoadGameLevel(std::string filename){
  CLog *pLog = CLog::Instance();

  std::string value;
  std::string asset;
  std::string parameter;
  TILE_STRUCTURE ts;

  m_gameTile.ClearTiles();

  //new
  pLog->Log("Loading game level", filename);
  CCSVReader csv;
  csv.LoadFile(filename);
  pLog->Log("Level table size", csv.GetTableSize());
  if(csv.GetTableSize() < 1)
    return false;
  for(size_t i = 0; i < csv.GetTableSize(); ++i){
    asset = csv.GetTerm(i, 0);        
    if(asset == "song" && csv.GetNumberParameters(i)== 6 ){
      parameter = csv.GetTerm(i, 1);
      ts.songID  = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 2);
      ts.spriteID = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 3);
      ts.soundID = atoi(parameter.c_str());      
      parameter = csv.GetTerm(i, 4);
      ts.blockNumber = atoi(parameter.c_str());      
      parameter = csv.GetTerm(i, 5);
      ts.maxBlocks = atoi(parameter.c_str());      
      ts.topLeftX = 0;
      ts.topLeftY = 0;
      ts.type = 1;
      m_gameTile.AddTile(ts);
    }
    else if(asset == "sound" && csv.GetNumberParameters(i)== 6 ){
      parameter = csv.GetTerm(i, 1);
      ts.songID  = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 2);
      ts.spriteID = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 3);
      ts.soundID = atoi(parameter.c_str());      
      parameter = csv.GetTerm(i, 4);
      ts.blockNumber = atoi(parameter.c_str());      
      parameter = csv.GetTerm(i, 5);
      ts.maxBlocks = atoi(parameter.c_str());      
      ts.topLeftX = 0;
      ts.topLeftY = 0;
      ts.type = 2;
      m_gameTile.AddTile(ts);
    }
  }

  //need to generate xy grid position...must be unique

  return true;
}
*/
//bool CGameData::SaveLevelFile(int fileNum){
//  return true;
//}

  /*
  if(fileNum < 1 || fileNum > 4)
    return false;

  std::string filename;
  if(fileNum == 1)
    filename = "assets\\data\\saved1.dat";
  else if(fileNum == 2)
    filename = "assets\\data\\saved2.dat";
  else if(fileNum == 3)
    filename = "assets\\data\\saved3.dat";
  else if(fileNum == 4)
    filename = "assets\\data\\saved4.dat";

  std::string line;
  std::ostringstream oss;
  */
  /*
  std::ofstream file(filename.c_str());
  if(file.is_open()){
    //add header
    file << "//Golden Spike Saved Game: " << filename << std::endl;
    file << "//****************************************************" << std::endl;
    
    //game variables
    file << "//miscellaneous game variables" << std::endl;
    file << "******************************" << std::endl;
    oss.str("");
    oss << "back_color" << ", " << m_screenColorRed << ", " << m_screenColorGreen << ", " << m_screenColorBlue << std::endl;
    file << oss.str();
*/
    /*
    oss.str("");    
    oss << "world" << ", " << m_worldLeft  << ", " <<  m_worldTop  << ", " << m_worldRight  << ", " <<  m_worldBottom << std::endl;
    file << oss.str();

    oss.str("");
    oss << "position" << ", " <<  m_worldX << ", " << m_worldY <<std::endl;
    file << oss.str();
    */
/*
    oss.str("");    
    oss << "scale" << ", " <<  m_scale << std::endl;
    file << oss.str();


    oss.str("");        
    file << std::endl;

    file.close();
  }  
  */


bool CGameData::OpenLevel (int level, CGraphics &con){
  CLog *pLog = CLog::Instance();
  if(level < 0 || level > 5)
    return false;
  m_gameLevel = level;
/*
  if(level == 1)
    LoadGameLevel("assets\\data\\level1.dat");
  else if(level == 2)
    LoadGameLevel("assets\\data\\level2.dat");
  else if(level == 3)
    LoadGameLevel("assets\\data\\level3.dat");
  else if(level == 4)
    LoadGameLevel("assets\\data\\level4.dat");
  else if(level == 5)
    LoadGameLevel("assets\\data\\level5.dat");

  pLog->Log("Level loaded", level);
*/
 // AddGraphicDataToLevelData(con);  

  return true;
}

//primary formula to increase credits and scores
//*******************************************************
void CGameData::IncrementCredits(float health){
}
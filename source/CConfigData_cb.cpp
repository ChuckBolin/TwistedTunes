#include "..\include\CConfigData.h"

CConfigData::CConfigData(){
  Initialize();
}


CConfigData::CConfigData(std::string configFilename){
  Initialize();
  LoadConfigFile(configFilename);
}

CConfigData::~CConfigData(){
}

bool CConfigData::LoadConfigFile(std::string configFilename){
  CLog *pLog = CLog::Instance();

  //function variables
  std::string line;
  int equalCount = 0;
  std::ifstream file(configFilename.c_str());
  
  //open file for reading
  if(file.is_open()){

    //read one line at a time
    pLog->Log("Reading config file");
    while(!file.eof()){
      getline (file, line);
      line = RemoveSpaces(line);
      line = RemoveComments(line);
      equalCount = CountEqualSigns(line);
      if(line.size() != 0 && equalCount == 1){
        m_lines.push_back(line);
        equalCount = 0;
        pLog->Log("Config line", line);
      }
    }

    file.close();
  }

  //file doesn't exist
  else
    return false;

  //break string into key and value
  std::string key;
  std::string value;
  size_t equalPos = 0;
  bool bFound;
  
  pLog->Log("***************************************");
  pLog->Log("Configuration File Data (key = value)");
  pLog->Log("***************************************");

  for(size_t i = 0; i < m_lines.size(); ++i){
    
    equalPos = m_lines[i].find("=");
    if(equalPos > 0){
      key = m_lines[i].substr(0, equalPos);
      value = m_lines[i].substr(equalPos + 1);
      
      //load all config variables with values
      bFound = false;

      if(key == "ScreenLeft"){
        ScreenLeft = atoi(value.c_str());
        bFound = true;
      }
      else if(key == "ScreenTop"){
        ScreenTop = atoi(value.c_str());
        bFound = true;
      }
      else if(key == "ScreenWidth"){
        ScreenWidth = atoi(value.c_str());
        bFound = true;
      }
      else if(key == "ScreenHeight"){
        ScreenHeight = atoi(value.c_str());
        bFound = true;
      }
      else if(key == "FullScreen"){
        if(atoi(value.c_str()) == 1)
          FullScreen = true;
        else
          FullScreen = false;
        bFound = true;
      }
      else if(key == "DisplayDebugInfo"){
        if(atoi(value.c_str()) == 0)
          DisplayDebugInfo = false;
        else if(atoi(value.c_str()) == 1)
          DisplayDebugInfo = true;
        bFound = true;
      }
      else if(key == "LogDebugInfo"){
        if(atoi(value.c_str()) == 0)
          LogDebugInfo = false;
        else if(atoi(value.c_str()) == 1)
          LogDebugInfo = true;
        bFound = true;
      }
      else if(key == "PlaySounds"){
        if(atoi(value.c_str()) == 0)
          PlaySounds = false;
        else if(atoi(value.c_str()) == 1)
          PlaySounds = true;
        bFound = true;
      }
      else if(key == "PlayMusic"){
        if(atoi(value.c_str()) == 0)
          PlayMusic = false;
        else if(atoi(value.c_str()) == 1)
          PlayMusic = true;
        bFound = true;
      }
      else if(key == "FrameworkAssetFile"){
        FrameworkAssetFile = value;
        bFound = true;
      }
      else if(key == "GamePlayAssetFile"){
        GamePlayAssetFile = value;
        bFound = true;
      }
      else if(key == "SoundAssetFile"){
        SoundAssetFile = value;
        bFound = true;
      }

      else if(key == "ObjectsFile"){
        ObjectsFile = value;
        bFound = true;
      }
      else if(key == "EffectsFile"){
        EffectsFile = value;
        bFound = true;
      }
      else if(key == "EffectsFileI"){
        EffectsFileI = value;
        bFound = true;
      }
      else if(key == "CreditsFile"){
        CreditsFile = value;
        bFound = true;
      }
      else if(key == "TilesFile"){
        TilesFile = value;
        bFound = true;
      }
      else if(key == "ProgramName"){
        ProgramName = value;
        bFound = true;
      }
      else if(key == "ProgramVersion"){
        ProgramVersion = value;
        bFound = true;
      }     
      else if(key == "RootPath"){
        RootPath = value;
        bFound = true;
      }     

      /*
      else if(key == "Level1"){
        Level1 = value;
        bFound = true;
      }
      else if(key == "Level2"){
        Level2 = value;
        bFound = true;
      }
      else if(key == "Level3"){
        Level3 = value;
        bFound = true;
      }
      else if(key == "Level4"){
        Level4 = value;
        bFound = true;
      }
      else if(key == "Level5"){
        Level5 = value;
        bFound = true;
      }
      else if(key == "Level6"){
        Level6 = value;
        bFound = true;
      }
      else if(key == "Level7"){
        Level7 = value;
        bFound = true;
      }
      else if(key == "Level8"){
        Level8 = value;
        bFound = true;
      }
      else if(key == "Level9"){
        Level9 = value;
        bFound = true;
      }
      else if(key == "Level10"){
        Level10 = value;
        bFound = true;
      }
      */
      if(bFound == false)
        pLog->Log("No match for config file key", key);
    }
  }
}

//sets default values for all configuration values
void CConfigData::Initialize(){

  //[Screen]
  ScreenLeft = 0;
  ScreenTop = 0;
  ScreenWidth = 1024;
  ScreenHeight = 768;
  FullScreen = false;

  //[Debug]
  DisplayDebugInfo = true;
  LogDebugInfo = true;

  //[Audio]
  PlaySounds = false;
  PlayMusic = false;

  //[Files]
  FrameworkAssetFile = "assets//data//editor.dat";
  GamePlayAssetFile  = "assets//data//assets.dat";
  SoundAssetFile     = "assets//data//sounds.dat";
  GameObjectFile     = "assets//data//objects.dat";
  CreditsFile        = "assets//data//credits.dat";
  TilesFile          = "assets//data//tiles.dat";
  EffectsFile        = "assets//data//effects.dat";
  EffectsFileI       = "assets//data//effects_i.dat";
  RootPath           = "assets//data//";

  //[Data]
  ProgramName        = "Exodus";
  ProgramVersion     = "";
}

//removes all spaces from a string
std::string CConfigData::RemoveSpaces(std::string in){
  std::string temp;

  for(size_t i = 0; i < in.size(); i++){
    if(in.substr(i, 1) != " "){
      temp = temp + in.substr(i,1);      
    }
  }

  return temp;
}


//removes all comments from a string //
std::string CConfigData::RemoveComments(std::string in){
  std::string temp;

  for(size_t i = 0; i < in.size(); i ++){
    if(in.substr(i,1) != ";")
      temp = temp + in.substr(i, 1);
    else
      return temp;
  }
  return temp;
}

//counts number of equal signs in string
int CConfigData::CountEqualSigns(std::string in){
  int count = 0;

  for(size_t i = 0; i < in.size(); i++){
    if(in.substr(i, 1) == "=")
      count++;
  }
  
  return count;
}
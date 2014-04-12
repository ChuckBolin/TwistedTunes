//CObject.h
//base class...all other game states are derived from here
#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include <vector>
#include "CGraphics.h"

//forward initialization
class CGraphics;
class CGameData;
class CObject;

struct GAME_OBJECT_DEFINITION{
  size_t id;
  size_t groupID;
  std::string name;
  int specialCode; //user defined
  int rating;    //1 - 5
  int footprint; //0 = circle, 1 = rectangle
  float radius;
  float widthNS;   
  float widthEW; 
  int layer; //0=ground level, 1 = taller object
  int numberSprites;
  float xOffset;
  float yOffset;
  int frameMax;
  int frameCount;
  double updateInterval;
  double animTime;  
  int numberSounds;
  std::vector<int> vecSoundID;
  std::vector<int> vecSpriteID;
  std::vector<int> vecSpriteID2;
}; 

//stores information related to a specific instance of an object
struct GAME_OBJECT{
  GAME_OBJECT_DEFINITION gd;

  //unique for each object
  float tileRow;
  float tileCol;
  float positionX;
  float positionY;
  float positionZ; //vertical offsets
}; 


class CObject{
public:
  virtual void Update(double timeDifference, CGameData &data) = 0;
  virtual void Render(CGraphics &con, CGameData &data) = 0;
  virtual void SetObject(GAME_OBJECT game_object) = 0;
  virtual bool RenderEffects(int &targetX,int &targetY, int &weaponType) = 0;
  virtual int GetLayer() = 0;
  virtual GAME_OBJECT GetObjectData() = 0;

  GAME_OBJECT m_go;
};

#endif 
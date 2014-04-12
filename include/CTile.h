#ifndef CTILE_H
#define CTILE_H

#include <vector>
#include <string>
#include <cmath>
#include "CGraphics.h"
#include "CGameData.h"
#include "CLog.h"
#include "CAudioManager.h"
#include "Keystatus.h"

struct TILE_STRUCTURE{
  int songID;
  int spriteID;
  int soundID;
  int audioIndex; //corresponding index in audio class...used for moving
  int blockNumber;
  int maxBlocks;
  int topLeftX;
  int topLeftY;
  int lastTopLeftX;
  int lastTopLeftY;
  int originalX;
  int originalY;
  int location; //0=play area, 1=top section
  //int type; //0=neutral, 1=music, 2=sound effect
  //bool inPlay; //true = tile is insided 640x480 area
  int abovePos; //if inPlay = false, 1234
  bool bValidTile;
  int frameMax;
  int frameCount;
  double updateInterval;
  double animTime;
};

struct TILE_ASSEMBLY_STRUCTURE{
  int songID;
  int tileCount;
  bool bIndex0;
  bool bIndex1;
  bool bIndex2;
  bool bIndex3;
  int tileIndex0;
  int tileIndex1;
  int tileIndex2;
  int tileIndex3;
};

struct BLING{
  int spriteID;
  float x;
  float y;
};

class CGameData;
class CGraphics;

class CTile{
public:
  CTile();
  ~CTile();
  void ResetAssemblyArea();
  void Update(double timeDifference, CGameData &data, CMouse & mouse);
  void Render(CGraphics &con, CGameData &data, CMouse & mouse);
  void ClearTiles();
  void AddTile(TILE_STRUCTURE ts);
  bool LoadGameLevel(std::string filename);  
  void SetLevel(int level);
  unsigned int GetScore(){return m_score;}
  void MouseDown(int mx, int my);
  void RightMouseDown(int mx, int my);
  void MouseUp( CMouse & mouse);
  void RightMouseUp( CMouse & mouse);
  int GetTimer(){return m_countDownTimer;}

  bool m_bMouseDown;
  int m_dx;
  int m_dy;
  int m_mouseX;
  int m_mouseY;
  int m_mouseTileIndex;
  int m_moveType; //0=inside play area, 1==area to top, 2=top to area
  int m_soundCount; //number of sound tiles above
  int m_soundList[4];
  int m_soundListPointer[4];
  int GetTotalActiveSongs(){return m_totalSongsInLevel;}
  

private:
  std::vector<TILE_STRUCTURE> m_tiles;
  int m_soundNumber;
  int m_areaTop;
  int m_assyIndex;
  TILE_ASSEMBLY_STRUCTURE m_TAS; //stores assembly songID and bool for each index
  int m_totalSongsInLevel;

  BLING m_bling[4];
  bool m_bBling;    //made true when all 4 tiles assembled, triggers a bling show
  double m_blingTime;
  
  int m_level;
  bool m_rightMouseDown;
  bool m_bTest;
  int m_tileInfoIndex;

  unsigned long m_score;       //total score
  bool m_bBonus;              //true means eligible to score bonus points
  int m_countDownTimer;       //current timer
  //int m_countDownTimerMax;    //if timer is less than max then score extra points
  long m_countDownExtraPoints; //points to score per second
  long m_pointsPerSong;        //points to score per song
  long m_removePoints;         //points to deduct for right clicking and removing from assembly area

};

#endif
#ifndef COBJECTMANAGER_H
#define COBJECTMANAGER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "CGraphics.h"
#include "CTimer.h"
#include "CGameData.h"
#include "CConfigData.h"
#include "CLog.h"
//#include "CObject.h"
#include "CCSVReader.h"
//#include "CThing.h"

class CObjectManager{
public:
  CObjectManager();
  ~CObjectManager();
  void Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con);
  void Render(CGraphics &con,  CGameData &data, CConfigData &cfg);
  void Render(CGraphics &con,  CGameData &data, CConfigData &cfg, int row, int col);
  bool LoadObjectDefinitions(CGraphics &con, CConfigData &cfg);
  void LoadLevelFile(std::string sFilename, CGraphics &con);
  void SaveLevelFile(std::string sFilename);
  void AddObject(float x, float y, float z, CGraphics &con);
  void AddObject(float x, float y, float z, int row, int col, CGraphics &con);
  void ClearObjects();
  void RemoveObject(float x, float y);
  int CalcIsoRow(float x, float y, int viewMode);
  int CalcIsoCol(float x, float y, int viewMode);

  //group objects
  size_t GetObjectDefinitionSize(){return m_objectDefinition.size();}
  size_t GetGroupSize(){return m_groupObjects.size();}
  size_t GetGroupNumber();
  int GetGroupObjectSprite(size_t index);
  //float GetGroupObjectZPosition(size_t index);
  void SetGroupNumber(size_t id);
  void SetGroupTileID(size_t id);

private:
  size_t m_objectID;
  std::vector<GAME_OBJECT_DEFINITION> m_objectDefinition;
  std::vector<GAME_OBJECT_DEFINITION> m_groupObjects;
  std::vector<CObject*> m_pObjects;
  size_t m_groupID;
  //size_t m_tileID;
};

#endif;

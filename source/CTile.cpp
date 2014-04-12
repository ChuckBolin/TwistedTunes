#include "..\include\CTile.h"

CTile::CTile(){
  m_soundNumber = -1;
  m_bMouseDown = false;
  m_dx = 0;
  m_dy = 0;
  m_moveType = 0;
  m_soundCount = 0;
  m_areaTop = 162;
  m_assyIndex = 0;

  m_TAS.songID = -1;
  m_TAS.tileCount = 0;
  m_TAS.bIndex0 = false;
  m_TAS.bIndex1 = false;
  m_TAS.bIndex2 = false;
  m_TAS.bIndex3 = false;
  m_TAS.tileIndex0 = -1;
  m_TAS.tileIndex1 = -1;
  m_TAS.tileIndex2 = -1;
  m_TAS.tileIndex3 = -1;
  m_totalSongsInLevel = 0;
  m_score = 0;
  m_level = 0;
  m_rightMouseDown = false;
  m_bTest = false;
  m_tileInfoIndex = -1;
}

CTile::~CTile(){

}

//used only for development and testing
void CTile::ResetAssemblyArea(){
  m_TAS.songID = -1;
  m_TAS.tileCount = 0;
  m_TAS.bIndex0 = false;
  m_TAS.bIndex1 = false;
  m_TAS.bIndex2 = false;
  m_TAS.bIndex3 = false;
  m_TAS.tileIndex0 = -1;
  m_TAS.tileIndex1 = -1;
  m_TAS.tileIndex2 = -1;
  m_TAS.tileIndex3 = -1;
  m_totalSongsInLevel = 0;


}

void CTile::SetLevel(int level){
  m_level = level;

  if(m_level == 1){
    m_score = 0;
    m_countDownTimer = 80;
    m_countDownExtraPoints = 40;
    m_removePoints = 40;
    m_pointsPerSong = 40;
    m_bBonus = true;
  }
  else if(m_level == 2){
    m_countDownTimer = 120;
    m_countDownExtraPoints = 60;
    m_removePoints = 60;
    m_pointsPerSong = 60;
    m_bBonus = true;
  }
  else if(m_level == 3){
    m_countDownTimer = 160;
    m_countDownExtraPoints = 80;
    m_removePoints = 80;
    m_pointsPerSong = 80;
    m_bBonus = true;
  }
  else if(m_level == 4){
    m_countDownTimer = 200;
    m_countDownExtraPoints = 100;
    m_removePoints = 100;
    m_pointsPerSong = 100;
    m_bBonus = true;
  }
  else if(m_level == 5){
    m_countDownTimer = 400;
    m_countDownExtraPoints = 200;
    m_removePoints = 200;
    m_pointsPerSong = 200;
    m_bBonus = true;
  }


}

//used to remove a tile from the assembly area
void CTile::RightMouseDown(int mx, int my){
  MouseDown(mx, my);
  CLog *pLog = CLog::Instance();
  m_rightMouseDown = true;

  //top-area...move tile back to player area
  if(m_mouseX > 443 && m_mouseX < 571 && m_mouseY > 100 && m_mouseY < 137){
  //if(mouse.MouseX() > 186 && mouse.MouseX() < 825 && mouse.MouseY() > 106 && mouse.MouseY() <= m_areaTop){ //mouse in top-section
    
    //right button down
    //if(m_rightMouseDown == true){
      if(m_mouseX >= 539){
        if(m_TAS.tileIndex3 > -1){ //tile index exists
          m_tiles[m_TAS.tileIndex3].topLeftX = m_tiles[m_TAS.tileIndex3].originalX;
          m_tiles[m_TAS.tileIndex3].topLeftY = m_tiles[m_TAS.tileIndex3].originalY;
          m_tiles[m_TAS.tileIndex3].bValidTile = true;
          m_tiles[m_TAS.tileIndex3].location = 0;
          m_score -= m_removePoints;
          if(m_score > 10000000)
            m_score = 0;
          m_TAS.tileIndex3 = -1;
          m_TAS.bIndex3 = false;
          m_TAS.tileCount--;
          if(m_TAS.tileCount == 0)
            m_TAS.songID = -1;
        }
      }
      else if(m_mouseX >= 507){
        if(m_TAS.tileIndex2 > -1){ //tile index exists
          m_tiles[m_TAS.tileIndex2].topLeftX = m_tiles[m_TAS.tileIndex2].originalX;
          m_tiles[m_TAS.tileIndex2].topLeftY = m_tiles[m_TAS.tileIndex2].originalY;
          m_tiles[m_TAS.tileIndex2].bValidTile = true;
          m_tiles[m_TAS.tileIndex2].location = 0;
          m_score -= m_removePoints;
          if(m_score > 10000000)
            m_score = 0;
          m_TAS.tileIndex2 = -1;
          m_TAS.bIndex2 = false;
          m_TAS.tileCount--;
          if(m_TAS.tileCount == 0)
            m_TAS.songID = -1;
        }
      }
      else if(m_mouseX >= 475){
        if(m_TAS.tileIndex1 > -1){ //tile index exists
          m_tiles[m_TAS.tileIndex1].topLeftX =  m_tiles[m_TAS.tileIndex1].originalX;
          m_tiles[m_TAS.tileIndex1].topLeftY =  m_tiles[m_TAS.tileIndex1].originalY;
          m_tiles[m_TAS.tileIndex1].bValidTile = true;
          m_tiles[m_TAS.tileIndex1].location = 0;
          m_score -= m_removePoints;
          if(m_score > 10000000)
            m_score = 0;
          m_TAS.tileIndex1 = -1;
          m_TAS.bIndex1 = false;
          m_TAS.tileCount--;
          if(m_TAS.tileCount == 0)
            m_TAS.songID = -1;
        }
      }
      else if(m_mouseX >= 443){
        if(m_TAS.tileIndex0 > -1){ //tile index exists
          m_tiles[m_TAS.tileIndex0].topLeftX =  m_tiles[m_TAS.tileIndex0].originalX;
          m_tiles[m_TAS.tileIndex0].topLeftY =  m_tiles[m_TAS.tileIndex0].originalY;
          m_tiles[m_TAS.tileIndex0].bValidTile = true;
          m_tiles[m_TAS.tileIndex0].location = 0;
          m_score -= m_removePoints;
          if(m_score > 10000000)
            m_score = 0;
          m_TAS.tileIndex0 = -1;
          m_TAS.bIndex0 = false;
          m_TAS.tileCount--;
          if(m_TAS.tileCount == 0)
            m_TAS.songID = -1;
        }
      }
    //}
  }  

//  if(m_mouseX > 443 && m_mouseX < 571 && m_mouseY > 100 && m_mouseY < 137){
    // m_rightMouseDown = true;
    //if(m_mouseX >= 539){
    // 
    //  if(m_TAS.bIndex3 == true){ //tile is located here
    //    if(m_TAS.tileIndex3 > -1){ //tile index exists
    //      
    //      m_tiles[m_TAS.tileIndex3].topLeftX = m_tiles[m_TAS.tileIndex3].lastTopLeftX;
    //      m_tiles[m_TAS.tileIndex3].topLeftY = m_tiles[m_TAS.tileIndex3].lastTopLeftY;
    //      m_TAS.tileIndex3 = -1;
    //      m_TAS.bIndex3 = false;
    //      m_TAS.tileCount++;
    //    }
    //  }
    //}
    //else if(m_mouseX >= 507){
    //  if(m_TAS.bIndex2 == true){ //tile is located here
    //    if(m_TAS.tileIndex2 > -1){ //tile index exists
    //      m_tiles[m_TAS.tileIndex2].topLeftX = 0;// m_tiles[m_TAS.tileIndex2].lastTopLeftX;
    //      m_tiles[m_TAS.tileIndex2].topLeftY = 0;// m_tiles[m_TAS.tileIndex2].lastTopLeftY;
    //      m_TAS.tileIndex2 = -1;
    //      m_TAS.bIndex2 = false;
    //      m_TAS.tileCount--;
    //    }
    //  }
    //}
    //else if(m_mouseX >= 475){
    //  if(m_TAS.bIndex1 == true){ //tile is located here
    //    if(m_TAS.tileIndex1 > -1){ //tile index exists
    //      m_tiles[m_TAS.tileIndex1].topLeftX = 0;// m_tiles[m_TAS.tileIndex1].lastTopLeftX;
    //      m_tiles[m_TAS.tileIndex1].topLeftY = 0;// m_tiles[m_TAS.tileIndex1].lastTopLeftY;
    //      m_TAS.tileIndex1 = -1;
    //      m_TAS.bIndex1 = false;
    //      m_TAS.tileCount--;
    //    }
    //  }
    //}
    //else if(m_mouseX >= 443){
    //  if(m_TAS.bIndex0 == true){ //tile is located here
    //    if(m_TAS.tileIndex0 > -1){ //tile index exists
    //      m_tiles[m_TAS.tileIndex0].topLeftX = 0;// m_tiles[m_TAS.tileIndex0].lastTopLeftX;
    //      m_tiles[m_TAS.tileIndex0].topLeftY = 0;// m_tiles[m_TAS.tileIndex0].lastTopLeftY;
    //      m_TAS.tileIndex0 = -1;
    //      m_TAS.bIndex0 = false;
    //      m_TAS.tileCount--;
    //    }
    //  }
    //}
  //}
}

void CTile::RightMouseUp(CMouse & mouse){
  CLog *pLog = CLog::Instance();
  
  m_rightMouseDown = false;
  m_bTest = false;

}

void CTile::MouseDown(int mx, int my){
  CLog *pLog = CLog::Instance();

  m_mouseX = mx;
  m_mouseY = my;  //mouse down in play area
  
  //mouse down in play area
  if(m_rightMouseDown == false && m_bMouseDown == false && m_mouseX > 186 && m_mouseX < 825 &&
    m_mouseY > m_areaTop && m_mouseY < 629){  //162
    m_dx = 0;
    m_dy = 0;
    int gridX = (m_mouseX - 186)/32;
    int gridY = (m_mouseY - m_areaTop + 8)/32;

    //iterate through all tiles and determine if tile exists in this spot
    for(size_t i = 0; i < m_tiles.size(); ++i){
      if(gridX >= m_tiles[i].topLeftX && gridX < m_tiles[i].topLeftX + 1 &&
        gridY >= m_tiles[i].topLeftY && gridY < m_tiles[i].topLeftY + 1){

        //only allow mouse down on valid tiles
        //if(m_tiles[i].bValidTile == true){
        m_dx = m_mouseX - 186 - (m_tiles[i].topLeftX * 32);
        m_dy = m_mouseY - m_areaTop - (m_tiles[i].topLeftY * 32) + 16;
        
        //store location (row col)
        m_tiles[i].lastTopLeftX = m_tiles[i].topLeftX;
        m_tiles[i].lastTopLeftY = m_tiles[i].topLeftY;

        m_bMouseDown = true;
        m_mouseTileIndex = i;
        break;
        //}
      }
    }
  }

  ////mouse down in top area...in one of four tile squares
  ////if tile square has a tile, restore tile to last location
  //else if(m_rightMouseDown == true && m_mouseX > 443 && m_mouseX < 571 &&
  //  m_mouseY > 100 && m_mouseY < 137){  //162

  //    if(m_mouseX >= 539){
  //      if(m_TAS.bIndex3 == true){ //tile is located here
  //        if(m_TAS.tileIndex3 > -1){ //tile index exists
  //          m_tiles[m_TAS.tileIndex3].topLeftX = 3;//m_tiles[m_TAS.tileIndex3].lastTopLeftX;
  //          m_tiles[m_TAS.tileIndex3].topLeftY = 0;//m_tiles[m_TAS.tileIndex3].lastTopLeftY;
  //          m_TAS.tileIndex3 = -1;
  //          m_TAS.bIndex3 = false;
  //          m_TAS.tileCount++;
  //        }
  //      }
  //    }
  //    else if(m_mouseX >= 507){
  //      if(m_TAS.bIndex2 == true){ //tile is located here
  //        if(m_TAS.tileIndex2 > -1){ //tile index exists
  //          m_tiles[m_TAS.tileIndex2].topLeftX = 2;//m_tiles[m_TAS.tileIndex2].lastTopLeftX;
  //          m_tiles[m_TAS.tileIndex2].topLeftY = 0;//m_tiles[m_TAS.tileIndex2].lastTopLeftY;
  //          m_TAS.tileIndex2 = -1;
  //          m_TAS.bIndex2 = false;
  //          m_TAS.tileCount++;
  //        }
  //      }
  //    }
  //    //else if(m_mouseX >= 475){
  //    //  if(m_TAS.bIndex1 == true){ //tile is located here
  //    //    if(m_TAS.tileIndex1 > -1){ //tile index exists
  //    //      m_tiles[m_TAS.tileIndex1].topLeftX = 0;// m_tiles[m_TAS.tileIndex1].lastTopLeftX;
  //    //      m_tiles[m_TAS.tileIndex1].topLeftY = 0;// m_tiles[m_TAS.tileIndex1].lastTopLeftY;
  //    //      m_TAS.tileIndex1 = -1;
  //    //      m_TAS.bIndex1 = false;
  //    //      m_TAS.tileCount--;
  //    //    }
  //    //  }
  //    //}
  //    //else if(m_mouseX >= 443){
  //    //  if(m_TAS.bIndex0 == true){ //tile is located here
  //    //    if(m_TAS.tileIndex0 > -1){ //tile index exists
  //    //      m_tiles[m_TAS.tileIndex0].topLeftX = 0;// m_tiles[m_TAS.tileIndex0].lastTopLeftX;
  //    //      m_tiles[m_TAS.tileIndex0].topLeftY = 0;// m_tiles[m_TAS.tileIndex0].lastTopLeftY;
  //    //      m_TAS.tileIndex0 = -1;
  //    //      m_TAS.bIndex0 = false;
  //    //      m_TAS.tileCount--;
  //    //    }
  //    //  }
  //    //}

  //  //m_mouseY > m_areaTop && m_mouseY < 629){  //162
  //  
  //  //int gridX = (m_mouseX - 186)/32;
  //  //int gridY = (m_mouseY - m_areaTop)/32;
  //  //if(m_moveType == 0){
  //  //  for(size_t i = 0; i < m_tiles.size(); ++i){
  //  //    if(gridX >= m_tiles[i].topLeftX && gridX < m_tiles[i].topLeftX + 1 &&
  //  //      gridY >= m_tiles[i].topLeftY && gridY < m_tiles[i].topLeftY + 1){

  //  //        //only allow mouse down on valid tiles
  //  //        if(m_tiles[i].bValidTile == true){
  //  //          m_dx = m_mouseX - 186 - (m_tiles[i].topLeftX * 32);
  //  //          m_dy = m_mouseY - m_areaTop - (m_tiles[i].topLeftY * 32) + 16;

  //  //          m_tiles[i].lastTopLeftX = m_tiles[i].topLeftX;
  //  //          m_tiles[i].lastTopLeftY = m_tiles[i].topLeftY;

  //  //          m_bMouseDown = true;
  //  //          m_mouseTileIndex = i;
  //  //          break;
  //  //        }
  //  //    }
  //  //  }
  //  //}
  //}
}

void CTile::MouseUp(CMouse & mouse){
  CLog *pLog = CLog::Instance();
  CAudioManager *pAudio = CAudioManager::Instance();

  int dx = 0;
  int dy = 0;

  //button released
  if(m_bMouseDown == true && m_rightMouseDown == false && m_mouseTileIndex > -1){

    //button released over assembly area
    if(mouse.MouseX() > 414 && mouse.MouseX() < 601 && mouse.MouseY() > 71 && mouse.MouseY() < 130){ //162){   
      if(m_bMouseDown == true && m_mouseTileIndex > -1){
        //pLog->Log("AssemblyTry", m_tiles[m_mouseTileIndex].songID, m_tiles[m_mouseTileIndex].blockNumber);

        //is the assembly area empty...check m_TAS.songID = -1...this is first tile for this song
        if(m_TAS.songID == -1){
          m_TAS.songID = m_tiles[m_mouseTileIndex].songID;
          m_TAS.tileCount = 0;
          m_TAS.bIndex0 = false;
          m_TAS.bIndex1 = false;
          m_TAS.bIndex2 = false;
          m_TAS.bIndex3 = false;
          m_TAS.tileIndex0 = -1;
          m_TAS.tileIndex1 = -1;
          m_TAS.tileIndex2 = -1;
          m_TAS.tileIndex3 = -1;
          //pLog->Log("m_TAS.songID", m_TAS.songID);
        }
        //correct tile for index 0
        if( m_TAS.songID == m_tiles[m_mouseTileIndex].songID && m_tiles[m_mouseTileIndex].blockNumber == 0 && m_TAS.bIndex0 == false){
          m_TAS.bIndex0 == true;
          m_TAS.tileIndex0 = m_mouseTileIndex;
          //m_bling[0].x = m_tiles[m_TAS.bIndex0].topLeftX;
          //m_bling[0].y = m_tiles[m_TAS.bIndex0].topLeftY;
          //m_bling[0].spriteID = m_TAS.tileIndex0;
          m_TAS.tileCount++;
          //m_tiles[m_mouseTileIndex].bValidTile = false;
          m_tiles[m_mouseTileIndex].topLeftX = 450;
          m_tiles[m_mouseTileIndex].topLeftY = 98;
          m_tiles[m_mouseTileIndex].location = 1;
          pAudio->MoveNoiseSource(m_mouseTileIndex, m_tiles[m_mouseTileIndex].topLeftX + 16, m_tiles[m_mouseTileIndex].topLeftY + 20, m_tiles[m_mouseTileIndex].topLeftY + 20);
          m_assyIndex++;
          m_bMouseDown = false;
          m_mouseTileIndex = -1;
          //pLog->Log("Assembly tile 0");
        }
        //correct tile for index 1
        else if( m_TAS.songID == m_tiles[m_mouseTileIndex].songID && m_tiles[m_mouseTileIndex].blockNumber == 1 && m_TAS.bIndex1 == false){
          m_TAS.tileCount++;
          m_TAS.bIndex1 == true;
          m_TAS.tileIndex1 = m_mouseTileIndex;
          //m_bling[1].x = m_tiles[m_TAS.bIndex1].topLeftX;
          //m_bling[1].y = m_tiles[m_TAS.bIndex1].topLeftY;
          //m_bling[1].spriteID = m_TAS.tileIndex1;
          //m_tiles[m_mouseTileIndex].bValidTile = false;
          m_tiles[m_mouseTileIndex].topLeftX = 482;
          m_tiles[m_mouseTileIndex].topLeftY = 98;
          m_tiles[m_mouseTileIndex].location = 1;
          pAudio->MoveNoiseSource(m_mouseTileIndex, m_tiles[m_mouseTileIndex].topLeftX + 16, m_tiles[m_mouseTileIndex].topLeftY + 20, m_tiles[m_mouseTileIndex].topLeftY + 20);
          m_assyIndex++;
          m_bMouseDown = false;
          m_mouseTileIndex = -1;
          //pLog->Log("Assembly tile 1");
        }
        //correct tile for index 2
        else if( m_TAS.songID == m_tiles[m_mouseTileIndex].songID && m_tiles[m_mouseTileIndex].blockNumber == 2 && m_TAS.bIndex2 == false){
          m_TAS.tileCount++;
          m_TAS.bIndex2 == true;
          m_TAS.tileIndex2 = m_mouseTileIndex;
          //m_bling[2].x = m_tiles[m_TAS.bIndex2].topLeftX;
          //m_bling[2].y = m_tiles[m_TAS.bIndex2].topLeftY;
          //m_bling[2].spriteID = m_TAS.tileIndex2;
          //m_tiles[m_mouseTileIndex].bValidTile = false;
          m_tiles[m_mouseTileIndex].topLeftX = 514;
          m_tiles[m_mouseTileIndex].topLeftY = 98;
          m_tiles[m_mouseTileIndex].location = 1;
          pAudio->MoveNoiseSource(m_mouseTileIndex, m_tiles[m_mouseTileIndex].topLeftX + 16, m_tiles[m_mouseTileIndex].topLeftY + 20, m_tiles[m_mouseTileIndex].topLeftY + 20);
          m_assyIndex++;
          m_bMouseDown = false;
          m_mouseTileIndex = -1;
          //pLog->Log("Assembly tile 2");
        }
        //correct tile for index 3
        else if( m_TAS.songID == m_tiles[m_mouseTileIndex].songID && m_tiles[m_mouseTileIndex].blockNumber == 3 && m_TAS.bIndex3 == false){
          m_TAS.tileCount++;
          m_TAS.bIndex3 == true;
          m_TAS.tileIndex3 = m_mouseTileIndex;
          //m_bling[3].x = m_tiles[m_TAS.bIndex3].topLeftX;
          //m_bling[3].y = m_tiles[m_TAS.bIndex3].topLeftY;
          //m_bling[3].spriteID = m_TAS.tileIndex3;
          //m_tiles[m_mouseTileIndex].bValidTile = false;
          m_tiles[m_mouseTileIndex].topLeftX = 546;
          m_tiles[m_mouseTileIndex].topLeftY = 98;
          m_tiles[m_mouseTileIndex].location = 1;
          pAudio->MoveNoiseSource(m_mouseTileIndex, m_tiles[m_mouseTileIndex].topLeftX + 16, m_tiles[m_mouseTileIndex].topLeftY + 202, m_tiles[m_mouseTileIndex].topLeftY + 20);
          m_assyIndex++;
          m_bMouseDown = false;
          m_mouseTileIndex = -1;
          //pLog->Log("Assembly tile 3");
        }
        //incorrect tile
        else{
          m_score -= m_removePoints;
          if(m_score > 10000000)
            m_score = 0;

        }

        //four tiles in assembly area...remove them
        if(m_TAS.tileCount == 4){ 
          for(unsigned int i = 0; i < m_tiles.size(); ++i){
            if(m_tiles[i].songID == m_TAS.songID){
              m_tiles[i].bValidTile = false;
              pAudio->KillSound(i);
            }
          }
          m_score += m_pointsPerSong; //m_level * 40;

          m_TAS.songID = -1;
          m_totalSongsInLevel--;

          //bonus if finish level before time delay  
          if(m_bBonus = true && m_totalSongsInLevel == 0 && m_countDownTimer > 0){//1st time here
            m_bBonus = false;
            m_score += (m_countDownExtraPoints * m_countDownTimer);
          }

          m_TAS.tileCount = 0;
          m_bBling = true;
          m_blingTime = 0;
          m_bling[0].x = 443;
          m_bling[0].y = 84;
          m_bling[0].spriteID = m_tiles[m_TAS.tileIndex0].spriteID;
          m_bling[1].x = 475;
          m_bling[1].y = 84;
          m_bling[1].spriteID = m_tiles[m_TAS.tileIndex1].spriteID;
          m_bling[2].x = 507;
          m_bling[2].y = 84;
          m_bling[2].spriteID = m_tiles[m_TAS.tileIndex2].spriteID;
          m_bling[3].x = 539;
          m_bling[3].y = 84;
          m_bling[3].spriteID = m_tiles[m_TAS.tileIndex3].spriteID;
          m_TAS.bIndex0 == false;
          m_TAS.bIndex1 == false;
          m_TAS.bIndex2 == false;
          m_TAS.bIndex3 == false;
        }       
    }

    //button released insided play area
    else if(mouse.MouseX() > 186 && mouse.MouseX() < 825 && mouse.MouseY() > m_areaTop + 128 && mouse.MouseY() < 629){   
      int tempX = (mouse.MouseX()  - 186)/32;;
      int tempY = (mouse.MouseY() - m_areaTop)/32;
      bool bFound = false;
      for(unsigned int i = 0; i < m_tiles.size();++i){
        if(i != m_mouseTileIndex && tempX == m_tiles[i].topLeftX && tempY == m_tiles[i].topLeftY){
          bFound = true;
          break;  
        }
      }
      if(bFound == true){//tile already here..restore     
        m_tiles[m_mouseTileIndex].topLeftX = m_tiles[m_mouseTileIndex].lastTopLeftX;
        m_tiles[m_mouseTileIndex].topLeftY = m_tiles[m_mouseTileIndex].lastTopLeftY;
        m_tiles[m_mouseTileIndex].location = 0;     
      }
      else{//assign new position  
        m_tiles[m_mouseTileIndex].topLeftX = (mouse.MouseX()  - 186)/32;
        m_tiles[m_mouseTileIndex].topLeftY  = (mouse.MouseY() - m_areaTop)/32;
        m_tiles[m_mouseTileIndex].location = 0;
        pAudio->MoveNoiseSource(m_mouseTileIndex, 186 + ((mouse.MouseX()  - 186)/32) * 32, m_areaTop + ((mouse.MouseY() - m_areaTop)/32)* 32, m_areaTop + ((mouse.MouseY() - m_areaTop)/32)* 32);
        //pAudio->MoveNoiseSource(m_mouseTileIndex, (mouse.MouseX()), (mouse.MouseY()), (mouse.MouseY()));
      }      
    }
    }
  }

  m_bMouseDown = false;
  m_mouseTileIndex = -1;
}

/*
  //inside top area
  else if(m_bMouseDown == true && m_mouseTileIndex > -1){
    if(mouse.MouseX() > 450 && mouse.MouseX() < 578 &&
      mouse.MouseY() > 116 && mouse.MouseY() < 144){
      m_tiles[m_mouseTileIndex].topLeftX = 450 + ((mouse.MouseX()  - 450)/32) * 32;
      m_tiles[m_mouseTileIndex].topLeftY  = 116 + ((mouse.MouseY() - 116)/32) * 32;
      m_tiles[m_mouseTileIndex].location = 1; //top section
    }
  }
*/

//only need to update moving tile
void CTile::Update(double timeDifference, CGameData &data, CMouse & mouse){
  CAudioManager *pAudio = CAudioManager::Instance();
  CLog *pLog = CLog::Instance();

  static double timer = 0;
  timer += timeDifference;
  if(timer > 1.0){
    m_countDownTimer--;
    if(m_countDownTimer < 0)
      m_countDownTimer = 0;
    if(m_countDownTimer  == 0)
      m_bBonus = false;
    timer = 0;
  }

  //update bling if true
  if(m_bBling == true){
    m_blingTime += timeDifference;    
    if(m_blingTime > 0.1){
      m_bling[0].y -= 10;
      m_bling[1].y -= 8;
      m_bling[2].y -= 6;
      m_bling[3].y -= 4;
      m_blingTime = 0;
      if(m_bling[3].y < -32){
        m_bBling = false;
      }
    }
  }

  //update animation
  if(data.m_bPaused == false){
    for(size_t i = 0; i < m_tiles.size(); ++i){
      if(m_tiles[i].frameMax > 1 && m_tiles[i].frameCount > -1){
      //if(m_tiles[i].bValidTile == true && m_tiles[i].frameMax > 1 && m_tiles[i].frameCount > -1){
        m_tiles[i].animTime += timeDifference;
        if(m_tiles[i].animTime > m_tiles[i].updateInterval){
          m_tiles[i].frameCount++;
          m_tiles[i].animTime = 0;
          if(m_tiles[i].frameCount > m_tiles[i].frameMax - 1){
            m_tiles[i].frameCount = 0;          
          }
        }
      }
    }
  }

  //user is moving a tile with the mouse button down
  if(mouse.MouseX() > 186 && mouse.MouseX() < 825 &&
    mouse.MouseY() > m_areaTop && mouse.MouseY() < 629){
    if(m_bMouseDown == true && m_mouseTileIndex > -1){
      m_tiles[m_mouseTileIndex].topLeftX = (mouse.MouseX() - 186)/32 ;
      m_tiles[m_mouseTileIndex].topLeftY = (mouse.MouseY() - m_areaTop)/32;
      m_tiles[m_mouseTileIndex].location = 0;
      pAudio->MoveNoiseSource(m_mouseTileIndex, 186 + ((mouse.MouseX()  - 186)/32) * 32, m_areaTop + ((mouse.MouseY() - m_areaTop)/32)* 32, m_areaTop + ((mouse.MouseY() - m_areaTop)/32)* 32);
    }
  }

  //display position of tile under mouse cursor in play area
  m_tileInfoIndex = -1;

  //mouse down in play area
  if(mouse.MouseX() > 186 && mouse.MouseX() < 825 && mouse.MouseY() > m_areaTop && mouse.MouseY() < 629){  //162
    int gridX = (mouse.MouseX() - 186)/32;
    int gridY = (mouse.MouseY() - m_areaTop + 8)/32;

    //iterate through all tiles and determine if tile exists in this spot
    for(size_t i = 0; i < m_tiles.size(); ++i){
      if(gridX >= m_tiles[i].topLeftX && gridX < m_tiles[i].topLeftX + 1 &&
        gridY >= m_tiles[i].topLeftY && gridY < m_tiles[i].topLeftY + 1){
        m_tileInfoIndex = i;
        break;
      }
    }
  }


}

void CTile::Render(CGraphics &con, CGameData &data, CMouse & mouse){
  CLog *pLog = CLog::Instance();
  GRAPHIC_IMAGE gi = con.GetSprite(0);

  int frameMax;
  int frameCount;
  double updateInterval;
  double animTime;

  //render all the tiles
  for(size_t i = 0; i < m_tiles.size(); ++i){
    if(m_tiles[i].bValidTile == true){
      gi = con.GetSprite(m_tiles[i].spriteID);

      //tile needs sprite info regarding animation
      if(m_tiles[i].frameCount == -1){
        m_tiles[i].frameMax = gi.frameMax;
        if(m_tiles[i].frameMax > 1){
          m_tiles[i].frameCount = (rand() % m_tiles[i].frameMax) - 1;
          m_tiles[i].animTime = 0;
          m_tiles[i].updateInterval = gi.updateInterval;
        }
      }

      //render tile being dragged around
      if(m_bMouseDown == true && m_mouseTileIndex == i){
        //gi = con.GetSprite(m_tiles[m_mouseTileIndex].spriteID);
        if(m_tiles[i].frameCount > -1)
          gi.frameCount = m_tiles[i].frameCount;
        con.RenderSprite(gi, mouse.MouseX() - m_dx, mouse.MouseY() - m_dy, 255, 255,255);
      }

      //render stationary tiles in play area using row and col
      else if(m_tiles[i].location == 0){//use row and col to render
        if(m_tiles[i].frameCount > -1)
          gi.frameCount = m_tiles[i].frameCount;

        con.RenderSprite(gi, (m_tiles[i].topLeftX * 32) + 186, (m_tiles[i].topLeftY * 32) + m_areaTop - 16, 255, 255,255);
      }

      //render tiles in assembly area
      else if(m_tiles[i].location == 1){//use x and y to render
        if(m_tiles[i].frameCount > -1)
          gi.frameCount = m_tiles[i].frameCount;

        con.RenderSprite(gi, m_tiles[i].topLeftX - 4, m_tiles[i].topLeftY - 16, 255, 255,255);
      }
    }
  }

  //render bling in assembly area
  if(m_bBling == true){
    for(int i = 0; i < 4; ++i){
      gi = con.GetSprite(m_bling[i].spriteID);
      //pLog->Log("bling index", i, m_bling[i].spriteID);
      //pLog->Log("bling x", i, m_bling[i].x);
      //pLog->Log("bling y", i, m_bling[i].y);
      con.RenderSprite(gi, m_bling[i].x, m_bling[i].y, 255, 255, 255);
    }
  }

 //if(m_rightMouseDown == true)
 //if(data.m_bRightMouseDown == true)
 //  con.Print("Here", F_V20, 512, 400, 0, 255, 255, 255);
 ////if(m_TAS.tileCount == true)
 //  con.Print("Test", m_bTest, F_V20, 512, 400, 0, 255, 255, 255);
 //  con.Print("TileCount", m_TAS.tileCount, F_V20, 512, 420, 0, 255, 255, 255);
 //  con.Print("SongID", m_TAS.songID, F_V20, 512, 440, 0, 255, 255, 255);
 //  con.Print("Tile0", m_TAS.tileIndex0, F_V20, 512, 460, 0, 255, 255, 255);
 //  con.Print("Tile1", m_TAS.tileIndex1, F_V20, 512, 480, 0, 255, 255, 255);
 //  con.Print("Tile2", m_TAS.tileIndex2, F_V20, 512, 500, 0, 255, 255, 255);
 //  con.Print("Tile3", m_TAS.tileIndex3, F_V20, 512, 520, 0, 255, 255, 255);

   //con.Print("x", m_tiles[0].topLeftX, F_V20, 512, 540, 0, 255, 255, 255);
   //con.Print("y", m_tiles[0].topLeftY, F_V20, 512, 560, 0, 255, 255, 255);
   //if(m_tileInfoIndex > -1){
   ////  con.Print("x", m_tiles[m_tileInfoIndex].topLeftX, F_V20,  218 + m_tiles[m_tileInfoIndex].topLeftX * 32, m_areaTop  - 40 + m_tiles[m_tileInfoIndex].topLeftY *32, 255, 255, 255, 255);
   ////  con.Print("y", m_tiles[m_tileInfoIndex].topLeftY, F_V20, 218 + m_tiles[m_tileInfoIndex].topLeftX * 32, m_areaTop -20 + m_tiles[m_tileInfoIndex].topLeftY *32, 255, 255, 255, 255);
   //  con.Print("id.", m_tiles[m_tileInfoIndex].songID, F_V20, 218 + m_tiles[m_tileInfoIndex].topLeftX * 32, m_areaTop - 40 + m_tiles[m_tileInfoIndex].topLeftY *32, 255, 255, 0, 255);
   //  con.Print("no.", m_tiles[m_tileInfoIndex].blockNumber, F_V20, 218 + m_tiles[m_tileInfoIndex].topLeftX * 32, m_areaTop - 20 + m_tiles[m_tileInfoIndex].topLeftY *32, 255, 255, 0, 255);
   //}
}



void CTile::ClearTiles(){
  m_tiles.clear();
}

//adds tile info to m_tiles
//also creates unique random position
void CTile::AddTile(TILE_STRUCTURE ts){
  CLog *pLog = CLog::Instance();
  CAudioManager *pAudio = CAudioManager::Instance();
  //pLog->Log("adding tile");
  
  int tx = 0;
  int ty = 0;
  bool bFound = false;
  bool bQuit = false;
  srand(time(NULL));

  while(1){    
    bFound = false;

    //generate random x and y
    tx = rand() % 20;
    ty = rand() % 15;
    //pLog->Log("mypos", tx, ty);

    //is this position already taken?
    for(size_t i = 0; i < m_tiles.size(); ++i){
      if(m_tiles[i].topLeftX == tx && m_tiles[i].topLeftY == ty){
        bFound = true;
        break;
      }
    }
    if(bFound == false)
      break;
  }

  ts.topLeftX = tx;
  ts.topLeftY = ty;
  ts.originalX = tx;
  ts.originalY = ty;
      //pLog->Log("Pos",ts.topLeftX, ts.topLeftY);
      
      //pLog->Log("Sound location",186 + (ts.topLeftX * 32) , 106 + (ts.topLeftY * 32), 106 + (ts.topLeftY * 32));
  m_soundNumber++;
  pAudio->AddNoiseSource(m_soundNumber, ts.soundID, 186 + (ts.topLeftX * 32) + 18, m_areaTop + (ts.topLeftY * 32) + 8, m_areaTop + (ts.topLeftY * 32) + 8, 10, 20, -1, 1.0);
  ts.audioIndex = m_soundNumber;
  m_tiles.push_back(ts);
}

bool CTile::LoadGameLevel(std::string filename){
  CLog *pLog = CLog::Instance();

  std::string value;
  std::string asset;
  std::string parameter;
  TILE_STRUCTURE ts;

  srand(time(NULL));
  m_tiles.clear();
  //ClearTiles();

  //new
  //pLog->Log("Loading game level", filename);
  CCSVReader csv;
  csv.LoadFile(filename);
  //pLog->Log("Level table size", csv.GetTableSize());
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
      ts.blockNumber = atoi(parameter.c_str());      //0-3
      parameter = csv.GetTerm(i, 5);
      ts.maxBlocks = atoi(parameter.c_str());      
      ts.topLeftX = 0;// rand() % 20;// 0;  AddTile will randomize and ensure unique position
      ts.topLeftY = 0;//rand() % 15;//0;
      //ts.type = 1;
      //ts.inPlay = true;
      ts.location = 0; //in play area
      ts.audioIndex = -1;
      ts.bValidTile = true;
      ts.frameMax = 1;
      ts.frameCount = -1; //means sprite data needs to be loaded for the first time.
      ts.updateInterval = 0;
      ts.animTime = 0;
      AddTile(ts);
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
      //ts.type = 2;
      ts.bValidTile = true;
      AddTile(ts);
    }
  }

  //total number of songs
  m_totalSongsInLevel = m_tiles.size()/4;

  return true;
}

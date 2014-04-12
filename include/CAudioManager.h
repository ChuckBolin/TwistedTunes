//****************************************************************************
// Class: CAudioManager
// CAudioManager *pAudio = CAudioManager::Instance();
//****************************************************************************
#ifndef CAUDIOMANAGER_H
#define CAUDIOMANAGER_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>
#include "CCSVReader.h"
#include "bass.h"
#include "CLog.h"

struct AUDIO_INFO{
  unsigned int fileID;
  std::string filename;
};

struct AUDIO_SOUND_DATA{
  unsigned int ID;
  unsigned int fileID;
  DWORD channel;
  BASS_3DVECTOR pos, vel, orient;
	float min; 
	float max;
	int iangle; 
	int oangle; 
	int outvol;
  double duration; //time to play before auto stopping
  double accTime;  //used to accumulate time
  float volume;
  bool bDead;
};

class CAudioManager{
public:
	static CAudioManager *Instance();
  bool LoadAudioData(std::string filename);   
  bool ClearAudioData();
  void PrintAudioInfo();
  void SetSystemFactors(float distf, float rollf, float doppf);
  void SetPlayerPosition(float x, float y, float z);
  void AddNoiseSource(unsigned int ID, unsigned int fileID, float x, float y, float z,
                      float min, float max, double duration, float volume);
  void MoveNoiseSource(unsigned int ID, float x, float y, float z);
  void PlaySounds(unsigned int ID);
  void PauseSound(unsigned int ID);
  void PauseAllSounds();
  void UnPauseAllSounds();
  void StopSound(unsigned int ID);
  void KillSound(unsigned int ID){m_audioSoundData[ID].bDead = true;}
  //void RemoveSound(unsigned int ID); //delete from vector
  //void RemoveFileStream(unsigned int fileID); //use BASS_StreamFree(..)
  void Update(double timeDifference, bool bPaused);
  void StopAll();
  void ResumeAll();
  void FadeOut(unsigned int ID, float newVolume, float timeToFade);

protected:
	CAudioManager();
	~CAudioManager();

private:
	static CAudioManager *pInstance;
  std::vector<AUDIO_INFO> m_audioInfo;            //stores a list of ID and filenumbers
  std::vector<AUDIO_SOUND_DATA> m_audioSoundData; 

  //sound system members
  //player methods
  float m_distanceFactor;
  float m_rolloffFactor;
  float m_dopplerFactor;
  BASS_3DVECTOR m_playerPos, m_playerVel, m_playerFront, m_playerTop;
  
  bool m_bIsValid; //true if initialized


};

#endif


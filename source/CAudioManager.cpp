#include "..\include\CAudioManager.h"

CAudioManager *CAudioManager::pInstance = 0;

CAudioManager *CAudioManager::Instance(){
  if(CAudioManager::pInstance == 0)
    CAudioManager::pInstance = new CAudioManager;

  //else
  return CAudioManager::pInstance;
}

//initialize sound system
CAudioManager::CAudioManager(){
  
  //verify correct version
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
    m_bIsValid = false;
		return;
	}

  //initialize BASS
  if (!BASS_Init(-1,44100,BASS_DEVICE_3D,0,0)){  
		BASS_Free();
    m_bIsValid = false;
    return;
  }	

  //set position
  BASS_Set3DFactors(1,1,1); //use meters
  ::BASS_Set3DPosition(&m_playerPos, 0, 0, 0);
  ::BASS_Apply3D();
  m_bIsValid = true;
}

CAudioManager::~CAudioManager(){
  BASS_Free();
}

bool CAudioManager::LoadAudioData(std::string filename){
  CLog *pLog = CLog::Instance();
  
  if(m_bIsValid == false){
    pLog->Log("Audio is NOT valid!");
    return false;
  }
  pLog->Log("Audio is valid!", filename);
	m_audioInfo.clear();
  m_audioSoundData.clear();
  CCSVReader CSV;
	CSV.LoadFile(filename);
  std::string parameter = "";
  AUDIO_INFO temp;
  pLog->Log("table size", CSV.GetTableSize());

  if(CSV.GetTableSize() > 0){
		for(int i = 0; i < CSV.GetTableSize(); ++i){      
			if(CSV.GetNumberParameters(i) == 3){
        parameter = CSV.GetTerm(i, 0);
        temp.fileID = atoi(parameter.c_str());
        temp.filename = CSV.GetTerm(i,1);
        pLog->Log("Audio file", temp.fileID, temp.filename);
        m_audioInfo.push_back(temp);         
			}
		}
	}
  else{
    return false;  //nothing to load
  }
  return true;
}

bool CAudioManager::ClearAudioData(){
  m_audioInfo.clear();
  return true;
}

//Win32 console only
void CAudioManager::PrintAudioInfo(){
  for(unsigned int i = 0; i < m_audioInfo.size();++i){
    std::cout << m_audioInfo[i].fileID << ", " << m_audioInfo[i].filename << std::endl;
  }
}

void CAudioManager::SetSystemFactors(float distf, float rollf, float doppf){
  m_distanceFactor = distf;
  m_rolloffFactor  = rollf;
  m_dopplerFactor  = doppf;
  ::BASS_Set3DFactors(distf, rollf, doppf);
  ::BASS_Apply3D();
}

void CAudioManager::SetPlayerPosition(float x, float y, float z){
  m_playerPos.x = x;
  m_playerPos.y = y;
  m_playerPos.z = z;
  ::BASS_Set3DPosition(&m_playerPos, 0, 0, 0);
  ::BASS_Apply3D();
}

void CAudioManager::AddNoiseSource(unsigned int ID, unsigned int fileID, float x, float y, float z,
                                   float min, float max, double duration, float volume){
  CLog *pLog = CLog::Instance();
  if(m_bIsValid == false)
    return;
  AUDIO_SOUND_DATA temp;
  bool bFound = false;
  std::string filename = "";

  //look for corresponding filename
  for(unsigned int i = 0; i < m_audioInfo.size(); ++i){
    if(m_audioInfo[i].fileID == fileID){
      bFound = true;
      filename = "assets\\sounds\\" + m_audioInfo[i].filename;
      //pLog->Log("noise source added ",filename);
      //filename = "sounds\\" + m_audioInfo[i].filename;
      break;
    }
  }
  if(bFound == false)
    return;

  temp.ID = ID;
  temp.fileID = fileID;
  temp.pos.x = x;
  temp.pos.y = y;
  temp.pos.z = z;
  temp.min = min;
  temp.max = max;
  temp.duration = duration;
  temp.accTime = 0;
  temp.volume = volume;
  temp.bDead = false;
    
  m_audioSoundData.push_back(temp);

  unsigned int size = m_audioSoundData.size();

  if (m_audioSoundData[size - 1].channel = BASS_StreamCreateFile(FALSE, filename.c_str(),0,0, BASS_SAMPLE_LOOP | BASS_SAMPLE_3D)){ 
    ::BASS_ChannelSet3DAttributes(m_audioSoundData[size - 1].channel,BASS_3DMODE_NORMAL, min, max, 360, 360, 1);
    ::BASS_ChannelSet3DPosition(m_audioSoundData[size - 1].channel, &m_audioSoundData[size - 1].pos,0, 0);
    ::BASS_ChannelSetAttribute(m_audioSoundData[size - 1].channel,BASS_ATTRIB_VOL, volume);
    ::BASS_Apply3D();
    pLog->Log("Successfully added noise source", temp.ID);
	} 
  
  else{
    //remove because it did not load
    if(m_audioSoundData.size() > 0)
      m_audioSoundData.pop_back();
    pLog->Log("Failed to add noise source", temp.ID, filename);
  }
}

void CAudioManager::PlaySounds(unsigned int ID){
 
  CLog *pLog = CLog::Instance();
  //pLog->Log("A");
  if(m_bIsValid == false){
    //pLog->Log("Playsounds", ID);
    return;
  }
  //pLog->Log("Size***********************", m_audioSoundData.size());
  for(unsigned int i = 0; i < m_audioSoundData.size(); ++i){
    //pLog->Log("Sound ID", i);
    if(ID == m_audioSoundData[i].ID){
      //pLog->Log("Playsound a");
      if(BASS_ChannelIsActive(m_audioSoundData[i].channel)!= BASS_ACTIVE_PLAYING)
        //pLog->Log("Playsound b");
        BASS_ChannelPlay(m_audioSoundData[i].channel,FALSE);    
      return;
    }
  }  
}

void CAudioManager::PauseSound(unsigned int ID){
  if(m_bIsValid == false)
    return;

  for(unsigned int i = 0; i < m_audioSoundData.size(); ++i){
    if(ID == m_audioSoundData[i].ID){
      if(BASS_ChannelIsActive(m_audioSoundData[i].channel)!= BASS_ACTIVE_PLAYING)
        BASS_ChannelPause(m_audioSoundData[i].channel);    
      return;
    }
  }  
}

void CAudioManager::PauseAllSounds(){
  if(m_bIsValid == false)
    return;

  for(unsigned int i = 0; i < m_audioSoundData.size(); ++i){
    //if(ID == m_audioSoundData[i].ID){
      //if(BASS_ChannelIsActive(m_audioSoundData[i].channel)!= BASS_ACTIVE_PLAYING)
        BASS_ChannelPause(m_audioSoundData[i].channel);    
      //return;
    //}
  }  
}

void CAudioManager::UnPauseAllSounds(){
  if(m_bIsValid == false)
    return;

  for(unsigned int i = 0; i < m_audioSoundData.size(); ++i){
    //if(ID == m_audioSoundData[i].ID){
      //if(BASS_ChannelIsActive(m_audioSoundData[i].channel)!= BASS_ACTIVE_PLAYING)
        BASS_ChannelStop(m_audioSoundData[i].channel);    
      //return;
    //}
  }  
}

void CAudioManager::StopSound(unsigned int ID){
  if(m_bIsValid == false)
    return;

  for(unsigned int i = 0; i < m_audioSoundData.size(); ++i){
    if(ID == m_audioSoundData[i].ID){
      if(BASS_ChannelIsActive(m_audioSoundData[i].channel)== BASS_ACTIVE_PLAYING)
        BASS_ChannelStop(m_audioSoundData[i].channel);    
      return;
    }
  }    
}

//Update looks for duration not -1 and increments by timeDifference amount
//If the accumulated amount is greater than duration, the sound is stopped
void CAudioManager::Update(double timeDifference, bool bPaused){
  


  float volume;
  float distance;
  CLog *pLog = CLog::Instance();
  //pLog->Log("audio sound data size", m_audioSoundData.size());
  for(unsigned int i = 0; i < m_audioSoundData.size(); ++i){
    
    //some sounds only play for specified amounts of time
    if(m_audioSoundData[i].duration != - 1){
      m_audioSoundData[i].accTime += timeDifference;
      if(m_audioSoundData[i].accTime > timeDifference){
        m_audioSoundData[i].accTime = 0;
        StopSound(m_audioSoundData[i].ID);
      }
    }

    //distance between player and noise source
    distance = sqrt((m_playerPos.x + 7 - m_audioSoundData[i].pos.x) * (m_playerPos.x + 7 - m_audioSoundData[i].pos.x) +
                    (m_playerPos.y + 2 - m_audioSoundData[i].pos.y) * (m_playerPos.y + 2 - m_audioSoundData[i].pos.y) +
                    (m_playerPos.z - m_audioSoundData[i].pos.z) * (m_playerPos.z - m_audioSoundData[i].pos.z));
                    
    //if(distance < m_audioSoundData[i].max){
    //  pLog->Log("Distance", i, distance, m_audioSoundData[i].max);

    //}
    //start playing if within range
    if(bPaused == false){
      if(BASS_ChannelIsActive(m_audioSoundData[i].channel)!= BASS_ACTIVE_PLAYING && distance < m_audioSoundData[i].max && m_audioSoundData[i].bDead == false){      
        PlaySounds(m_audioSoundData[i].ID);
        BASS_ChannelSlideAttribute(m_audioSoundData[i].channel, BASS_ATTRIB_VOL, m_audioSoundData[i].volume, 500);
      }
    }

    //fade out if outside of range
    if(bPaused == false){
      if(BASS_ChannelIsActive(m_audioSoundData[i].channel)== BASS_ACTIVE_PLAYING && BASS_ChannelIsSliding(m_audioSoundData[i].channel,BASS_ATTRIB_VOL) == false && distance > m_audioSoundData[i].max){
        FadeOut(m_audioSoundData[i].ID, -0.1, 100); //-0.1 causes the sound to 'stop'...so it can be replayed
      }
    }
  } 
}

void CAudioManager::MoveNoiseSource(unsigned int ID, float x, float y, float z){
  if(m_bIsValid == false)
    return;
  if(ID > 0 && ID < m_audioSoundData.size()){
    m_audioSoundData[ID].pos.x = x;
    m_audioSoundData[ID].pos.y = y;
    m_audioSoundData[ID].pos.z = z;
  }
}

void CAudioManager::StopAll(){
  ::BASS_Stop();
}

void CAudioManager::ResumeAll(){
  ::BASS_Start();
}

void CAudioManager::FadeOut(unsigned int ID, float newVolume, float timeToFade){
  for(unsigned int i = 0; i < m_audioSoundData.size(); ++i){
    if(m_audioSoundData[i].ID == ID){
      if(BASS_ChannelIsActive(m_audioSoundData[i].channel)== BASS_ACTIVE_PLAYING && BASS_ChannelIsSliding(m_audioSoundData[i].channel,BASS_ATTRIB_VOL) == false){
        //-1 stops channel when
        ::BASS_ChannelSlideAttribute(m_audioSoundData[i].channel,BASS_ATTRIB_VOL, newVolume, timeToFade);//  //BASS_ATTRIB_MUSIC_VOL_CHAN  //-1
      }
    }
  }
}
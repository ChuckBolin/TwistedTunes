#include "..\include\CEffect.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pd3d){
  CLog *pLog = CLog::Instance();

  m_pd3dDevice = pd3d;
	g_pParticleLibrary = CreateParticleLibrary(PYRO_SDK_VERSION);
  pPyroGraphics = NULL;
	pPyroGraphics = new PyroParticles::CGraphicsDevice_D3D9(m_pd3dDevice, g_pParticleLibrary->GetMaxRenderParticleQuads(), FALSE /* 3D */);
	g_pParticleLibrary->Init(pPyroGraphics);
}

CEffect::CEffect(){
 
}

void CEffect::Init(LPDIRECT3DDEVICE9 pd3d){
  
  m_pd3dDevice = pd3d;
	g_pParticleLibrary = CreateParticleLibrary(PYRO_SDK_VERSION);
  pPyroGraphics = NULL;
	pPyroGraphics = new PyroParticles::CGraphicsDevice_D3D9(m_pd3dDevice, g_pParticleLibrary->GetMaxRenderParticleQuads(), FALSE /* 3D */);
	g_pParticleLibrary->Init(pPyroGraphics);
}

CEffect::~CEffect(){
}

bool CEffect::LoadFile(std::string file){
  CLog *pLog = CLog::Instance();

  CCSVReader csv;
  csv.LoadFile(file);
  
  EFFECT eff;
  m_effect.clear();
  for(size_t i = 0; i < csv.GetTableSize(); ++i){
    m_effect.push_back(eff);
    m_effect[i].pPyroFile = g_pParticleLibrary->LoadPyroFile(csv.GetTerm(i, 1).c_str());
    m_effect[i].pPyroFile->CreateTextures();
    m_effect[i].pEmitter = const_cast<PyroParticles::IPyroParticleEmitter*>(m_effect[i].pPyroFile->GetFileEmitter(0));
    m_effect[i].pEmitterName = m_effect[i].pEmitter->GetName();
    m_effect[i].Time = 0.0f;
    m_effect[i].Phase = -100.0f;
    m_effect[i].g_pEmitters = new PyroParticles::CEmitterList();
  }

  return true;
}

void CEffect::Delete(){

  for(size_t i = 0; i < m_effect.size(); ++i){
    m_effect[i].pPyroFile->DestroyTextures();
    delete m_effect[i].g_pEmitters;
    delete m_effect[i].pPyroFile;
  }

	g_pParticleLibrary->Done();
	DestroyParticleLibrary(g_pParticleLibrary);
	delete pPyroGraphics;
  
}


void CEffect::Add(size_t index, float x, float y, float scale){
  //CLog *pLog = CLog::Instance();
  if(index < 0 || index > m_effect.size() - 1)
    return;

  //only allow 15 effects
  if(index > 15)
    return;

  if(m_effect[index].pPyroFile != NULL){
    m_effect[index].pEmitter->SetScale(scale);
    m_effect[index].g_pEmitters->Append(m_effect[index].pPyroFile, m_effect[index].pEmitterName, m_effect[index].Time, x / scale, y /scale);

  }
  
}

void CEffect::Update(double timeDifference){

  //iterate through all single effects (no streams in this loop
  //***************************************************************
  for(size_t i = 0; i < m_effect.size(); ++i){
    m_effect[i].PrevTime = m_effect[i].Time;
    m_effect[i].Time = static_cast<float>((timeGetTime() - timeDifference) * 0.005f);  //adjust value to speed up effect
    m_effect[i].TimeDelta = m_effect[i].Time - m_effect[i].PrevTime;
    m_effect[i].CurTime = m_effect[i].PrevTime;

    while (m_effect[i].TimeDelta > 0.0f)
    {
      float SafeTimeDelta = (m_effect[i].TimeDelta >= 0.05f) ? 0.05f : m_effect[i].TimeDelta;
      m_effect[i].g_pEmitters->Move(m_effect[i].CurTime, SafeTimeDelta);
      m_effect[i].g_pEmitters->Prepare(m_effect[i].CurTime, SafeTimeDelta);
      m_effect[i].CurTime += SafeTimeDelta;
      m_effect[i].TimeDelta -= SafeTimeDelta;
    }
  }

  //iterate through streams
  //***************************
  float x = 0;
  float y = 0;
  for(size_t i = 0; i < m_stream.size(); ++i){

    //process only if active
    if(m_stream[i].active == true){
      m_stream[i].time += timeDifference;
      
      //time to add another
      if(m_stream[i].time > 0.05){
        m_stream[i].time = 0;
        m_stream[i].count++;
        
        //can't add more particles...we're done with this one
        if(m_stream[i].count > m_stream[i].number){
          m_stream[i].active = false;
        }
        //calculate another position and add
        else{
          x = m_stream[i].x + cos(m_stream[i].angle) * m_stream[i].velocity * (float)m_stream[i].count;
          y = m_stream[i].y + sin(m_stream[i].angle) * m_stream[i].velocity * (float)m_stream[i].count;
          Add(m_stream[i].effectType, x, y, m_stream[i].scale);   
        }

      }
    }
  }
}

void CEffect::Render(){
  for(size_t i = 0; i < m_effect.size(); ++i){
    m_effect[i].g_pEmitters->Render(m_effect[i].Time);
    m_effect[i].g_pEmitters->RemoveInactive(m_effect[i].Time, 0.05f);
  }
}

int CEffect::GetEmitterCount(size_t index){
  if(index < 0 || index > m_effect.size() - 1)
    return 0;

  return m_effect[index].g_pEmitters->GetCount();
}

void CEffect::Reset(){
 
}

size_t CEffect::GetNumberEffects(){
  return m_effect.size();

}

//*************************************************************************
// Creates stream of smoke like a rocket
// Adds particle at x, y
// Moves in direction (angle) and speed (velocity) and adds another particle
//*************************************************************************
void CEffect::AddEffectsStream(int effectType, float x, float y, float angle, float velocity, int numberParticles, float scale){
  EFFECT_STREAM temp;
  temp.effectType = effectType;
  temp.x = x;
  temp.y = y;
  temp.angle = angle;
  temp.velocity = velocity;
  temp.number = numberParticles;
  temp.time = 0;
  temp.active = true;
  temp.count = 1;
  temp.scale = scale;

  m_stream.push_back(temp);

  Add(effectType, x, y, scale);   
}

//code to create rocket smoke effect
/*
       //scaling
        //***********************
        static float scale = 0;
        scale = 0.5;
        pEmitter->SetScale(scale);

        //add at fixed interval while moving
        //***********************************
        static float timeX = 0;
        static float xPos = 10;
        static float yPos = 300;

        if(g_bReset == true){
          g_bReset = false;
          xPos = 10;
          yPos = 300;
        }

        timeX += 0.1f;
        if(timeX > 0.1){
          g_bAddEmitter = true;
          g_AddPosition = GetMousePos();
          g_AddPosition.x /= scale;
          g_AddPosition.y /= scale;
          xPos += 15;
          yPos += 1;
          g_pEmitters->Append(pPyroFile, pEmitterName, Time, xPos, yPos);
          timeX = 0;
        }
        */
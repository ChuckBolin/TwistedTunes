#ifndef CEFFECT_H
#define CEFFECT_H

#include <windows.h>
#include <string>
#include <vector>
#include "Pyro.h"
#include "PyroD3D9.h"
#include "EmitterList.h"
#include "CLog.h"
#include "CCSVReader.h"

class CEffect{
public:

  struct EFFECT{
    PyroParticles::CEmitterList *g_pEmitters;
    PyroParticles::IPyroFile *pPyroFile; 
	  PyroParticles::IPyroParticleEmitter *pEmitter;
	  const char *pEmitterName;  
    float Time;
	  float Phase;  
    float PrevTime;
    float TimeDelta;
    float CurTime;
  }; 

  CEffect(LPDIRECT3DDEVICE9 pd3d);
  CEffect();
  void Init(LPDIRECT3DDEVICE9 pd3d);
  ~CEffect();
  bool LoadFile(std::string file);
  void Add(size_t index, float x, float y, float scale);
  void Update(double timeDifference);
  void Render();
  void Delete();
  int GetEmitterCount(size_t index);
  void Reset();
  size_t GetNumberEffects();
  void AddEffectsStream(int effectType, float x, float y, float angle, float velocity, int numberParticles, float scale);

  PyroParticles::IPyroParticleLibrary *g_pParticleLibrary;
  PyroParticles::PyroGraphics::IDevice *pPyroGraphics;
  LPDIRECT3DDEVICE9       m_pd3dDevice;

private:
  std::vector<EFFECT> m_effect;

  //effect streams
  struct EFFECT_STREAM{
    int effectType;
    float x;
    float y;
    float angle;
    float velocity;
    int number;
    double time;
    bool active;
    int count;
    float scale;
  };
  
  std::vector<EFFECT_STREAM> m_stream;

};

#endif
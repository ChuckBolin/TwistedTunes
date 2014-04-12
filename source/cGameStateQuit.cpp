#include "..\include\CGameStateQuit.h"

CGameStateQuit::CGameStateQuit(void){
}

CGameStateQuit::~CGameStateQuit(void)
{
	m_TE.clear();
}

void CGameStateQuit::Initialize(){

}

void CGameStateQuit::Activate(CGameData &data, CConfigData &cfg, CGraphics &con){
}

CGameStateObject* CGameStateQuit::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;
 

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

int CGameStateQuit::Render(CGraphics &con,  CGameData &data, CConfigData &cfg){
 
  return 0;
}

void CGameStateQuit::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void CGameStateQuit::Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con){
}
void CGameStateQuit::Resume(){
}
void CGameStateQuit::Pause(){
}
void CGameStateQuit::Save(){
}

void CGameStateQuit::AddTransitionEvent(int event, CGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

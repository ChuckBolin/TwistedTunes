//CTimer.cpp
//Need to link to winmm.lib because of timeGetTime()
#include "../include/CTimer.h"

CTimer::CTimer(){}

CTimer::~CTimer(){}

__int64 CTimer::getFrequency(){
  return cntsPerSec;  //3,579,545 Hz
}

//Added CB 11.17.07 to allow user to reset timer
void CTimer::ResetTimer(){
  //performed if performance counter is available
  if(counterAvailable == true){
	  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  }
  else{
    currTime = ::timeGetTime();//system time in milliseconds
  }

  lastTime = currTime;
}


bool CTimer::getTimer(double timeDiff){
  //performed if performance counter is available
  if(counterAvailable == true){
	  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  }
  else{
    currTime = ::timeGetTime();//system time in milliseconds
  }

  tempTime = lastTime;
  deltaTime = (double)(currTime - lastTime) * timeScale ;
  timeElapsed += deltaTime;
 
  if(timeElapsed >= timeDiff){
    timeElapsed = 0.0f;
    lastTime = currTime;
    return true;
  }

  lastTime = currTime;
  return false;
}

//returns time difference since last call
double CTimer::getTimeDifference(){
  
  //performed if performance counter is available
  if(counterAvailable == true){
	  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  }
  else{
    currTime = ::timeGetTime();//system time in milliseconds
  }

  tempTime = lastTime;
  deltaTime = (double)(currTime - lastTime) * timeScale ;
  lastTime = currTime;  
  return deltaTime;
 
}

//returns true if accumulated time is greater than 1 second
bool CTimer::secondTimer(){
  //performed if performance counter is available
  if(counterAvailable == true){
	  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  }
  else{
    currTime = ::timeGetTime();//system time in milliseconds
  }

  tempTime = lastTime;
  deltaTime = (double)(currTime - lastTime) * timeScale ;
  secondsElapsed += deltaTime;
 
  if(secondsElapsed >= 1){
    secondsElapsed = 0.0f;
    lastTime = currTime;
    return true;
  }

  lastTime = currTime;
  return false;
}

//initialize timer
bool CTimer::Initialize(){
	cntsPerSec = 0;
  currTime = 0; 
	bool perfExists = QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec)!=0;
  //perfExists = false; //enable this line to test timeGetTime() which is a 
                        //backup to lack of performance counter
  if(!perfExists){
    counterAvailable = false;
    lastTime = ::timeGetTime();
    timeScale = 0.001;
  }
  else{
    counterAvailable = true;
    timeScale = 1.0 / (double)cntsPerSec;
    ::QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
  }

	timeElapsed = 0.0f;
  secondsElapsed = 0.0f;

  return true;
}

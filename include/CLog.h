/*
  CLog.h - Singleton design pattern. Written by Chuck Bolin, January 16, 2008
  Purpose: Provides variety of logging abilities.

  Updated: January 18, 2009

  Initialization:

    CLog *pLog = CLog::Instance();
*/
#ifndef CLOG_H
#define CLOG_H

#include <fstream>
#include <cstdarg>
#include "CDateTime.h"
#include <sstream>

class CLog{
public:
  static CLog *Instance();
  void SetFile(std::string sFile);
  void SetDelimiter(char delimiter);
  template <typename T> void Log(T value);
  template <typename T1, typename T2> void Log(T1 value1, T2 value2);
  template <typename T1, typename T2> void Logx(T1 value1, T2 value2); 
  template <typename T1, typename T2, typename T3>
  void Log(T1 value1, T2 value2, T3 value3);  
  template <typename T1, typename T2, typename T3, typename T4> 
  void Log(T1 value1, T2 value2, T3 value3, T4 value4);
  void LogSeries(int num, char typecode, ...);
  void LogDate();
  void LogTime();
  template <typename T1, typename T2>void LogPair(T1 value1, T2 value2); 

protected:
  CLog();
  ~CLog();

private:
  static CLog* pInstance;
  std::string m_filename;
  char m_delimiter;
};

//saves any data type to a file
template <typename T> 
void CLog::Log(T value){
  std::ofstream file;
  file.open(m_filename.c_str(), std::ios_base::app);
  
  //std::cout << "Log: " << typeid(value).name() << endl;
  if(file){
    file << value << std::endl;
  }

  file.close();  
}

//saves two data types to a file
template <typename T1, typename T2> 
void CLog::Log(T1 value1, T2 value2){
  std::ofstream file;
  file.open(m_filename.c_str(), std::ios_base::app);
  
  if(file){
    file << value1 << m_delimiter << value2 << std::endl;
  }

  file.close();  
}

//saves two data types to a file
template <typename T1, typename T2> 
void CLog::Logx(T1 value1, T2 value2){
  std::ofstream file;
  file.open(m_filename.c_str(), std::ios_base::app);
  
  if(file){
    file << value1 << m_delimiter <<  std::hex << std::showbase << value2 << std::endl;
  }

  file.close();  
}

template <typename T1, typename T2, typename T3>
void CLog::Log(T1 value1, T2 value2, T3 value3){
  std::ofstream file;
  file.open(m_filename.c_str(), std::ios_base::app);
  
  if(file){
    file << value1 << "(" 
         << value2 << ", "
         << value3 << ")" << std::endl;
  }

  file.close();  
}


template <typename T1, typename T2, typename T3, typename T4> 
void CLog::Log(T1 value1, T2 value2, T3 value3, T4 value4){
  std::ofstream file;
  file.open(m_filename.c_str(), std::ios_base::app);
  
  if(file){
    file << value1 << "(" 
         << value2 << ", "
         << value3 << ", "
         << value4 << ")" << std::endl;
  }

  file.close();  
}

template <typename T1, typename T2>
void CLog::LogPair(T1 value1, T2 value2){
  std::ofstream file;
  file.open(m_filename.c_str(), std::ios_base::app);
  
  if(file){
    file << "Key \"" << value1 << "\" = " << value2 << std::endl;
  }

  file.close();  
}

#endif



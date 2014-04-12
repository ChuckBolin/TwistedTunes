#include "..\include\CLog.h"

CLog *CLog::pInstance = 0;

CLog::CLog(){
  m_filename = "logfile.txt";
  m_delimiter = ',';
}

void CLog::SetDelimiter(char delimiter){
  m_delimiter = delimiter;
}

void CLog::LogDate(){
  CDateTime dt;
  Log("Date", dt.GetLocalTime("MM.DD.YYYY"));
}
 
void CLog::LogTime(){
  CDateTime dt;
  Log("Time", dt.GetLocalTime("HH:MM:SS"));
}

void CLog::SetFile(std::string sFile){
  m_filename = sFile;
}

CLog *CLog::Instance(){
  if(CLog::pInstance == 0)
    CLog::pInstance = new CLog;

  //else
  return CLog::pInstance;
}

CLog::~CLog(){
  delete pInstance;
}

/*
LogSeries - Allows a series of the same data type
to be saved to file. Legal data types are 
(i) int, (f) float and (d) double. 'num' indicates
how many variable arguments are in the arg list.
  
  Ex: //pLog->LogSeries(4,'i',12,34,54,67);
*/
void CLog::LogSeries(int num, char typecode, ...){
  std::ofstream file;
  file.open(m_filename.c_str(), std::ios_base::app);
   
  if(file){
    va_list ap;  
    
    va_start(ap, num);
    std::ostringstream oss;

    for(int i = 1; i <= num; ++i){
      if(typecode == 'i')
        oss << va_arg(ap, int);
      else if(typecode == 'f')
        oss << va_arg(ap, float);
      else if(typecode == 'd')
        oss << va_arg(ap, double);

      if(i < num)
        oss << m_delimiter;//", ";

    }
    va_end(ap);
    oss << std::endl;
    file << oss.str();
    
  }

  file.close();  
}

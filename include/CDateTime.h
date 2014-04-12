/*File: CDateTime.h  Author: 1.16.2008
  Purpose: Manages all date and time information.

  //Iplementation:

  CDateTime dt;
  cout << dt.GetDayOfWeekName() << endl;
  cout << dt.GetLocalTime("MM/DD/YY") << endl;
  cout << dt.GetLocalTime("MM-DD-YYYY") << endl;
  cout << dt.GetLocalTime("MM.DD.YYYY") << endl;
  cout << dt.GetLocalTime("MONTH D, YYYY") << endl;
  cout << dt.GetLocalTime("HH:MM") << endl;
  cout << dt.GetLocalTime("HH:MM:SS") << endl;
  cout << dt.GetMonthName() << endl;

  //use this to eliminate warnings for using deprecated localtime ...   _CRT_SECURE_NO_WARNINGS added to Project, Properties, C++, Preprocessor

*/
#ifndef CDATETIME_H
#define CDATETIME_H

#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

class CDateTime{
public:
  CDateTime();
  std::string GetLocalTime(std::string filter);
  std::string GetGMTime();
  std::string GetMonthName();
  std::string GetDayOfWeekName();
  int GetDay();
  int GetMonth();
  int GetYear();
  int GetHour();
  int GetMinute();
  int GetSecond();

private:
  char * m_dayOfWeekName[7];
  char * m_monthName[12];
};

#endif
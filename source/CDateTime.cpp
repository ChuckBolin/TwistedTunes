#include "..\include\CDateTime.h"

CDateTime::CDateTime(){
  m_dayOfWeekName[0] = "Sunday";
  m_dayOfWeekName[1] = "Monday";
  m_dayOfWeekName[2] = "Tuesday";
  m_dayOfWeekName[3] = "Wednesday";
  m_dayOfWeekName[4] = "Thursday";
  m_dayOfWeekName[5] = "Friday";
  m_dayOfWeekName[6] = "Saturday";

  m_monthName[0] = "January";
  m_monthName[1] = "February";
  m_monthName[2] = "March";
  m_monthName[3] = "April";
  m_monthName[4] = "May";
  m_monthName[5] = "June";
  m_monthName[6] = "July";
  m_monthName[7] = "August";
  m_monthName[8] = "September";
  m_monthName[9] = "October";
  m_monthName[10] = "November";
  m_monthName[11] = "December";

}
std::string CDateTime::GetLocalTime(std::string filter){
  std::string sOutput;
  std::ostringstream oss;

  if(filter == "MM/DD/YY"){
    oss << std::setfill ('0') << std::setw(2);
    oss << GetMonth() << "/" ;
    oss << std::setfill ('0') << std::setw(2);
    oss << GetDay() << "/";
    oss << std::setfill ('0') << std::setw(2);
    oss << (GetYear() - 2000);
    sOutput = oss.str();
    return sOutput;
  }
  else if(filter == "MM-DD-YYYY"){
    oss << std::setfill ('0') << std::setw(2);
    oss << GetMonth() << "/" ;
    oss << std::setfill ('0') << std::setw(2);
    oss<< GetDay() << "/" << GetYear();
    sOutput = oss.str();
    return sOutput;
  }
  else if(filter == "MM.DD.YYYY"){
    oss << std::setfill ('0') << std::setw(2);
    oss << GetMonth() << "." ;
    oss << std::setfill ('0') << std::setw(2);
    oss<< GetDay() << "." << GetYear();
    sOutput = oss.str();
    return sOutput;
  }
  else if(filter == "HH:MM"){
    oss << std::setfill ('0') << std::setw(2);
    oss << GetHour() << ":" ;
    oss << std::setfill ('0') << std::setw(2);
    oss<< GetMinute();
    sOutput = oss.str();
    return sOutput;
  }
  else if(filter == "HH:MM:SS"){
    oss << std::setfill ('0') << std::setw(2);
    oss << GetHour() << ":" ;
    oss << std::setfill ('0') << std::setw(2);
    oss<< GetMinute() << ":";
    oss << std::setfill ('0') << std::setw(2);
    oss<< GetSecond();
    sOutput = oss.str();
    return sOutput;
  }
  else if(filter == "MONTH D, YYYY"){
    oss << GetMonthName() << " ";
    oss << GetDay() << ", " ;
    //oss << std::setfill ('0') << std::setw(2);
    oss<< GetYear();
    sOutput = oss.str();
    return sOutput;
  }

  return "";
}

std::string CDateTime::GetGMTime(){
  return "";
}

std::string CDateTime::GetMonthName(){
  time_t rawtime;
  time(&rawtime);
  tm * timeinfo;
  //errno_t err;
  timeinfo = localtime(&rawtime);
  //err = localtime_s(&timeinfo, &rawtime);
  
  
  return m_monthName[timeinfo->tm_mon];  
}

std::string CDateTime::GetDayOfWeekName(){
  time_t rawtime;
  time(&rawtime);
  tm * timeinfo;
  timeinfo = localtime(&rawtime);
  
  return m_dayOfWeekName[timeinfo->tm_wday];  
}

int CDateTime::GetDay(){
  time_t rawtime;
  time(&rawtime);
  tm * timeinfo;
  timeinfo = localtime(&rawtime);
  return timeinfo->tm_mday;
}

int CDateTime::GetMonth(){
  time_t rawtime;
  time(&rawtime);
  tm * timeinfo;
  timeinfo = localtime(&rawtime);
  return timeinfo->tm_mon + 1;
}

int CDateTime::GetYear(){
  time_t rawtime;
  time(&rawtime);
  tm * timeinfo;
  timeinfo = localtime(&rawtime);
  return timeinfo->tm_year + 1900;
}

int CDateTime::GetHour(){
  time_t rawtime;
  time(&rawtime);
  tm * timeinfo;
  timeinfo = localtime(&rawtime);
  return timeinfo->tm_hour;
}

int CDateTime::GetMinute(){
  time_t rawtime;
  time(&rawtime);
  tm * timeinfo;
  timeinfo = localtime(&rawtime);
  return timeinfo->tm_min;
}

int CDateTime::GetSecond(){
  time_t rawtime;
  time(&rawtime);
  tm * timeinfo;
  timeinfo = localtime(&rawtime);
  return timeinfo->tm_sec;  
}

//  char * m_dayOfWeekName[7];
//  char * m_monthName[12];

/*
 
 
  time_t rawtime;
  time ( &rawtime );
  cout << ctime(&rawtime) << endl;
  
  tm * ptm;
  ptm = gmtime(&rawtime);
  cout << (ptm->tm_hour)%24 << ":" << (ptm->tm_min) << endl;

  tm * timeinfo;
  timeinfo = localtime(&rawtime);
  cout << (timeinfo->tm_hour)%24 << ":" << (timeinfo->tm_min) << endl;

  cout << asctime(timeinfo) << endl;
   
  char * weekday[] = { "Sunday", "Monday",
                       "Tuesday", "Wednesday",
                       "Thursday", "Friday", "Saturday"};

  cout << weekday[timeinfo->tm_wday] << endl;

  time_t seconds;
  seconds = time (NULL);
  cout << seconds/3600 << " hours since Jan 1, 1970 " << endl;



*/
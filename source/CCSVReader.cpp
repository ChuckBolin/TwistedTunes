#include "..\include\CCSVReader.h"

CCSVReader::CCSVReader(){
  m_tableSize = 0;
  m_numberParameters = 0;
}

size_t CCSVReader::GetNumberParameters(size_t row){
  if(row < 0 || row > m_table.size())
    return 0;
  m_numberParameters = m_table[row].line.size();
  return m_numberParameters;
}

void CCSVReader::LoadFile(std::string filename){
  PARAMETERS temp;
  std::string par;
  size_t pos = 0;
  CLog *pLog = CLog::Instance();

  std::ifstream file(filename.c_str());
  std::string sLine;
  if(file){
    
    while(!file.eof()){
      temp.line.clear();
      pos = 0;
      getline(file, sLine);
      
      //remove spaces and comments
      sLine = RemoveSpaces(sLine);
      sLine = RemoveComments(sLine);
            
      //find commas
      if(sLine.size() > 0){
        //pLog->Log(sLine);
        for(size_t i = 0; i < sLine.size(); ++i){
          if(sLine.substr(i, 1) == ","){
            par = sLine.substr(pos, i - pos);
            par = RemoveSpaces(par);
            temp.line.push_back(par);
            pos = i + 1;
          }
        }
        par = sLine.substr(pos, sLine.size() - pos);
        temp.line.push_back(par);
        
        m_table.push_back(temp);
      }
    }
  }
  file.close();
  m_tableSize = m_table.size();
}

std::string CCSVReader::GetTerm(size_t row, size_t col){
  std::string temp = "";  

  if(row < 0 || row > m_table.size())
    return temp;
  
  if(col < 0 || col > m_table[row].line.size())
    return temp;

  temp = m_table[row].line[col];

  return temp;

}

//removes all spaces from a string
std::string CCSVReader::RemoveSpaces(std::string in){
  std::string temp;
  
  for(size_t i = 0; i < in.size(); i++){
    if(in.substr(i, 1) != " "){
      temp = temp + in.substr(i,1);      
    }
  }

  return temp;
}

//removes all comments from a string //
std::string CCSVReader::RemoveComments(std::string in){
  std::string temp;

  for(size_t i = 0; i < in.size(); i ++){
    if(in.substr(i,2) != "//")
      temp = temp + in.substr(i, 1);
    else
      return temp;
  }
  return temp;
}
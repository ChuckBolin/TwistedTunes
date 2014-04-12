#include "..\include\CFileReader.h"

CFileReader::CFileReader(){
}

CFileReader::CFileReader(std::string filename){
  m_Filename = filename;
  std::string line;
  m_NumberOfLines = 0;
  
  std::ifstream file(filename.c_str());
  if(file.is_open()){
    m_bFileExists = true;

    while(!file.eof()){
      getline (file, line);
      line = RemoveSpaces(line);
      line = RemoveComments(line);

      if(line.size() != 0){
        m_NumberOfLines++;
        m_Lines.push_back(line);
      }
    }

    file.close();
  }
  else
    m_bFileExists = false;

}

void CFileReader::LoadFile(std::string filename){
  m_Filename = filename;
  std::string line;
  m_NumberOfLines = 0;
  m_Lines.clear();
  
  std::ifstream file(filename.c_str());
  if(file.is_open()){
    m_bFileExists = true;

    while(!file.eof()){
      getline (file, line);

      if(m_bSupressSpaces == false)
        line = RemoveSpaces(line);

      line = RemoveComments(line);

      if(line.size() != 0){
        m_NumberOfLines++;
        m_Lines.push_back(line);
      }
    }

    file.close();
  }
  else
    m_bFileExists = false;

}

//returns number of lines...without comments, blank lines
int CFileReader::GetNumberOfLines(){
  return m_NumberOfLines;
}


//returns number of lines in the file
std::string CFileReader::GetLineFromFile(int i){
  if(i < 0 || i > m_NumberOfLines)
    return "";

  return m_Lines[i];
}

//returns number of terms in a line in the m_Lines vector
int CFileReader::GetNumberOfTermsFromLine(int i){
  if(i < 0 || i > m_NumberOfLines)
    return 0;

  return CountCommas(m_Lines[i])+ 1;
}

//returns particular term from 
std::string CFileReader::GetTerm(int lineNumber, int term){
  
  return "";
}

//returns terms a comma delimited string
std::string CFileReader::GetTerm(std::string line, int term){
  int commas = CountCommas(line);
  int count = 0;

  if(term < 1 || term > commas + 1)
    return "";

  
  if(commas == 0)// no commas..first term
    return line;

  if(commas > 0 && term == 1){
    for(size_t i = 0; i < line.size(); i++){
      if(line.substr(i,1) == ",")
        return line.substr(0,i);
    }
  }

  if(term == commas + 1){ //last term in a line
    for(size_t i = 0; i < line.size(); i++){
      if(line.substr(i,1) == ",")
        count++;
      if(count == commas){
        return line.substr(i + 1,line.size() - i);
      }
    }
  }


  int commaCount = 0;
  int beginComma = term - 1;
  size_t endComma = term;
  size_t beginPos = 0;
  size_t endPos = 0;

  for(size_t i = 0; i <  line.size(); i++){
    if(line.substr(i,1) == ","){
      commaCount++;
        
      if(commaCount == beginComma)
        beginPos = i;

      if(commaCount == endComma){
        endPos = i;
        if(endPos > beginPos){
          return line.substr(beginPos + 1, endPos - beginPos - 1);
        }
      }
    }
  }
  return "";

}

//***************************************** PRIVATE METHODS **********************
//removes all spaces from a string
//m_bSupressSpaces = true then don't remove spaces on lines with only one comma
std::string CFileReader::RemoveSpaces(std::string in){
  std::string temp;
  
  if(m_bSupressSpaces == true && CountCommas(in)> 1){
    
    for(size_t i = 0; i < in.size(); i++){
      if(in.substr(i, 1) != " "){
        temp = temp + in.substr(i,1);      
      }
    }
    
  }
  else if(m_bSupressSpaces == true  && CountCommas(in) == 1){
    //do nothing
    temp = in;

  }
  else{//false...don't supress
    for(size_t i = 0; i < in.size(); i++){
      if(in.substr(i, 1) != " "){
        temp = temp + in.substr(i,1);      
      }
    }
  }

  return temp;
}


//removes all comments from a string //
std::string CFileReader::RemoveComments(std::string in){
  std::string temp;

  for(size_t i = 0; i < in.size(); i ++){
    if(in.substr(i,2) != "//")
      temp = temp + in.substr(i, 1);
    else
      return temp;
  }
  return temp;
}

//returns the number of commas in a string
int CFileReader::CountCommas(std::string in){
  int count = 0;

  for(size_t i = 0; i < in.size(); i++){
    if(in.substr(i, 1) == ",")
      count++;

  }
  
  return count;
}

  
//  bool m_bFileExists;
//  int m_NumberOfLines;
//  std::vector<std::string> m_Lines;
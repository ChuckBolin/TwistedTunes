#ifndef CFILEREADER_H
#define CFILEREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class CFileReader{
public:
  CFileReader();
  CFileReader(std::string filename);

  void LoadFile(std::string filename);
  
  bool IsValid(){return m_bFileExists;}
  int GetNumberOfLines();
  std::string GetLineFromFile(int i);
  int GetNumberOfTermsFromLine(int i);
  std::string GetTerm(int lineNumber, int term);
  std::string GetTerm(std::string line, int term);
  void SupressSpaceRemoval(bool result){m_bSupressSpaces = result;};

private:
  std::string RemoveSpaces(std::string in);
  std::string RemoveComments(std::string in);
  int CountCommas(std::string in);
  

  std::string m_Filename;
  bool m_bFileExists;
  int m_NumberOfLines;
  std::vector<std::string> m_Lines;
  bool m_bSupressSpaces;
};

#endif
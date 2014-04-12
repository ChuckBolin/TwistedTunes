/*************************************************************************
  CCSVReader.h - Written by Chuck Bolin, January 2009
  Purpose:  Reads CSV data files and stores information into an
  std::vector.
*************************************************************************/
#ifndef CCSV_READER_H
#define CCSV_READER_H

#include <string>
#include <vector>
#include <fstream>
#include "CLog.h"

struct PARAMETERS{
  std::vector<std::string> line;
};

class CCSVReader{
public:
  CCSVReader();
  std::string GetTerm(size_t row, size_t col);
  void LoadFile(std::string filename);
  size_t GetNumberParameters(size_t row);
  size_t GetTableSize(){return m_tableSize;}
  std::string RemoveComments(std::string in);

private:
  std::string CCSVReader::RemoveSpaces(std::string in);
  std::vector<PARAMETERS> m_table;
  size_t m_tableSize;
  size_t m_numberParameters;
};

#endif
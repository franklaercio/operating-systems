#ifndef READER_H
#define READER_H

#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>

#include "matrix.h"

using namespace std;

enum FileType
{
  M1,
  M2
};

class Reader
{
public:
  Reader();
  ~Reader();
  vector<vector<int>> reader_file(string delimiter, Matrix matrix, FileType file_type);
  Matrix reader_matrix(FileType file_type, string name, string delimiter);
};

#endif
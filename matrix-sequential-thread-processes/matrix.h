#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;

class Matrix
{
private:
  int rows;
  int columns;

public:
  Matrix();
  ~Matrix();
  Matrix(int rows, int columns);
  void save_matrix(Matrix matrix, string file_name);
  int get_rows();
  void set_rows(int rows);
  int get_columns();
  void set_columns(int columns);
};

#endif
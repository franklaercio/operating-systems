#ifndef MATRIX_H
#define MATRIX_H

#include <string>

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
  int get_rows();
  void set_rows(int rows);
  int get_columns();
  void set_columns(int columns);
  bool isEmpty();
};

#endif
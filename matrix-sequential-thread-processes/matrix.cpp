#include "matrix.h"

using namespace std;

Matrix::Matrix()
{
  this->rows = 0;
  this->columns = 0;
}

Matrix::~Matrix() {}

Matrix::Matrix(int rows, int columns)
{
  this->rows = rows;
  this->columns = columns;
}

int Matrix::get_rows()
{
  return this->rows;
}

void Matrix::set_rows(int rows)
{
  this->rows = rows;
}

int Matrix::get_columns()
{
  return this->columns;
}

void Matrix::set_columns(int columns)
{
  this->columns = columns;
}

bool Matrix::isEmpty()
{
  return this->rows == 0 || this->columns == 0;
}
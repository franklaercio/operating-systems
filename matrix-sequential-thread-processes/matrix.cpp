#include "matrix.h"

using namespace std;

Matrix::Matrix() {}

Matrix::~Matrix() {}

Matrix::Matrix(int rows, int columns)
{
  this->rows = rows;
  this->columns = columns;
}

void Matrix::save_matrix(Matrix matrix, string file_name)
{
  fstream matrix_file;

  matrix_file.open(file_name, fstream::out | fstream::trunc);

  if (matrix_file.is_open())
  {
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    for (int i = 0; i < matrix.get_rows(); i++)
    {
      if (i < matrix.get_rows() && i > 0)
      {
        matrix_file << "\n";
      }

      for (int j = 0; j < matrix.get_columns(); j++)
      {
        if (j < matrix.get_columns() && j > 0)
        {
          matrix_file << "|";
        }

        matrix_file << (0 + rand() % ((100 + 1) - 0));
      }
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Time for generate " << file_name << " was " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " [ms]" << endl;
  }

  matrix_file.close();
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
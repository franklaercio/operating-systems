#include <iostream>
#include <iostream>
#include <chrono>
#include <fstream>

#include "matrix.h"

using namespace std;

#define M1_FILE_NAME "matrix_a.txt"
#define M2_FILE_NAME "matrix_b.txt"

#define DELIMITER " "

void save_file_matrix(Matrix matrix, string file_name)
{
  fstream matrix_file;

  matrix_file.open(file_name, fstream::out | fstream::trunc);

  if (matrix_file.is_open())
  {
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    matrix_file << matrix.get_rows() << DELIMITER << matrix.get_columns() << "\n";

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
          matrix_file << DELIMITER;
        }

        matrix_file << (0 + rand() % ((100 + 1) - 0));
      }
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Time for generate " << file_name << " was " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " [ms]" << endl;

    matrix_file << "\n"
                << chrono::duration_cast<chrono::milliseconds>(end - begin).count();
  }

  matrix_file.close();
}

int main(int argc, char *argv[])
{
  int rows_m1 = atoi(argv[1]);
  int cols_m1 = atoi(argv[2]);
  int rows_m2 = atoi(argv[3]);
  int cols_m2 = atoi(argv[4]);

  if (cols_m1 != rows_m2)
  {
    cout << "[ERROR] The number of columns of matrix a differs from the number of rows of matrix b [" << cols_m1 << " != " << rows_m2 << "]." << endl;
    return 0;
  }

  Matrix matrix_a(rows_m1, cols_m1);
  Matrix matrix_b(rows_m2, cols_m2);

  srand(time(0));

  save_file_matrix(matrix_a, M1_FILE_NAME);
  save_file_matrix(matrix_b, M2_FILE_NAME);

  return 0;
}
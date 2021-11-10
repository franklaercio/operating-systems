#include <iostream>

#include "matrix.h"

using namespace std;

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

  matrix_a.save_matrix(matrix_a, "matrix_a.txt");
  matrix_b.save_matrix(matrix_b, "matrix_b.txt");

  return 0;
}
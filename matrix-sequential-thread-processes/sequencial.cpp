#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include "matrix.h"
#include "reader.h"

using namespace std;

#define M1_FILE_NAME "matrix_a.txt"
#define M2_FILE_NAME "matrix_b.txt"
#define SEQUENCIAL_FILE_NAME "sequencial.txt"

#define DELIMITER " "

int main()
{
  Reader reader = Reader();

  Matrix m1 = reader.reader_matrix(M1, M1_FILE_NAME, DELIMITER);
  Matrix m2 = reader.reader_matrix(M1, M1_FILE_NAME, DELIMITER);

  if (m1.isEmpty() || m2.isEmpty())
  {
    cout << "[ERROR] Unable to open file, check for matrix_a.txt and matrix_b.txt files." << endl;
    return 0;
  }

  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

  fstream sequencial_file;

  sequencial_file.open(SEQUENCIAL_FILE_NAME, fstream::out | fstream::trunc);

  if (sequencial_file.is_open())
  {
    sequencial_file << m1.get_rows() << DELIMITER << m2.get_columns() << "\n";

    vector<vector<int>> matrix_m1 = reader.reader_file(DELIMITER, m1, M1);
    vector<vector<int>> matrix_m2 = reader.reader_file(DELIMITER, m2, M2);

    vector<vector<int>> matrix_m3(m1.get_rows(), vector<int>(m2.get_columns()));

    for (int i = 0; i < m1.get_rows(); i++)
    {
      for (int j = 0; j < m1.get_columns(); j++)
      {
        matrix_m3[i][j] = matrix_m1[i][j] * matrix_m2[j][i];
      }
    }

    for (int i = 0; i < m1.get_rows(); i++)
    {
      for (int j = 0; j < m2.get_columns(); j++)
      {
        sequencial_file << "c" << i << j << " " << matrix_m3[i][j] << "\n";
      }
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Time for matrix multiplication sequencial result was " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " [ms]" << endl;

    sequencial_file << chrono::duration_cast<chrono::milliseconds>(end - begin).count();
  }

  sequencial_file.close();

  return 0;
}
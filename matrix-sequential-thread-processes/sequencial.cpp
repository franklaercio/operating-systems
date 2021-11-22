#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/wait.h>

#include "constants.h"
#include "matrix.h"
#include "reader.h"

using namespace std;

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

  vector<vector<int>> matrix_m1 = reader.reader_file(DELIMITER, m1, M1);
  vector<vector<int>> matrix_m2 = reader.reader_file(DELIMITER, m2, M2);
  vector<vector<int>> matrix_m3(m1.get_rows(), vector<int>(m2.get_columns()));

  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, 80, "%F%T", timeinfo);

  string time_string = buffer;

  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

  fstream sequencial_file;

  string file_name = SEQUENCIAL_FILE_NAME + time_string + ".txt";

  sequencial_file.open(file_name, fstream::out | fstream::trunc);

  if (!sequencial_file.is_open())
  {
    cout << "[ERROR] Unable to open file." << endl;
    return -1;
  }

  int sum = 0;

  sequencial_file << m1.get_rows() << DELIMITER << m2.get_columns() << "\n";

  for (int i = 0; i < m1.get_rows(); i++)
  {
    for (int j = 0; j < m2.get_columns(); j++)
    {
      for (int k = 0; k < m2.get_rows(); k++)
      {
        sum = sum + matrix_m1[i][k] * matrix_m2[k][j];
      }

      matrix_m3[i][j] = sum;
      sum = 0;
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

  sequencial_file.close();

  sleep(1);

  return 0;
}
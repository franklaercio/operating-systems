#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

#include "constants.h"
#include "matrix.h"
#include "reader.h"

using namespace std;

vector<vector<int>> matrix_m1;
vector<vector<int>> matrix_m2;
vector<vector<int>> matrix_m3;

int rows_m3, cols_m3;

void matrix_multiplication(chrono::steady_clock::time_point begin,
                           int first_line, int last_line, Matrix m2, int nthreads)
{
  fstream thread_file;

  string file_name = THREAD_PREFIX + to_string(nthreads) + ".txt";

  thread_file.open(file_name, fstream::out | fstream::trunc);

  if (!thread_file.is_open())
  {
    cout << "[ERROR] Unable to open file." << endl;
    exit(-1);
  }

  thread_file << rows_m3 << DELIMITER << cols_m3 << "\n";

  int sum = 0;

  for (int i = first_line; i < last_line; i++)
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

  for (int i = first_line; i < last_line; i++)
  {
    for (int j = 0; j < m2.get_columns(); j++)
    {
      thread_file << "c" << i << j << " " << matrix_m3[i][j] << "\n";
    }
  }

  chrono::steady_clock::time_point end = chrono::steady_clock::now();

  int time = chrono::duration_cast<chrono::milliseconds>(end - begin).count();

  cout << "Time for matrix multiplication thread #" << nthreads << " result was " << time << " [ms]" << endl;

  thread_file << time;

  thread_file.close();
}

int main(int argc, char *argv[])
{
  int nthreads = atoi(argv[1]);

  Reader reader = Reader();

  Matrix m1 = reader.reader_matrix(M1, M1_FILE_NAME, DELIMITER);
  Matrix m2 = reader.reader_matrix(M1, M1_FILE_NAME, DELIMITER);

  if (m1.isEmpty() || m2.isEmpty())
  {
    cout << "[ERROR] Unable to open file, check for matrix_a.txt and matrix_b.txt files." << endl;
    return 0;
  }

  matrix_m1 = reader.reader_file(DELIMITER, m1, M1);
  matrix_m2 = reader.reader_file(DELIMITER, m2, M2);

  rows_m3 = m1.get_rows();
  cols_m3 = m2.get_columns();

  vector<vector<int>> matrix_copy(rows_m3, vector<int>(cols_m3));
  matrix_m3 = matrix_copy;

  int rows = int(m1.get_rows() / nthreads);

  if (m1.get_rows() % nthreads != 0)
  {
    rows += 1;
  }

  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

  thread threads[nthreads];

  int first_line = 0;
  int last_line = rows;

  for (int i = 0; i < nthreads; i++)
  {
    threads[i] = thread(matrix_multiplication, begin, first_line, last_line, m2, i);
    first_line += rows;
    last_line += rows;
  }

  for (auto &thread : threads)
  {
    thread.join();
  }

  chrono::steady_clock::time_point end = chrono::steady_clock::now();

  int time_total = chrono::duration_cast<chrono::milliseconds>(end - begin).count();

  cout << "Total time threads for matrix multiplication result was " << time_total << " [ms]" << endl;

  fstream thread_file;
  thread_file.open(THREAD_FILE_NAME, fstream::out | fstream::trunc);

  thread_file << rows_m3 << DELIMITER << cols_m3 << "\n";

  if (!thread_file.is_open())
  {
    cout << "[ERROR] Unable to open file." << endl;
    return -1;
  }

  for (int i = 0; i < rows_m3; i++)
  {
    for (int j = 0; j < cols_m3; j++)
    {
      thread_file << "c" << i << j << " " << matrix_m3[i][j] << "\n";
    }
  }

  thread_file << time_total;

  thread_file.close();

  return 0;
}
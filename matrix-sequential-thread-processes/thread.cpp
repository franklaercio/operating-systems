#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "matrix.h"
#include "reader.h"

using namespace std;

#define M1_FILE_NAME "matrix_a.txt"
#define M2_FILE_NAME "matrix_b.txt"

#define DELIMITER " "

int time_total = 0;

void matrix_multiplication(int rows, int cols_m1, int cols, int nthreads, vector<vector<int>> matrix_m1, vector<vector<int>> matrix_m2)
{
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

  fstream thread_file;

  string file_name = "thr_" + to_string(nthreads) + ".txt";

  thread_file.open(file_name, fstream::out | fstream::trunc);

  if (thread_file.is_open())
  {
    thread_file << rows << DELIMITER << cols << "\n";

    vector<vector<int>> matrix_m3(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols_m1; j++)
      {
        matrix_m3[i][j] = matrix_m1[i][j] * matrix_m2[j][i];
      }
    }

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
      {
        thread_file << "c" << i << j << " " << matrix_m3[i][j] << "\n";
      }
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    int time = chrono::duration_cast<chrono::milliseconds>(end - begin).count();

    cout << "Time for matrix multiplication thread #" << nthreads << " result was " << time << " [ms]" << endl;

    thread_file << time;

    thread_file.close();

    time_total += time;
  }
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

  vector<vector<int>> matrix_m1 = reader.reader_file(DELIMITER, m1, M1);
  vector<vector<int>> matrix_m2 = reader.reader_file(DELIMITER, m2, M2);

  thread threads[nthreads];

  int rows = int(m1.get_rows() / nthreads);

  if (m1.get_rows() % nthreads != 0)
  {
    rows += 1;
  }

  for (int i = 0; i < nthreads; i++)
  {
    threads[i] = thread(matrix_multiplication, rows, m1.get_columns(), m2.get_columns(), i, matrix_m1, matrix_m2);
  }

  for (int i = 0; i < nthreads; i++)
  {
    threads[i].join();
  }

  cout << "Total time threads for matrix multiplication result was " << time_total << " [ms]" << endl;

  return 0;
}
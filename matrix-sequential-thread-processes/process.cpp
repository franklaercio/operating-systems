#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <queue>

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
  fstream process_file;

  string file_name = PROCESS_PREFIX + to_string(nthreads) + ".txt";

  process_file.open(file_name, fstream::out | fstream::trunc);

  if (!process_file.is_open())
  {
    cout << "[ERROR] Unable to open file." << endl;
    exit(-1);
  }

  process_file << rows_m3 << DELIMITER << cols_m3 << "\n";

  vector<vector<int>> matrix_m3(rows_m3, vector<int>(cols_m3));

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
      process_file << "c" << i << j << " " << matrix_m3[i][j] << "\n";
    }
  }

  chrono::steady_clock::time_point end = chrono::steady_clock::now();

  int time = chrono::duration_cast<chrono::milliseconds>(end - begin).count();

  cout << "Time for matrix multiplication process #" << nthreads << " result was " << time << " [ms]" << endl;

  process_file << time;

  process_file.close();
}

int main(int argc, char *argv[])
{
  int process = atoi(argv[1]);

  Reader reader = Reader();

  Matrix m1 = reader.reader_matrix(M1, M1_FILE_NAME, DELIMITER);
  Matrix m2 = reader.reader_matrix(M1, M1_FILE_NAME, DELIMITER);

  if (m1.isEmpty() || m2.isEmpty())
  {
    cout << "[ERROR] Unable to open file, check for matrix_a.txt and matrix_b.txt files." << endl;
    return -1;
  }

  rows_m3 = m1.get_rows();
  cols_m3 = m2.get_columns();

  matrix_m1 = reader.reader_file(DELIMITER, m1, M1);
  matrix_m2 = reader.reader_file(DELIMITER, m2, M2);

  vector<vector<int>> matrix_copy(rows_m3, vector<int>(cols_m3));
  matrix_m3 = matrix_copy;

  int rows = int(m1.get_rows() / process);

  if (m1.get_rows() % process != 0)
  {
    rows += 1;
  }

  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

  pid_t *pids = (int *)malloc((process) * sizeof(int));

  int first_line = 0;
  int last_line = rows;

  for (int i = 0; i < process; i++)
  {
    if (i > 0)
    {
      first_line += rows;
      last_line += rows;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
      matrix_multiplication(begin, first_line, last_line, m2, i);
      exit(0);
    }
  }

  for (int i = 0; i < process; i++)
  {
    wait(NULL);
  }

  free(pids);

  chrono::steady_clock::time_point end = chrono::steady_clock::now();

  int time_total = chrono::duration_cast<chrono::milliseconds>(end - begin).count();

  cout << "Total time threads for matrix multiplication result was " << time_total << " [ms]" << endl;

  fstream process_file;
  process_file.open(THREAD_FILE_NAME, fstream::out | fstream::trunc);

  process_file << rows_m3 << DELIMITER << cols_m3 << "\n";

  if (!process_file.is_open())
  {
    cout << "[ERROR] Unable to open file." << endl;
    return -1;
  }

  for (int i = 0; i < rows_m3; i++)
  {
    for (int j = 0; j < cols_m3; j++)
    {
      process_file << "c" << i << j << " " << matrix_m3[i][j] << "\n";
    }
  }

  process_file << time_total;

  process_file.close();
}
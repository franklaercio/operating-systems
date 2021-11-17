#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <queue>

#include "matrix.h"
#include "reader.h"

using namespace std;

#define M1_FILE_NAME "matrix_a.txt"
#define M2_FILE_NAME "matrix_b.txt"
#define PROCESS_PREFIX "process_"
#define PROCESS_FILE_NAME "process.txt"

#define DELIMITER " "

int time_total = 0;

void matrix_multiplication(int rows, int cols_m1, int cols, int nthreads, vector<vector<int>> matrix_m1, vector<vector<int>> matrix_m2)
{
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

  fstream process_file;

  string file_name = PROCESS_PREFIX + to_string(nthreads) + ".txt";

  process_file.open(file_name, fstream::out | fstream::trunc);

  if (process_file.is_open())
  {
    process_file << rows << DELIMITER << cols << "\n";

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
        process_file << "c" << i << j << " " << matrix_m3[i][j] << "\n";
      }
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    int time = chrono::duration_cast<chrono::milliseconds>(end - begin).count();

    cout << "Time for matrix multiplication process #" << nthreads << " result was " << time << " [ms]" << endl;

    process_file << time;

    process_file.close();

    time_total += time;
  }
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
    return 0;
  }

  vector<vector<int>> matrix_m1 = reader.reader_file(DELIMITER, m1, M1);
  vector<vector<int>> matrix_m2 = reader.reader_file(DELIMITER, m2, M2);

  int rows = int(m1.get_rows() / process);

  if (m1.get_rows() % process != 0)
  {
    rows += 1;
  }

  pid_t *pids = (int *)malloc((process) * sizeof(int));

  for (int i = 0; i < process; i++)
  {
    matrix_multiplication(rows, m1.get_columns(), m2.get_columns(), i, matrix_m1, matrix_m2);

    pids[i] = fork();

    if (pids[i] == 0)
    {
      exit(0);
    }
  }

  wait(NULL);
  free(pids);
  cout << "Total time process for matrix multiplication result was " << time_total << " [ms]" << endl;

  int total_rows = m2.get_columns();
  int total_cols = m1.get_columns();

  fstream process_file;
  process_file.open(PROCESS_FILE_NAME, fstream::out | fstream::trunc);

  process_file << total_rows << DELIMITER << total_cols << "\n";

  if (process_file.is_open())
  {

    for (int i = 0; i < process; i++)
    {
      fstream child_process_file;
      string file_name = PROCESS_PREFIX + to_string(i) + ".txt";
      child_process_file.open(file_name);

      string line;

      int rows_counts = 0;
      int cols_counts = 0;

      if (child_process_file.is_open())
      {
        getline(child_process_file, line);

        int total_lines = rows * m2.get_columns();

        for (int i = 0; i < total_lines; i++)
        {
          getline(child_process_file, line);

          int time = stoi(line.substr(line.find(DELIMITER) + 1));

          process_file << "c" << rows_counts << cols_counts << DELIMITER << time << "\n";

          if (total_cols == cols_counts)
          {
            cols_counts = 0;
            rows_counts += 1;
          }

          cols_counts += 1;
        }
      }

      child_process_file.close();
    }
  }

  process_file << time_total;

  process_file.close();
}
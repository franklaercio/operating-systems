#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

string DELIMITER = "|";

ifstream matrix_a_txt;
ifstream matrix_b_txt;

enum FileType
{
  M1,
  M2
};

vector<vector<int>> read_matrix(int rows, int columns, FileType type_file)
{
  vector<vector<int>> matrix(rows, vector<int>(columns));

  for (int i = 0; i < rows; i++)
  {
    string line;

    if (type_file == M1)
    {
      getline(matrix_a_txt, line);
    }
    else
    {
      getline(matrix_b_txt, line);
    }

    int j = 0;
    size_t pos = 0;
    string number;

    while ((pos = line.find(DELIMITER)) != std::string::npos)
    {
      number = line.substr(0, pos);

      matrix[i][j] = stoi(number);

      line.erase(0, pos + DELIMITER.length());

      j += 1;
    }

    matrix[i][j] = stoi(line);
  }

  return matrix;
}

int main()
{

  matrix_a_txt.open("matrix_a.txt");
  matrix_b_txt.open("matrix_b.txt");

  if (matrix_a_txt.is_open() && matrix_b_txt.is_open())
  {
    string line_matrix_a, line_matrix_b;

    getline(matrix_a_txt, line_matrix_a);
    getline(matrix_b_txt, line_matrix_b);

    string rows_m1 = line_matrix_a.substr(0, line_matrix_a.find(DELIMITER));
    string cols_m1 = line_matrix_a.substr(line_matrix_a.find(DELIMITER) + 1);
    string rows_m2 = line_matrix_b.substr(0, line_matrix_b.find(DELIMITER));
    string cols_m2 = line_matrix_b.substr(line_matrix_b.find(DELIMITER) + 1);

    fstream sequencial_file;

    int number_rows_m1 = stoi(rows_m1);
    int number_cols_m1 = stoi(cols_m1);
    int number_rows_m2 = stoi(rows_m2);
    int number_cols_m2 = stoi(cols_m2);

    sequencial_file.open("sequencial_result.txt", fstream::out | fstream::trunc);

    if (sequencial_file.is_open())
    {
      sequencial_file << rows_m1 << " " << cols_m2 << "\n";

      vector<vector<int>> matrix_m1 = read_matrix(number_rows_m1, number_cols_m1, M1);
      vector<vector<int>> matrix_m2 = read_matrix(number_rows_m2, number_cols_m2, M2);

      chrono::steady_clock::time_point begin = chrono::steady_clock::now();

      for (int i = 0; i < number_rows_m1; i++)
      {
        for (int j = 0; j < number_cols_m1; j++)
        {
          sequencial_file << "c" << i << j << " " << matrix_m1[i][j] * matrix_m2[j][i] << "\n";
        }
      }

      chrono::steady_clock::time_point end = chrono::steady_clock::now();

      cout << "Time for generate sequencial result was " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " [ms]" << endl;

      sequencial_file << chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    }

    sequencial_file.close();
  }
  else
  {
    cout << "[ERROR] Unable to open file, check for matrix_a.txt and matrix_b.txt files." << endl;
  }
}
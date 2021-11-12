#include "reader.h"

#define M1_FILE_NAME "matrix_a.txt"
#define M2_FILE_NAME "matrix_b.txt"

Reader::Reader() {}

Reader::~Reader() {}

vector<vector<int>> Reader::reader_file(string delimiter, Matrix matrix, FileType file_type)
{
  vector<vector<int>> vector_matrix(matrix.get_rows(), vector<int>(matrix.get_columns()));

  string line;
  ifstream file;

  if (file_type == M1)
  {
    file.open(M1_FILE_NAME);
  }
  else
  {
    file.open(M2_FILE_NAME);
  }

  getline(file, line);

  for (int i = 0; i < matrix.get_rows(); i++)
  {
    getline(file, line);

    int j = 0;
    size_t pos = 0;
    string number;

    while ((pos = line.find(delimiter)) != std::string::npos)
    {
      number = line.substr(0, pos);

      vector_matrix[i][j] = stoi(number);

      line.erase(0, pos + delimiter.length());

      j += 1;
    }

    vector_matrix[i][j] = stoi(line);
  }

  return vector_matrix;
}

Matrix Reader::reader_matrix(FileType file_type, string name, string delimiter)
{
  Matrix matrix = Matrix();
  ifstream file;

  file.open(name);

  if (file.is_open())
  {
    string line;

    getline(file, line);

    int rows = stoi(line.substr(0, line.find(delimiter)));
    int cols = stoi(line.substr(line.find(delimiter) + 1));

    fstream sequencial_file;

    matrix = Matrix(rows, cols);

    file.close();
  }

  return matrix;
}
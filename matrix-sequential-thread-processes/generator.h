#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
#include "matrix.h"

using namespace std;

class Main
{
public:
  string quit();
  string random_matrix(string rows_m1, string columns_m1, string rows_m2, string columns_m2);
};

#endif
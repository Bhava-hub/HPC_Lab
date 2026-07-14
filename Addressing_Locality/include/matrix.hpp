#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

const int SIZE = 2500;

using Matrix = std::vector<std::vector<double>>;

Matrix createMatrix();
void fillMatrix(Matrix &A);

#endif
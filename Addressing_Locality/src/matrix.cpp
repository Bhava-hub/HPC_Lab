#include "matrix.hpp"
#include <cstdlib>

Matrix createMatrix()
{
    return Matrix(SIZE, std::vector<double>(SIZE,0));
}

void fillMatrix(Matrix &A)
{
    for(int i=0;i<SIZE;i++)
        for(int j=0;j<SIZE;j++)
            A[i][j]=rand()%100;
}
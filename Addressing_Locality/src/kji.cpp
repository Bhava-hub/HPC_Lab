#include "kji.hpp"

Matrix multiply_kji(const Matrix &A,const Matrix &B)
{
    Matrix C=createMatrix();

    for(int k=0;k<SIZE;k++)
        for(int j=0;j<SIZE;j++)
            for(int i=0;i<SIZE;i++)
                C[i][j]+=A[i][k]*B[k][j];

    return C;
}
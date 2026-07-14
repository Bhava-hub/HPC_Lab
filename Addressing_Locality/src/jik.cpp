#include "jik.hpp"

Matrix multiply_jik(const Matrix &A,const Matrix &B)
{
    Matrix C=createMatrix();

    for(int j=0;j<SIZE;j++)
        for(int i=0;i<SIZE;i++)
            for(int k=0;k<SIZE;k++)
                C[i][j]+=A[i][k]*B[k][j];

    return C;
}
#include "ikj.hpp"

Matrix multiply_ikj(const Matrix &A,const Matrix &B)
{
    Matrix C=createMatrix();

    for(int i=0;i<SIZE;i++)
        for(int k=0;k<SIZE;k++)
            for(int j=0;j<SIZE;j++)
                C[i][j]+=A[i][k]*B[k][j];

    return C;
}
#include "jki.hpp"

Matrix multiply_jki(const Matrix &A,const Matrix &B)
{
    Matrix C=createMatrix();

    for(int j=0;j<SIZE;j++)
        for(int k=0;k<SIZE;k++)
            for(int i=0;i<SIZE;i++)
                C[i][j]+=A[i][k]*B[k][j];

    return C;
}
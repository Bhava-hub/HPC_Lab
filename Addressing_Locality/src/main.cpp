#include <iostream>
#include <string>

#include "matrix.hpp"

#include "ijk.hpp"
#include "ikj.hpp"
#include "jik.hpp"
#include "jki.hpp"
#include "kij.hpp"
#include "kji.hpp"

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        std::cout<<"Usage: ./matrix_mul ijk|ikj|jik|jki|kij|kji\n";
        return 1;
    }

    Matrix A=createMatrix();
    Matrix B=createMatrix();

    fillMatrix(A);
    fillMatrix(B);

    Matrix C;

    std::string s=argv[1];

    if(s=="ijk") C=multiply_ijk(A,B);
    else if(s=="ikj") C=multiply_ikj(A,B);
    else if(s=="jik") C=multiply_jik(A,B);
    else if(s=="jki") C=multiply_jki(A,B);
    else if(s=="kij") C=multiply_kij(A,B);
    else if(s=="kji") C=multiply_kji(A,B);

    std::cout<<C[0][0]<<std::endl;
}
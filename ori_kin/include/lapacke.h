#pragma once
extern "C" {
    #define LAPACK_ROW_MAJOR               101
    #define LAPACK_COL_MAJOR               102
    int LAPACKE_dgesdd(int, char,int,int,double*,int,double*,double*,int,double*,int);
}
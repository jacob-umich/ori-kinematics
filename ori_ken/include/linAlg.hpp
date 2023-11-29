#include <iostream>
#include <string>
#include <fstream>  
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#if BLAS_FOUND
#include <lapacke.h>
#include <cblas.h>
#endif

class linAlg{
    // constructor/destructor
    public:
        linAlg();
        virtual ~linAlg();
    //member functions
    public:
        void matPseudoInv(int m, int n, double *A, double *Ainv);
        void matMult(int m, int n,int k, double *A, double *B, double *C);

};
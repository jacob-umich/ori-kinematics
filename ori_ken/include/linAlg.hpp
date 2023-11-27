#include <iostream>
#include <string>
#include <fstream>  
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lapacke.h>
#include <cblas.h>

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
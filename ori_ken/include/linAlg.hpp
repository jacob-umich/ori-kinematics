#ifndef LINEARALGEBRA_H
#define LINEARALGEBRA_H

#include <iostream>
#include <string>
#include <fstream>  
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <lapacke.h>
#include <cblas.h>
using namespace std;
class linAlg{
    // constructor/destructor
    public:
        linAlg();
        virtual ~linAlg();
    //member functions
    public:
        void matPseudoInv(const int m, const int n, vector<double> &A_vec, vector<double> &Ainv_vec);
        void matMult(int m, int n,int k, vector<double> &A_vec, vector<double> &B_vec, vector<double> &C_vec);

};
#endif
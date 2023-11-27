#include "ori_ken.hpp"
#include "linAlg.hpp"
using namespace std;
int main(){
    linAlg *linAlgClass;
    linAlgClass = new linAlg;
    int m=3, n=2;
    double A=[4,-1,7,2,-6,1];
    double Ainv = [m*n];
    double C [m*m];
    linAlgClass->matPseudoInv(int m, int n, double *A, double *Ainv);
    linAlgClass->matMult(m, m,n, A, Ainv, C);
    cout<<"Ainv: ";
    for (int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            cout<<Ainv[i*n+j]<<" ";
        }
    }
    cout<<endl<<"A*Ainv";
    for (int i=0; i<m; i++){
        for(int j=0; j<m; j++){
            cout<<C[i*m+j]<<" ";
        }
    }
    cout<<endl;
    return 0;
}
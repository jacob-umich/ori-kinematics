#include "linAlg.hpp"
#include <vector>
using namespace std;
int main(){
    linAlg *linAlgClass;
    linAlgClass = new linAlg;
    const int m=3, n=2;
    vector<double> A{4,-1,7,2,-6,1};
    vector<double> Ainv;
    vector<double> C;
    Ainv.resize(m*n);
    C.resize(m*n);

    linAlgClass->matPseudoInv(m, n, A, Ainv);
    linAlgClass->matMult(m, m,n, A, Ainv, C);
    cout<<"Ainv: ";
    for (int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            cout<<Ainv[i*n+j]<<" ";
        }
    }
    cout<<endl<<"A*Ainv: ";
    for (int i=0; i<m; i++){
        for(int j=0; j<m; j++){
            cout<<C[i*m+j]<<" ";
        }
    }
    cout<<endl;
    delete linAlgClass;
    return 0;
}
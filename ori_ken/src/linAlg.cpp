#include "linAlg.hpp"


// compiling commands in great lakes:
//module load gcc
//module load openblas
//module load lapack


linAlg::linAlg(void) {}

linAlg::~linAlg() {}
#if BLAS_FOUND
void linAlg::matPseudoInv(int m, int n, double *A, double *Ainv){
    const int m=3,n=2;

    double a[m * n] = {2, -1, 2,1,-2,1};
    double u[m * m], s[std::min(n,m)],vt[n * n], v[n*n],suT[n*m];
    const int aa = 1; //dgemm AB scaling
    const int c = 0; //dgemm C scaling

    int lda = m, ldu = m, ldvt = n;

    // SVD: A = UsV^T where U=mxm s=mxn V^T=nxn A=mxn
    // Ainv = V(s^-1)U^T where V=nxn s^-1=nxm U^T=mxm Ainv = nxm

    //computing the SVD
    int info = LAPACKE_dgesdd(LAPACK_COL_MAJOR, 'A', m, n, a, lda, s,
                u, ldu, vt, ldvt);
    if (info !=0){
    std::cerr<<"Lapack error occured in dgesdd. error code :"<<info<<std::endl;
    }


    for (int i = 0; i < n; i++) {
            s[i] = 1.0 / s[i];       
    }

    // transpose vt and u
    // for (int i = 0; i <n; i++) {// index of rows (column-wise)
    //     for (int j=0; j<n; j++){// index of columns (row-wise)
    //         v[i*n+j]=vt[j*n+i];
    //     }
    // }
    // for (int i = 0; i < n; i++) {
    // for (int j = 0; j < n; j++) {
    //     std::cout<<v[i*n+j]<<" ";}}
    //     std::cout<<std::endl;
    for (int i = 0; i <m; i++) { //u^T is mxm but ignore last row since multiplied by s^-1 (nxm)
        for (int j=0; j<n; j++){
            suT[i*n+j]=u[j*m+i];
        }
    }
    // for (int i = 0; i < m; i++) {
    // for (int j = 0; j < n; j++) {
    //     std::cout<<suT[i*n+j]<<" ";}}
    //     std::cout<<std::endl;
    // for (int j = 0; j < n; j++) {
    //     std::cout<<s[j]<<" ";}
    //compute the  first multiplication (s^-1)u^T (s^-1 is nxm u^T is mxm) creates nxm
    // here : s is not a vector : it is a diagonal matrix. The ouput must be of size n*n
    for (int i = 0; i<m; i++) {
        for (int j=0; j<n; j++){
            suT[i*n+j]=s[j]*suT[i*n+j];
        }
    }

    // for (int i = 0; i < m; i++) {
    // for (int j = 0; j < n; j++) {
    //     std::cout<<suT[i*n+j]<<" ";}}

    //compute the second multiplication Ainv=v(s^-1)u^T which is nxm vt is nxn (s-1)u^T is nxm
    cblas_dgemm(CblasColMajor,CblasTrans, CblasNoTrans, n, m, n, aa, vt, n, suT, n, c, Ainv, n);
    //Ainv is the pseudoinverse of A.

}
void linAlg::matMult(int m, int n,int k, double *A, double *B, double *C){
    // Order:Specifies row-major (C) or column-major (Fortran) data ordering.
    // TransA:Specifies whether to transpose matrix A.
    // TransB:Specifies whether to transpose matrix B.
    // M:Number of rows in matrices A and C.
    // N:Number of columns in matrices B and C.
    // K:Number of columns in matrix A; number of rows in matrix B.
    // alpha:Scaling factor for the product of matrices A and B.
    // A:Matrix A.
    // lda:The size of the first dimension of matrix A; if you are passing a matrix A[m][n], the value should be m.
    // B:Matrix B.
    // ldb:The size of the first dimension of matrix B; if you are passing a matrix B[m][n], the value should be m.
    // beta:Scaling factor for matrix C.
    // C:Matrix C.
    // ldc:The size of the first dimension of matrix C; if you are passing a matrix C[m][n], the value should be m.
    const int aa = 1; //dgemm AB scaling
    const int cc = 0; //dgemm C scaling
    cblas_dgemm(CblasColMajor,CblasNoTrans, CblasNoTrans, m, n, k, aa, A, m, B, k, cc, C, m);
};
#else

void linAlg::matMult(int m, int n,int k, double *A, double *B, double *C)
{
  for (int y=0; y<m;y++){
    for (int z=0; z<n; z++){
      for (int x=0; x<k;x++){
        C[x*m+z]+=A[x*m+y]*B[y*n+z];
      }
    }
	
}
}

void linAlg::matPseudoInv(int m, int n, double *A, double *Ainv){
    for (int y=0; y<m;y++){
        for (int z=0; z<n; z++){
            Ainv[y*n+z]=A[y*n+z];
        }
    }

}
#endif
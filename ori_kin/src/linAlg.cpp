#include "linAlg.hpp"


// compiling commands in great lakes:
//module load gcc
//module load openblas
//module load lapack


linAlg::linAlg(void) {}

linAlg::~linAlg() {}

using std::vector;
#if BLAS_FOUND

void linAlg::matPseudoInv(const int m, const int n, vector<double> &A_vec, vector<double> &Ainv_vec){

    double *u = new double[m*m];
    double *s= new double[std::min(n,m)];
    double *vt = new double[n*n];
    double *suT = new double[n*m];
    double *A = new double[n*m];
    double *Ainv = new double[n*m];
    const int aa = 1; //dgemm AB scaling
    const int c = 0; //dgemm C scaling
    //assign values to A array
    //for (int i=0; i<n*m; i++) A[i]=A_vec[i];
    copy(A_vec.begin(), A_vec.end(), A);

    int lda = m, ldu = m, ldvt = n;

    // SVD: A = UsV^T where U=mxm s=mxn V^T=nxn A=mxn
    // Ainv = V(s^-1)U^T where V=nxn s^-1=nxm U^T=mxm Ainv = nxm

    //computing the SVD
    int info = LAPACKE_dgesdd_(LAPACK_COL_MAJOR, 'A', m, n, A, lda, s,
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
    //     std::cout<<vt[i*n+j]<<" ";}}
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
    // cout<< "suT: ";
    // for (int i = 0; i < m; i++) {
    // for (int j = 0; j < n; j++) {
    //     std::cout<<suT[i*n+j]<<" ";}}
    // std::cout<<std::endl;

    //compute the second multiplication Ainv=v(s^-1)u^T which is nxm vt is nxn (s-1)u^T is nxm
    cblas_dgemm(CblasColMajor,CblasTrans,CblasNoTrans, n, m, n, aa, vt, n, suT, n, c, Ainv, n);
    //Ainv is the pseudoinverse of A.
    // cout<< "Ainv: ";
    // for (int i = 0; i < m; i++) {
    // for (int j = 0; j < n; j++) {
    //     std::cout<<Ainv[i*n+j]<<" ";}}
    // std::cout<<std::endl;
    //assign values to Ainv to output vector
    for (int i=0; i<n*m; i++) Ainv_vec[i]=Ainv[i];

    // delete [] u;
    delete [] s;
    delete [] vt;
    delete [] suT;
    delete [] A;
    delete [] Ainv;
}
void linAlg::matMult(int m, int n,int k, vector<double> &A_vec, vector<double> &B_vec, vector<double> &C_vec){
    // Order:Specifies row-major (C) or column-major (Fortran) data ordering.
    // TransA:Specifies whether to transpose matrix A.
    // TransB:Specifies whether to transpose matrix B.
    // M:Number of rows in matrices A and C.
    // N:Number of columns in matrices B and C.
    // K:Number of columns in matrix A; number of rows in matrix B.
    // alpha:Scaling factor for the product of matrices A and B.
    // A:Matrix A mxk.
    // lda:The size of the first dimension of matrix A; if you are passing a matrix A[m][n], the value should be m.
    // B:Matrix B kxn.
    // ldb:The size of the first dimension of matrix B; if you are passing a matrix B[m][n], the value should be m.
    // beta:Scaling factor for matrix C mxn.
    // C:Matrix C.
    // ldc:The size of the first dimension of matrix C; if you are passing a matrix C[m][n], the value should be m.
    const int aa = 1; //dgemm AB scaling
    const int cc = 0; //dgemm C scaling
    double *A = new double[m*k];
    double *B = new double[k*n];
    double *C = new double[n*m];
    //convert to standard arrays from input vectors
    //for (int i=0; i<k*m; i++) A[i]=A_vec[i]; 
    copy(A_vec.begin(), A_vec.end(), A);
    copy(B_vec.begin(), B_vec.end(), B);

    cblas_dgemm(CblasColMajor,CblasNoTrans, CblasNoTrans, m, n, k, aa, A, m, B, k, cc, C, m);

    for (int i=0; i<n*m; i++) C_vec[i]=C[i];

    delete[] A;
    delete[] B;
    delete[] C;
}
#else
void linAlg::matMult(int m, int n,int k, vector<double> &A_vec, vector<double> &B_vec, vector<double> &C_vec)
{
  for (int y=0; y<m;y++){
    for (int z=0; z<n; z++){
      for (int x=0; x<k;x++){
        C_vec[x*m+z]+=A_vec[x*m+y]*B_vec[y*n+z];
      }
    }
	
}
}



void linAlg::matPseudoInv(const int m, const int n, vector<double> &A_vec, vector<double> &Ainv_vec){
    for (int y=0; y<m;y++){
        for (int z=0; z<n; z++){
            Ainv_vec[z*m+y]=A_vec[y*n+z];
        }
    }

}

// this is me trying to compute svd

double sign(double x){
    if (x<0){
        return -1;
    } else {
        return 1;
    }
}

void computeq(int m, int k,vector<double>&a,vector<double>&q){
    double sumsq=0;
    for (int i=k;i<m;i++){
        sumsq+=a[i*m+k]*a[i*m+k];
    }
    double mag = sqrt(sumsq);
    q[0] = a[k*m+k]-sign(a[k*m+k])*mag;
    for (int i=1;i<(m-k);i++){
        q[i]=a[i*m+k];
    }
    sumsq=0;
    for (int i=0;i<(m-k);i++){
        sumsq+=q[i]*q[i];
    }
    for (int i=0;i<(m-k);i++){
        q[i]=q[i]/sumsq;
    }
}

void computep(int m, int n, int k,vector<double>&a,vector<double>&p){
    double sumsq=0;
    for (int i=k+1;i<n;i++){
        sumsq+=a[k*m+i]*a[k*m+i];
    }
    double mag = sqrt(sumsq);
    p[0] = a[k*m+k+1]-sign(a[k*m+k+1])*mag;
for (int i=1;i<(n-k-1);i++){
        p[i]=a[k*m+i+k+1];
    }
    sumsq=0;
    for (int i=0;i<(n-k-1);i++){
        sumsq+=p[i]*p[i];
    }
    for (int i=0;i<(n-k-1);i++){
        p[i]=p[i]/sumsq;
    }
}

void householderReflect(int m, int n,int k, vector<double>a,vector<double>Q,vector<double>P){
    vector<double> ident;
    ident.resize((m-k)*(m-k));
    Q.resize((m-k)*(m-k));
    P.resize((n-k-1)*(n-k-1));
    for (int i=0;i<(m-k);i++){
        for (int j=0;j<(m-k);j++){
            if(i==j){
                ident[i*(m-k)+j]=1;
            }else{
                ident[i*(m-k)+j]=0;
            }
        }
        
    }
    vector<double> q;
    vector<double> p;
    q.resize(m-k);
    p.resize(n-k-1);
    computeq(m,k,a,q);
    computep(m,n,k,a,q);
    for (int i=0;i<(m-k);i++){
        for (int j=0;j<(m-k);j++){
            Q[i*(m-k)+j]=ident[i*(m-k)+j]-2*q[i]*q[j];
        }
        
    }
    for (int i=0;i<(n-k-1);i++){
        for (int j=0;j<(n-k-1);j++){
            P[i*(n-k-1)+j]=ident[i*(n-k-1)+j]-2*p[i]*p[j];
        }
        
    }

}

void step1(vector<double>a){
    //svd is very complex
};

#endif
    


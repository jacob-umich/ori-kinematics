// Testing library required for testing (Always include this last!)
#include "ori_kin.hpp"
#include "unit_test_framework.hpp"
using namespace std;
// Test for pseudo inv
#if BLAS_FOUND
TEST_CASE(pseudoInv) {

  linAlg *linAlgClass;
  linAlgClass = new linAlg;
  const int m=3, n=2;
  vector<double> A{4,-1,7,2,-6,1};
  vector<double> Ainv_true{0.053863134657837,0.021192052980132,0.037527593818985,-0.165562913907285,0.117439293598234,-0.035761589403974};
  vector<double> Ainv;
  Ainv.resize(m*n);
  linAlgClass->matPseudoInv(m, n, A, Ainv);
  for (int ii=0;ii<m*n;ii++) ASSERT_NEAR(Ainv[ii],Ainv_true[ii],1e-5);
  delete linAlgClass;

}
TEST_CASE(pseudoInvNGrM) {

  linAlg *linAlgClass;
  linAlgClass = new linAlg;
  const int m=2, n=3;
  vector<double> A{4,-1,7,2,-6,1};
  vector<double> Ainv_true{0.0474,0.0576,-0.06779,-0.1983,0.2949,0.21186};
  vector<double> Ainv;
  Ainv.resize(m*n);
  linAlgClass->matPseudoInv(m, n, A, Ainv);
  for (int ii=0;ii<m*n;ii++) ASSERT_NEAR(Ainv[ii],Ainv_true[ii],1e-3);
  delete linAlgClass;

}

// Test for mat mult
TEST_CASE(matMult) {

  linAlg *linAlgClass;
  linAlgClass = new linAlg;
  const int m=3, n=5, k=2;
  vector<double> A{4,-1,7,2,-6,1};//3x2
  vector<double> B{7.8,1.9,-0.5,12.7,9.4,-11.2,1.1,-8.9,15.7,-0.22};//2x5
  vector<double> C_true{35,19.2,56.5,23.4,-75.69999999999999,9.2,15.2,57.799999,54.59999,-13.4,52.3,-1.19999999999,62.36,-14.37999999999,1.096800000000000e+02};
  vector<double> C;
  C.resize(m*n);
  linAlgClass->matMult(m, n, k, A, B, C);
  for (int ii=0;ii<m*n;ii++) ASSERT_NEAR(C[ii],C[ii],1e-5);
  delete linAlgClass;

}


// Test for mat vec mult
TEST_CASE(matVec) {

  linAlg *linAlgClass;
  linAlgClass = new linAlg;
  const int m=3, n=1, k=2;
  vector<double> A{4,-1,7,2,-6,1};//3x2
  vector<double> B{7.8,1.9,-0.5};//2x1
  vector<double> C_true{35,19.2,56.5};
  vector<double> C;
  C.resize(m*n);
  linAlgClass->matMult(m, n, k, A, B, C);
  for (int ii=0;ii<m*n;ii++) ASSERT_NEAR(C[ii],C[ii],1e-5);
  delete linAlgClass;

}
#endif
int main()
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  #if BLAS_FOUND
  TEST(pseudoInv);
  TEST(matMult);
  TEST(pseudoInvNGrM);
  TEST(matVec);
  #endif
  return 0; 
}
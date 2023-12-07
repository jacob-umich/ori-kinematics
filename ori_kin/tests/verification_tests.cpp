#include "ori_kin.hpp"
#include "unit_test_framework.hpp"

// Test for pseudo inv and mat mult sequentially
TEST_CASE(linAlgSequential) {
    linAlg *linAlgClass;
    linAlgClass = new linAlg;
    const int m=2, n=4;
    vector<double> A = {1,7,2,2,3,9,4,-4};
    vector<double> Ainv_true = {-9.960159362549636e-04,0.063745019920319,0.062749003984064,0.171314741035856,0.046812749003984,0.003984063745020,0.050796812749004,-0.051792828685259};
    vector<double> AinvATrue = {1,0,0,1};
    vector<double> Ainv;
    vector<double> AinvA;
    Ainv.resize(m*n);
    AinvA.resize(m*n);
    linAlgClass->matPseudoInv(m, n, A, Ainv);
    linAlgClass->matMult(m, m,n, A, Ainv, AinvA);

    for (int i=0; i<m*n; i++) ASSERT_NEAR(Ainv[i],Ainv_true[i],1e-5);
    for (int i=0; i<m*m; i++) ASSERT_NEAR(AinvA[i],AinvATrue[i],1e-5);

    delete linAlgClass;
}
// test projection using linear algebra routines
TEST_CASE(make_projection){
    linAlg linlib; 
    // vector<double> cnst_mat = {0.18257419,0.54162708, 0.36514837,0.04609592, 0.54772256,0.587723, 0.73029674,-0.59924698};
    vector<double> cnst_mat = {1,7,2,2,3,9,4,-4};
    vector<double> pinv_true = {-9.960159362549636e-04,0.063745019920319,0.062749003984064,0.171314741035856,0.046812749003984,0.003984063745020,0.050796812749004,-0.051792828685259};
    vector<double>projectorTrue = {0.3267,0.0916,0.4183, -0.1912,0.0916,0.1355,0.2271,0.2390,0.4183,0.2271,0.6454,0.0478,-0.1912,0.2390,0.0478,0.8924};
    vector<double> projector(16,0.0);
    vector<double> pinv(8,0.0);
    linlib.matPseudoInv(2,4,cnst_mat,pinv);//cnst is 2x4 pinv is 4x2
    for (int i=0;i<8;i++){
        ASSERT_NEAR(pinv[i],pinv_true[i],1e-5);
    }
    linlib.matMult(4,4,2,pinv,cnst_mat,projector);
    for (int i=0;i<16;i++){
        ASSERT_NEAR(projector[i],projectorTrue[i],1e-3);
    }
    vector<double>vel = {0,0,0,0};
    vector<double>targetVel= {0,0,0,1};
    vector<double>velTrue= { 0.19123506, -0.23904382, -0.04780876,  0.10756972};
    
    for (int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            int ident;
            if (i==j){
                ident=1;
            }else{
                ident=0;
            }
            vel[i]=vel[i]+(ident-projector[j*4+i])*targetVel[j];
        }

    }
    for (int i=0;i<4;i++){
        //std::cout<<vel[i]<<"  "<<velTrue[i]<<std::endl;
        ASSERT_NEAR(vel[i],velTrue[i],1e-5);
    }
}
// test for if angular velocity is orthogonal to current vector
//TEST_CASE(orthoVec) {
//}
//small error in change of edge lengths
TEST_CASE(rigidEdges) {
    int nsteps=50;
    Okin::Structure cube("testMesh.json",0.001);
    //cube.simulate("euler");
    auto firstEdge=cube._edges.begin();
    double original_length=(*firstEdge)->length;
    std::cout<<"edge length: "<<original_length<<std::endl;
    //Okin::Edge *edgeOfInt= cube.getEdge(1);
    for (int iter=0;iter<=nsteps;iter++){
        cube.genConstraints();
        linAlg linlib; 
        vector<double> targetVel = cube.getNextTarVelocity();
        vector<double> vel(cube.coordinate,0.0);
        vector<double> projector(cube.coordinate*cube.coordinate,0.0);
        vector<double> pinv(cube.coordinate*cube.n_const,0.0);
        linlib.matPseudoInv(cube.n_const,cube.coordinate,cube.cnst_mat,pinv);
        linlib.matMult(cube.coordinate,cube.coordinate,cube.n_const,pinv,cube.cnst_mat,projector);
        for (int i=0;i<cube.coordinate;i++){
            for(int j=0;j<cube.coordinate;j++){
                int ident;
                if (i==j){
                    ident=1;
                }else{
                    ident=0;
                }
                vel[i]=vel[i]+(ident-projector[j*cube.coordinate+i])*targetVel[j];
            }

        }
        cube.eulerIntegrate(vel);
        for(auto edge=cube._edges.begin();edge!=cube._edges.end();edge++){
            (*edge)->updatePos();
        }
        std::cout<<original_length-(*firstEdge)->length<<std::endl;
        assert(original_length-(*firstEdge)->length<0.001);
        //calculate change in edge
        
    }
}
int main()
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  #if BLAS_FOUND
    TEST(linAlgSequential);
    TEST(make_projection);
    TEST(rigidEdges);
  #endif
  return 0; 
}


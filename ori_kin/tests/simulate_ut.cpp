#include "ori_kin.hpp"
#include "unit_test_framework.hpp"
void normalize(std::vector<double>& a){
    double dot=0;
    for (int i =0;i<4;i++){
        dot+=a[i]*a[i];
    }
    dot = sqrt(dot);
    for (int i =0;i<4;i++){
        a[i]=a[i]/dot;
    }
}
TEST_CASE(projection){
    vector<double>vel = {0,0,0,0};
    vector<double>projector = { 0.32669323,  0.09163347,  0.41832669, -0.19123506, 0.09163347,  0.13545817 , 0.22709163,  0.23904382, 0.41832669,  0.22709163,  0.64541833,  0.04780876,-0.19123506,  0.23904382,  0.04780876,  0.89243028};
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
        ASSERT_NEAR(vel[i],velTrue[i],1e-5);
    }
}
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
    normalize(vel);
    normalize(velTrue);
    for (int i=0;i<4;i++){
        //std::cout<<vel[i]<<"  "<<velTrue[i]<<std::endl;
        ASSERT_NEAR(vel[i],velTrue[i],1e-5);
    }
}

int main(){
    TEST(projection);
    TEST(make_projection);
    return 0;
}

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

int main(){
    TEST(projection);
    return 0;
}

#include "ori_kin.hpp"
#include "unit_test_framework.hpp"
void normalize(std::vector<double>& );
std::vector<double> project(std::vector<double>,std::vector<double>);

TEST_CASE(velocityVectorInit){
    Okin::Structure tester("testMesh.json",0.1);
    JSONParser parser("testMesh.json");
    parser.parse();
    Okin::tVelocity testvel (parser.root->returnObject()["target_velocities"]->returnList()[0]->returnObject(),tester);
    assert(testvel.start==0);
    assert(testvel.end==10);
    ASSERT_NEAR(testvel.stepSize,0.001,1e-10);
    assert(testvel.type=="vector");
}
TEST_CASE(velocityAngleInit){
    Okin::Structure tester("testMesh.json",0.1);
    JSONParser parser("testMesh.json");
    parser.parse();
    Okin::tVelocity testvel (parser.root->returnObject()["target_velocities"]->returnList()[1]->returnObject(),tester);
    assert(testvel.start==10);
    assert(testvel.end==100);
    ASSERT_NEAR(testvel.stepSize,0.001,1e-10);
    assert(testvel.type=="angular");
    assert(testvel.refNode->_id==4);
    assert(testvel.targetNode->_id==5);
    assert((testvel.joint2->_id==6 || testvel.joint2->_id==7));
}

TEST_CASE(normalizeTest){
    vector<double> a({8,8,0});
    normalize(a);
    ASSERT_NEAR(a[0],0.707,1e-2);
    ASSERT_NEAR(a[1],0.707,1e-2);
    ASSERT_NEAR(a[2],0,1e-2);


}
TEST_CASE(projectTest){
    vector<double> a({8,0,0});
    vector<double> b({0,8,0});
    vector<double> c = project(a,b);
    ASSERT_NEAR(c[0],8,1e-2);
    ASSERT_NEAR(c[1],0,1e-2);
    ASSERT_NEAR(c[2],0,1e-2);

    
}
TEST_CASE(updateAngle){
    Okin::Structure tester("testMesh.json",0.1);
    JSONParser parser("testMesh.json");
    parser.parse();
    Okin::tVelocity testvel (parser.root->returnObject()["target_velocities"]->returnList()[1]->returnObject(),tester);
    vector<double> vel = testvel.getVelocity();
    ASSERT_NEAR(vel[0],0,1e-10);
    ASSERT_NEAR(vel[1],0,1e-10);
    ASSERT_NEAR(vel[2],1,1e-10);

    testvel.targetNode->_position[0]=1.0001;
    testvel.targetNode->_position[2]=1.1;
    vel = testvel.getVelocity();
    ASSERT_NEAR(vel[0],0,1e-2);
    ASSERT_NEAR(vel[1],0,1e-2);
    ASSERT_NEAR(vel[2],0,1e-2);

    testvel.targetNode->_position[0]=1.02;
    testvel.targetNode->_position[2]=1.1;
    vel = testvel.getVelocity();
    ASSERT_NEAR(vel[0],-1,1e-2);
    ASSERT_NEAR(vel[1],0,1e-2);
    ASSERT_NEAR(vel[2],0.019,1e-2);

    testvel.targetNode->_position[0]=.98;
    testvel.targetNode->_position[2]=1.1;
    vel = testvel.getVelocity();
    ASSERT_NEAR(vel[0],1,1e-2);
    ASSERT_NEAR(vel[1],0,1e-2);
    ASSERT_NEAR(vel[2],0.019,1e-2);


}
TEST_CASE(strucutreVelAccess){
    Okin::Structure tester("testMesh.json",0.1);
    vector<double> ret;
    ret.resize(tester.coordinate);
    ret = tester.getNextTarVelocity();
    ASSERT_NEAR(ret[24],10,1e-2);
    ASSERT_NEAR(ret[25],0,1e-2);
    ASSERT_NEAR(ret[26],0,1e-2);
    for(int i=0;i<9;i++){
        ret= tester.getNextTarVelocity();
    }
    ret = tester.getNextTarVelocity();
    ASSERT_NEAR(ret[36],0,1e-2);
    ASSERT_NEAR(ret[37],0,1e-2);
    ASSERT_NEAR(ret[38],1,1e-2);
}

int main(){
    TEST(velocityVectorInit);
    TEST(velocityAngleInit);
    TEST(normalizeTest);
    TEST(projectTest);
    TEST(updateAngle);
    TEST(strucutreVelAccess);
    return 0;
}
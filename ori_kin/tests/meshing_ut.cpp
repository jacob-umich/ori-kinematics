#include "ori_kin.hpp"
#include "unit_test_framework.hpp"


TEST_CASE(nodeInit){

    Okin::Structure tester("testMesh.json");
    ASSERT_NEAR(tester.getNode(0)->_position[0],0,1e-10);
    ASSERT_NEAR(tester.getNode(0)->_position[1],0,1e-10);
    ASSERT_NEAR(tester.getNode(0)->_position[2],0,1e-10);
    assert(tester.getNode(0)->_fixities[0]==1);
    assert(tester.getNode(0)->_fixities[1]==1);
    assert(tester.getNode(0)->_fixities[2]==1);
    assert(tester.getNode(1)->_fixities[0]==0);
    assert(tester.getNode(1)->_fixities[1]==0);
    assert(tester.getNode(1)->_fixities[2]==0);
    assert(tester.getNode(0)->_id==0);
}
TEST_CASE(nodeSub){
    Okin::Structure tester("testMesh.json");
    vector<double> res = *tester.getNode(1)-*tester.getNode(0);
    ASSERT_NEAR(res[0],1,1e-10);
    ASSERT_NEAR(res[1],0,1e-10);
    ASSERT_NEAR(res[2],0,1e-10);

}
TEST_CASE(nodeComp){
    Okin::Structure tester("testMesh.json");
    bool res = *tester.getNode(1)>*tester.getNode(0);
    assert(res);
    res = *tester.getNode(0)<*tester.getNode(1);
    assert(res);
}

TEST_CASE(edgeInit){
    Okin::Structure tester("testMesh.json");
    Okin::Edge test (tester.getNode(0),tester.getNode(1));
    ASSERT_NEAR(test.length,1,1e-10);
    assert(test._nodes[0]->_id==0);
    assert(test._nodes[1]->_id==1);
    ASSERT_NEAR(test.start_pos[0],0,1e-10);
    ASSERT_NEAR(test.start_pos[1],0,1e-10);
    ASSERT_NEAR(test.start_pos[2],0,1e-10);
    ASSERT_NEAR(test.end_pos[0],1,1e-10);
    ASSERT_NEAR(test.end_pos[1],0,1e-10);
    ASSERT_NEAR(test.end_pos[2],0,1e-10);
    ASSERT_NEAR(test.dir[0],1,1e-10);
    ASSERT_NEAR(test.dir[1],0,1e-10);
    ASSERT_NEAR(test.dir[2],0,1e-10);

}

TEST_CASE(edgeUpdate){
    Okin::Structure tester("testMesh.json");
    Okin::Edge test (tester.getNode(0),tester.getNode(1));
    test._nodes[1]->_position[0]=8;
    test.updatePos();
    ASSERT_NEAR(test.length,8,1e-10);
    ASSERT_NEAR(test.end_pos[0],8,1e-10);
    ASSERT_NEAR(test.end_pos[1],0,1e-10);
    ASSERT_NEAR(test.end_pos[2],0,1e-10);
    ASSERT_NEAR(test.dir[0],8,1e-10);
    ASSERT_NEAR(test.dir[1],0,1e-10);
    ASSERT_NEAR(test.dir[2],0,1e-10);

}

TEST_CASE(bodyInit){
    Okin::Structure tester("testMesh.json");
    Okin::Body *test = tester.getBody(0);
    assert(test->_id==0);
    assert(test->_edges.size()==18); //minimally rigid rect-prism
    assert(test->faces.size()==6);
    assert(test->faces[0][0]==0);
    assert(test->faces[0][1]==1);
    assert(test->faces[0][2]==2);
    assert(test->faces[0][3]==3);
}
TEST_CASE(nodeOrder){

    Okin::Structure tester("testMesh.json");
    assert(tester.getNode(4)->_id==4);
    ASSERT_NEAR(tester.getNode(0)->_position[0],0,1e-10);
    ASSERT_NEAR(tester.getNode(0)->_position[1],0,1e-10);
    ASSERT_NEAR(tester.getNode(0)->_position[2],0,1e-10);
    ASSERT_NEAR(tester.getNode(8)->_position[0],1,1e-10);
    ASSERT_NEAR(tester.getNode(8)->_position[1],0,1e-10);
    ASSERT_NEAR(tester.getNode(8)->_position[2],0,1e-10);
}


TEST_CASE(allHere){
    Okin::Structure tester("testMesh.json");
    assert(tester.getNNode()==16);
    assert(tester.getNBody()==2);

}

TEST_CASE(allCoordinated){
    //test that all nodes are coordinated

    Okin::Structure tester("testMesh.json");
    size_t nNodes = tester.getNNode();
    for (size_t i=0;i<nNodes;i++){
        assert(tester.getNode(i)->coordinated);
    }
    assert(tester.getNCoords()==42);
}
TEST_CASE(jointDefined){
    //test that joints are properly defined
    Okin::Structure tester("testMesh.json");
    Okin::Node * node_1 = tester.getBody(0)->_nodes[5];
    Okin::Node * node_2 = tester.getBody(1)->_nodes[4];
    assert(node_1->coordinates[0]==node_2->coordinates[0]);
    assert(node_1->coordinates[1]==node_2->coordinates[1]);
    assert(node_1->coordinates[2]==node_2->coordinates[2]);
    node_1 = tester.getBody(0)->_nodes[6];
    node_2 = tester.getBody(1)->_nodes[7];
    assert(node_1->coordinates[0]==node_2->coordinates[0]);
    assert(node_1->coordinates[1]==node_2->coordinates[1]);
    assert(node_1->coordinates[2]==node_2->coordinates[2]);

    assert(tester.getJoint(0).first.first==0);
    assert(tester.getJoint(0).first.second==1);
    assert(tester.getJoint(0).second.first==5);
    assert(tester.getJoint(0).second.second==4);
    assert(tester.getJoint(1).second.first==6);
    assert(tester.getJoint(1).second.second==7);
}

TEST_CASE(compareJsonAddr){
    Okin::Structure tester("testMesh.json");
    assert(tester.parser.root.get()==tester.root.get());

    JSONObject* obj1 = tester.parser.root->returnObjectPtr();
    JSONObject* obj2 = tester.root->returnObjectPtr();
    JSONObject* obj3 = tester.root->returnObjectPtr();
    assert(obj1==obj2);
    assert(obj1==obj3);
    assert(tester.jStructure=obj3);
    JSONList * bodies = (*obj2)["bodies"]->returnListPtr();
    JSONObject* body1 = (*bodies)[0]->returnObjectPtr();
    JSONList * nodes = (*body1)["nodes"]->returnListPtr();
    JSONObject * node1 = (*nodes)[0]->returnObjectPtr();
    assert(tester.getNode(0)->jNode==node1);
}



// verify angular velocity
// verify vector velocity

int main(){
    TEST(nodeOrder);
    TEST(allHere);
    TEST(nodeInit);
    TEST(nodeSub);
    TEST(nodeComp);
    TEST(edgeInit);
    TEST(edgeUpdate);
    TEST(bodyInit);
    TEST(allCoordinated);
    TEST(jointDefined);
    TEST(compareJsonAddr);
    return 0;
}
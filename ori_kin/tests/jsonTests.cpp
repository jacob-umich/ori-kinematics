#include "ori_kin.hpp"
#include "unit_test_framework.hpp"

// TODO: make a test to check file type (have to implement feature first)
TEST_CASE(stringParse){
    JSONParser parser("jsonTest.json");
    parser.parse();
    assert(parser.root->returnObject()["foo"]->returnString()=="bar");
}
TEST_CASE(listParse){
    JSONParser parser("jsonTest.json");
    parser.parse();
    assert(parser.root->returnObject()["baz"]->returnList().size()==3);
}
TEST_CASE(nestObjParse){
    try{
        JSONParser parser("jsonTest.json");
        parser.parse();
        JSONObject obj = parser.root->returnObject()["more_objects"]->returnList()[0]->returnObject();
        assert(true);
    } catch (std::logic_error){
        assert(false);
    }
}
TEST_CASE(nestNumParse){
    try{
        JSONParser parser("jsonTest.json");
        parser.parse();
        JSONObject obj = parser.root->returnObject()["more_objects"]->returnList()[0]->returnObject();
        ASSERT_NEAR(obj["obj1"]->returnNumber(),10,0.000001);
    } catch (std::logic_error){
        assert(false);
    }
}
TEST_CASE(trueParse){
    try{
        JSONParser parser("jsonTest.json");
        parser.parse();
        JSONObject obj = parser.root->returnObject()["more_objects"]->returnList()[1]->returnObject();
        assert(obj["obj2"]->returnList()[0]->returnBool());
    } catch (std::logic_error){
        assert(false);
    }
}
TEST_CASE(falseParse){
    try{
        JSONParser parser("jsonTest.json");
        parser.parse();
        JSONObject obj = parser.root->returnObject()["more_objects"]->returnList()[1]->returnObject();
        assert(!(obj["obj2"]->returnList()[1]->returnBool()));
    } catch (std::logic_error){
        assert(false);
    }
}
TEST_CASE(multiNestParse){
    try{
        JSONParser parser("jsonTest.json");
        parser.parse();
        JSONObject obj = parser.root->returnObject()["more_objects"]->returnList()[1]->returnObject();
        JSONObject obj2=obj["obj2"]->returnList()[2]->returnObject();
        assert(obj2["type"]->returnString()=="sigmoid");
    } catch (std::logic_error){
        assert(false);
    }
}
TEST_CASE(badParse){
    try{
        JSONParser parser("jsonTest.json");
        parser.parse();
        assert(parser.root->returnObject()["baz"]->returnString().size()==3);

    } catch (std::logic_error){
        assert(true);
    }
}

int main(){
    TEST(stringParse);
    TEST(listParse);
    TEST(nestObjParse);
    TEST(nestNumParse);
    TEST(trueParse);
    TEST(falseParse);
    TEST(multiNestParse);
    TEST(badParse);

}
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
    TEST(badParse)

}
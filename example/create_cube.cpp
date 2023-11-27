#include "ori_ken.hpp"
#include "jsonParser.hpp"

int main(){
    char in[] = "[{pos:[0,0,5]}]";
    Okin::Structure cube {in};
    
    JSONParser parser("./cube.json");
    parser.parse();
    // cube.printNodes();
    return 0;
}
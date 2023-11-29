#include "ori_ken.hpp"

int main(){

    Okin::Structure cube {"./cube.json"};
    cube.generateConstraints();
    return 0;
}
#include "ori_kin.hpp"

int main(){

    Okin::Structure cube {"./cube.json"};
    cube.genConstraints();
    return 0;
}
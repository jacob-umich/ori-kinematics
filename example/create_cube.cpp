#include "ori_kin.hpp"

int main(){

    Okin::Structure cube {"./cube.json",0.005};
    cube.simulate("euler");
    cube.save("cube_out.json");



    return 0;
}
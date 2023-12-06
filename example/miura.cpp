#include "ori_kin.hpp"

int main(){

    Okin::Structure cube {"./miura.json",0.05};
    cube.simulate("euler");
    cube.save("miura_out.json");



    return 0;
}
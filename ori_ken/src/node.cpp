#include "ori_ken.hpp"
#include <iostream>

namespace Okin
{
    Node::Node(double * position){
        _position=position;
    }
    Node::Node(){
        double def_array[]{0.0, 0.0 ,0.0};
        _position=def_array;
    }

    void Node::printPos(){
        std::cout<< _position[0]<< _position[1]<< _position[2]<<std::endl;
    }
} // namespace Okin

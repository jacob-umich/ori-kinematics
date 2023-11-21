#include "ori_ken.hpp"
#include <iostream>
#include <vector>
using std::vector;
namespace Okin
{
    Node::Node(vector<double>& position){
        _position=position;
    }
    Node::Node(){
        _position=vector<double>{3,0.0};
    }

    void Node::printPos(){
        std::cout<< _position[0]<< _position[1]<< _position[2]<<std::endl;
    }
} // namespace Okin

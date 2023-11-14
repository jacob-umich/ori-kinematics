#include "ori_ken.hpp"

namespace Okin
{
    Node::Node(double * position){
        _position=position;
    }
    Node::Node(){
        double def_array[]{0.0, 0.0 ,0.0};
        _position=def_array;
    }
} // namespace Okin

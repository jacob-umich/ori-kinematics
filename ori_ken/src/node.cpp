#include "ori_ken.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
using std::vector;
namespace Okin
{
    double toDPos(std::shared_ptr<JSONNode> posNode){
        return posNode->returnNumber();
    }
    Node::Node(JSONObject jNode){
        JSONList jPos = jNode["pos"]->returnList();
        _position.resize(3);
        std::transform(jPos.begin(),jPos.end(),_position.begin(),toDPos);
    }
    Node::Node(){
        _position=vector<double>{3,0.0};
    }

    void Node::printPos(){
        std::cout<< _position[0]<< _position[1]<< _position[2]<<std::endl;
    }
} // namespace Okin

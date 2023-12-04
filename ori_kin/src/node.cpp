#include "ori_kin.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
using std::vector;
namespace Okin
{
    double toDPos(std::shared_ptr<JSONNode> posNode){
        return posNode->returnNumber();
    }
    double toI(std::shared_ptr<JSONNode> posNode){
        return int(posNode->returnNumber());
    }
    Node::Node(JSONObject jNode){
        JSONList jPos = jNode["pos"]->returnList();
        _position.resize(3);
        _fixities=vector<int> (3,0);
        std::transform(jPos.begin(),jPos.end(),_position.begin(),toDPos);
        _id = int(jNode["id"]->returnNumber());
        if (jNode.find("fix") == jNode.end()){
            _fixities=std::vector<int>(3,0);
        } else {
            JSONList jFix = jNode["fix"]->returnList();
            std::transform(jFix.begin(),jFix.end(),_fixities.begin(),toI);
        }
        coordinated=false;
        _positionHist.resize(1);
        _positionHist[0]=_position;

    }

    void Node::printPos(){
        std::cout<< _position[0]<< _position[1]<< _position[2]<<std::endl;
    }
    void Node::printFix(){
        std::cout<< _fixities[0]<< _fixities[1]<< _fixities[2]<<std::endl;
    }
    vector<double> Node::operator-(Node input){
        vector<double>res{0.0,0.0,0.0};
        for (int i=0;i<3;i++){
            res[i]=_position[i]-input._position[i];
        }
        return res;

    }
    bool Node::operator<(Node j){
        return (j._id>_id);
       
    }
    bool Node::operator>(Node j){
        return (j._id<_id);
       
    }
} // namespace Okin

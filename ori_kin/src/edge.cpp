#include "edge.hpp"
#include <cmath>

namespace Okin
{
    Edge::Edge(Node *a, Node *b){
        _nodes.push_back(a);
        _nodes.push_back(b);
        updatePos();
    }

    void Edge::updatePos(){
        start_pos = _nodes[0]->_position;
        end_pos = _nodes[1]->_position;
        dir = *_nodes[1]-*_nodes[0];
        length=0;
        for(int i=0;i<3;i++){
            length+=dir[i]*dir[i];
        }
        length=std::sqrt(length);
    }
} // namespace Okin

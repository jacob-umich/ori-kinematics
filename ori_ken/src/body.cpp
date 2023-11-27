#include "body.hpp"

namespace Okin{
    Body::Body(JSONObject jBody){
        _id=int(jBody["id"]->returnNumber());
        JSONList jNodes =  jBody["nodes"]->returnList();
        size_t numNodes =jNodes.size();

        for (size_t i=0;i<numNodes;i++){
            JSONObject jNode=jNodes[i]->returnObject();
            Node newNode(jNode);
            _nodes.push_back(&newNode);
        }
    }
}
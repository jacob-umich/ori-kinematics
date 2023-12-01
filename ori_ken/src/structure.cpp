#include "structure.hpp"
#include "node.hpp"
#include "edge.hpp"
#include <regex>
#include <string>
#include <iostream>

namespace Okin {
    Structure::Structure(const std::string inputFile): parser(inputFile){
        parser.parse();
        root = parser.root;
        JSONObject jStructure = root->returnObject();
        JSONList jBodies =  jStructure["bodies"]->returnList();
        size_t numBodies =jBodies.size();

        for (size_t i=0;i<numBodies;i++){
            JSONObject jBody=jBodies[i]->returnObject();
            Body *newBody = new Body(jBody);
            _bodies.push_back(newBody);
            for (auto node=newBody->_nodes.begin();node!=newBody->_nodes.end();node++){
                _nodes.push_back(*node);
            }
            for (auto edge=newBody->_edges.begin();edge!=newBody->_edges.end();edge++){
                _edges.push_back(*edge);
            }
        }
        coordinate = 0;
        JSONList jJoints =  jStructure["joints"]->returnList();
        for (auto body=_bodies.begin();body!=_bodies.end();body++){
            for (auto node=(*body)->_nodes.begin();node!=(*body)->_nodes.end();node++){
                if (!(*node)->coordinated){
                    (*node)->coordinates.resize(3);
                    (*node)->coordinates[0]=coordinate++;
                    (*node)->coordinates[1]=coordinate++;
                    (*node)->coordinates[2]=coordinate++;
                }
            }
            for (auto jObj=jJoints.begin();jObj!=jJoints.end();jObj++){
                JSONList joinedBodies = (*jObj)->returnObject()["bodies"]->returnList();
                if (int(joinedBodies[0]->returnNumber())==(*body)->_id){
                    size_t nJoinBod=joinedBodies.size();
                    JSONList joinedNodes = (*jObj)->returnObject()["nodes"]->returnList();
                    for (size_t j=1;j<nJoinBod;j++){
                        int nodeNumLocal = int(joinedNodes[j]->returnNumber());
                        int nodeNumCopy = int(joinedNodes[0]->returnNumber());
                        _bodies[int(joinedBodies[j]->returnNumber())]->_nodes[nodeNumLocal]->coordinates =(*body)->_nodes[nodeNumCopy]->coordinates ;
                        _bodies[int(joinedBodies[j]->returnNumber())]->_nodes[nodeNumLocal]->coordinated = true;

                    }
                }
            }
        }
    }
}
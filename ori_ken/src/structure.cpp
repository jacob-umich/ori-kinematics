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
            idB=jBody["id"]->returnNumber();
            Body newBody(jBody);
            _bodies.push_back(&newBody);
        }

        // std::regex pattern {"[0-9.]+"};
        // // char * test = end(input);
        // auto value = std::cregex_iterator(input,input+15,pattern);   
        // auto valueEnd = std::cregex_iterator();   
        // double * position = new double[3]();
        // int count = 0;
        // for (std::cregex_iterator i=value;i!=valueEnd;i++){
        //     position[count]=stod((*i).str());
        //     count++;
        // }
    }

    // void Structure::printNodes(){
    //     for (int i=0;i<_nNodes;i++){
    //         _nodes[i].printPos();
    //     }
    // }
}
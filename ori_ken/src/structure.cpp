#include "structure.hpp"
#include "body.hpp"
#include "node.hpp"
#include "edge.hpp"
#include <regex>
#include <string>

namespace Okin {
    Structure::Structure(char * input){
        std::regex pattern {"[0-9.]+"};
        // char * test = end(input);
        auto value = std::cregex_iterator(input,input+15,pattern);   
        auto valueEnd = std::cregex_iterator();   
        double * position = new double[3]();
        int count = 0;
        for (std::cregex_iterator i=value;i!=valueEnd;i++){
            position[count]=stod((*i).str());
            count++;
        }
    }

    void Structure::printNodes(){
        for (int i=0;i<_nNodes;i++){
            _nodes[i].printPos();
        }
    }
}
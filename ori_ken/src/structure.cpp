#include "structure.hpp"
#include "body.hpp"
#include "node.hpp"
#include "edge.hpp"
#include <regex>
#include <string>

namespace std {
    Structure::Structure(char * input){
        regex pattern {"[0-9.]+"};
        // char * test = end(input);
        auto value = cregex_iterator(input,input+10,pattern);   
        auto valueEnd = cregex_iterator();   
        double * position = new double[3]();
        int count = 0;
        for (cregex_iterator i=value;i!=valueEnd;i++){
            position[count]=stod((*i).str());
            count++;
        }
    }

    void Structure::printNodes(){
        for (int i;i<_nNodes;i++){
            _nodes[i].printPos();
        }
    }
}
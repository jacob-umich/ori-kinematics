#include "structure.hpp"
#include "body.hpp"
#include "node.hpp"
#include "edge.hpp"
#include <regex>

namespace std {
    Structure::Structure(char * input){
        regex pattern {"[0-9.]+"};
        cmatch m;
        bool value = regex_search(input, m, pattern);
        double * position = new double[3]();
        for (int i=0;i<3;i++){
            position[i]=atof(m.iterator[i]);
        }
    }

    void Structure::printNodes(){
        for (int i;i<_nNodes;i++){
            _nodes[i].printPos();
        }
    }
}
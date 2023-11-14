#include "structure.hpp"

namespace Okin {
    Structure::Structure(char * input){
        Body ** this->_bodies;
        Edge ** this->_edges;
        Node ** this->_nodes;
    }
}
#include <iostream>
#include <vector>
#include "node.hpp"
#include "body.hpp"
#include "jsonParser.hpp"
#include "linAlg.hpp"

#pragma once

using std::vector;
namespace Okin{
    class Structure{
        using bodyList = std::vector<Body*>;
        using nodeList = std::vector<Node*>;
        using edgeList = std::vector<Edge*>;

        protected:
            char * name;
            int id;
            nodeList _nodes;
            bodyList _bodies;
            edgeList _edges;
            int _nNodes;
        private:
            size_t n_bodies;
            JSONParser parser;
            std::shared_ptr<JSONNode> root;
            int coordinate;
            vector<double> cnst_mat;

        public:
            Structure(std::string inputFile);
            double idB;
            void printNodes();
            //generate constraints
            void genConstraints();

    };


}
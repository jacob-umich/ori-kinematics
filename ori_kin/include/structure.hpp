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

        public:
            Structure(std::string inputFile);
            double idB;
            vector<double> cnst_mat;
            void printNodes();
            Node * getNode(int);
            Body * getBody(int);
            size_t getNNode();
            size_t getNBody();
            size_t getNCoords();
            //generate constraints
            void genConstraints();

    };


}
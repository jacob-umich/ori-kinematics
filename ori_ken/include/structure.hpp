#include <iostream>
#include <vector>
#include "node.hpp"
#include "body.hpp"
#include "jsonParser.hpp"

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
            void genBodies();
            void genNodes();
            void genEdge();

        public:
            Structure(std::string inputFile);
            double idB;
            void printNodes();
            //generate constraints
            void generateConstraints(vector<vector<double>> &nodes,
                vector<vector<int>> &edges, size_t nnodes,size_t nedges,
                vector<vector<int>> &const_nodes,vector<double> &C, vector<int> &colIdx, vector<int> &rowIdx);

    };


}
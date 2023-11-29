#include <iostream>
#include <vector>
#include "node.hpp"
#include "linAlg.hpp"

#pragma once

using std::vector;
namespace Okin{
    class Structure{

        protected:
            char * name;
            int id;
            Node * _nodes;
            int _nNodes;
        private:
            size_t n_bodies;
            void genBodies();
            void genNodes();
            void genEdge();
        public:
            Structure(char * input);
            void printNodes();
            //generate constraints
            void genConstraints(vector<vector<double>> &nodes,
    vector<vector<int>> &edges,vector<int> &glob_cnst, vector<double> cnst_mat);

    };


}
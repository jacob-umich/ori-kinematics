#include <iostream>
#include <vector>

#pragma once

namespace std{
    class structure{

        protected:
            char * name;
            int id;
        private:
            size_t n_bodies;
        public:
<<<<<<< HEAD
            
=======
            //generate constraints
            void generateConstraints(vector<vector<double>> &nodes,
                vector<vector<int>> &edges, size_t nnodes,size_t nedges,
                vector<vector<int>> &const_nodes,vector<double> &C, vector<int> &colIdx, vector<int> &rowIdx);

>>>>>>> origin/constraints
    };
}
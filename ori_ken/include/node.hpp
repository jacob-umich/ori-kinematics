#pragma once
#include <vector>
using std::vector;
namespace Okin{
    class Node{

        protected:
            char * name;
            vector<double> _position;
            Node * neighbors;
            int * fixities;
        private:
            size_t n_bodies;
        public:
            Node(vector<double>& position);
            Node();
            void printPos();

    };
}
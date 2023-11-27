#pragma once
#include <vector>
#include "jsonParser.hpp"
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
            Node (JSONObject);
            Node();
            void printPos();
            // double toDPos(JSONNode*);

    };
}
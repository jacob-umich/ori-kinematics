#pragma once
#include <vector>
#include "jsonParser.hpp"
using std::vector;
namespace Okin{
    class Node{

        public:
            int _id;
            vector<Node*> neighbors;
            vector<int> _fixities;
            vector<double> _position;
            Node (JSONObject);
            Node();
            void printPos();
            void printFix();
            vector<double> operator-(Node);
            // double toDPos(JSONNode*);

    };
}
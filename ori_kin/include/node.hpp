#pragma once
#include <vector>
#include "jsonParser.hpp"
using std::vector;
namespace Okin{
    class Node{

        public:
            int _id;
            bool coordinated;
            vector<int> coordinates;
            vector<Node*> neighbors;
            vector<int> _fixities;
            vector<double> _position;
            Node (JSONObject);
            void printPos();
            void printFix();
            vector<double> operator-(Node);
            bool operator<(Node);
            bool operator>(Node);
            // double toDPos(JSONNode*);

    };
}
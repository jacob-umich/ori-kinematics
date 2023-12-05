#pragma once
#include <vector>
#include "jsonParser.hpp"
using std::vector;
namespace Okin{
    class Node{

        public:
            int _id;
            int idg;
            JSONObject* jNode;
            bool coordinated;
            vector<int> coordinates;
            vector<Node*> neighbors;
            vector<int> _fixities;
            vector<double> _position;
            vector<vector<double>> _positionHist;
            Node (JSONObject*);
            void printPos();
            void printFix();
            vector<double> operator-(Node);
            bool operator<(Node);
            bool operator>(Node);
            void updateJSON();
            // double toDPos(JSONNode*);

    };
}
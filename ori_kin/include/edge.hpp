#include "node.hpp"

#pragma once
namespace Okin{
    class Edge{
        using nodeList = std::vector<Node*>;
        public:
            Edge(Node*,Node*);
            double length;
            vector<double> dir;
            nodeList _nodes;
            vector<double> start_pos, end_pos;
            void updatePos();


    };
}
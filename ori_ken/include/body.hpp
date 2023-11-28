#include "jsonParser.hpp"
#include "node.hpp"
#include "edge.hpp"

#pragma once
namespace Okin{
    class Body{
        using nodeList = std::vector<Node*>;
        using edgeList = std::vector<Edge*>;
        protected:
        public:
            char * name;
            std::vector<std::vector<int>> connectivityLocal;
            int _id;
            nodeList _nodes;
            edgeList _edges;
            Body(JSONObject);
            void printConnectivity();
            size_t nEdges;

    };
}
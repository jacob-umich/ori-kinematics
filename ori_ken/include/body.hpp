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
            std::vector<std::vector<int>> connectivityLocal;
            int _id;
            nodeList _nodes;
            edgeList _edges;
            Body(JSONObject);
            void printConnectivity();
            void printFace(std::vector<size_t>&);
            size_t nEdges;
            vector<vector<size_t>> faces;

    };
}
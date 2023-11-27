#include "jsonParser.hpp"
#include "node.hpp"
#pragma once
namespace Okin{
    class Body{
        using nodeList = std::vector<Node*>;
        protected:
            char * name;
            int _id;
            std::vector<std::vector<int>> connectivityLocal;
        private:
            nodeList _nodes;
        public:
            Body(JSONObject);

    };
}
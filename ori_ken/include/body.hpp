#include "jsonParser.hpp"
#include "node.hpp"
#pragma once
namespace Okin{
    class Body{
        using nodeList = std::vector<Node*>;
        protected:
            char * name;
            int _id;
        private:
            nodeList _nodes;
        public:
            Body(JSONObject);

    };
}
#pragma once
#include "structure.hpp"
#include "node.hpp"
#include <string>
#include <vector>
#include <stdexcept>

namespace Okin{
    class Structure;
    class tVelocity{
        public:
            std::string type;
            int start, end;
            Node * node;
            Node * refNode;
            Node * targetNode;
            Node * joint1;
            Node * joint2;
            double stepSize;
            double targetRatio;
            Structure& parent;
            std::vector<double> targetVelocity;
            tVelocity(JSONObject,Structure&);
            void updateFromAngle();
            std::vector<double> getVelocity();
    };
}
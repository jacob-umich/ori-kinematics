#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include "node.hpp"
#include "body.hpp"
#include "jsonParser.hpp"
#include "linAlg.hpp"
#include "targetVelocity.hpp"

using std::vector;
using std::pair;
namespace Okin{
    class tVelocity;
    class Structure{
        using bodyList = std::vector<Body*>;
        using nodeList = std::vector<Node*>;
        using edgeList = std::vector<Edge*>;
        using velList = std::vector<tVelocity*>;

        protected:
            char * name;
            int id;
            nodeList _nodes;
            bodyList _bodies;
            edgeList _edges;
            velList _tVels;
            int _nNodes;
        private:
            size_t n_bodies;
            JSONParser parser;
            std::shared_ptr<JSONNode> root;
            int coordinate;
            vector<double> cnst_mat;
            int n_const;
            int currentStep;
            double eulerStepSize;

        public:
            Structure(std::string inputFile);
            vector<pair<pair<int,int>,pair<int,int>>> _joints;
            int max_step;
            Node * getNode(int);
            Body * getBody(int);
            pair<pair<int,int>,pair<int,int>> getJoint(int);
            size_t getNNode();
            size_t getNBody();
            size_t getNCoords();
            vector<double> getSimStep(int i);
            //generate constraints
            void genConstraints();
            vector<double> getNextTarVelocity();
            void simulate(std::string);
            void eulerIntegrate(vector<double>&);

    };



}
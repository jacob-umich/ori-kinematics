#include "structure.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "linAlg.hpp"
#include "lapacke.h"
#include <regex>
#include <string>
#include <iostream>


namespace Okin {
    Structure::Structure(const std::string inputFile): parser(inputFile){
        eulerStepSize=0.001;
        currentStep = 0;
        parser.parse();
        root = parser.root;
        JSONObject jStructure = root->returnObject();
        JSONList jBodies =  jStructure["bodies"]->returnList();
        size_t numBodies =jBodies.size();

        for (size_t i=0;i<numBodies;i++){
            JSONObject jBody=jBodies[i]->returnObject();
            Body *newBody = new Body(jBody);
            _bodies.push_back(newBody);
            for (auto node=newBody->_nodes.begin();node!=newBody->_nodes.end();node++){
                _nodes.push_back(*node);
            }
            for (auto edge=newBody->_edges.begin();edge!=newBody->_edges.end();edge++){
                _edges.push_back(*edge);
            }
        }
        coordinate = 0;
        JSONList jJoints =  jStructure["joints"]->returnList();
        for (auto body=_bodies.begin();body!=_bodies.end();body++){
            for (auto node=(*body)->_nodes.begin();node!=(*body)->_nodes.end();node++){
                if (!(*node)->coordinated){
                    (*node)->coordinates.resize(3);
                    (*node)->coordinates[0]=coordinate++;
                    (*node)->coordinates[1]=coordinate++;
                    (*node)->coordinates[2]=coordinate++;
                    (*node)->coordinated=true;

                }
            }
            for (auto jObj=jJoints.begin();jObj!=jJoints.end();jObj++){
                JSONList joinedBodies = (*jObj)->returnObject()["bodies"]->returnList();
                if (int(joinedBodies[0]->returnNumber())==(*body)->_id){
                    size_t nJoinBod=joinedBodies.size();
                    JSONList joinedNodes = (*jObj)->returnObject()["nodes"]->returnList();
                    for (size_t j=1;j<nJoinBod;j++){
                        int nodeNumLocal = int(joinedNodes[j]->returnNumber());
                        int nodeNumCopy = int(joinedNodes[0]->returnNumber());
                        _bodies[int(joinedBodies[j]->returnNumber())]->_nodes[nodeNumLocal]->coordinates =(*body)->_nodes[nodeNumCopy]->coordinates ;
                        _bodies[int(joinedBodies[j]->returnNumber())]->_nodes[nodeNumLocal]->coordinated = true;

                    }
                }
            }
        }
        // save joints for later
        _joints.resize(jJoints.size());
        for (auto jObj=jJoints.begin();jObj!=jJoints.end();jObj++){
            int body_1 =int( (*jObj)->returnObject()["bodies"]->returnList()[0]->returnNumber());
            int body_2 =int( (*jObj)->returnObject()["bodies"]->returnList()[1]->returnNumber());
            int node_1 =int( (*jObj)->returnObject()["nodes"]->returnList()[0]->returnNumber());
            int node_2 =int( (*jObj)->returnObject()["nodes"]->returnList()[1]->returnNumber());
            int jointId =int( (*jObj)->returnObject()["id"]->returnNumber());
            std::pair<int,int> bodies(body_1,body_2);
            std::pair<int,int> nodes(node_1,node_2);
            std::pair<std::pair<int,int>,std::pair<int,int>> joint (bodies,nodes);
            _joints[jointId]=joint;
        }
        
        JSONList jVelocities =  jStructure["target_velocities"]->returnList();
        
        max_step=0;
        for (auto vel=jVelocities.begin();vel!=jVelocities.end();vel++){
            tVelocity *newVel=new tVelocity((*vel)->returnObject(),*this);
            _tVels.push_back(newVel);
            if (newVel->end>max_step){
                max_step=newVel->end;
            }
        }
    }

    Node * Structure::getNode(int i){
        return _nodes[i];
    }
    Body * Structure::getBody(int i){
        return _bodies[i];
    }
    std::pair<std::pair<int,int>,std::pair<int,int>> Structure::getJoint(int i){
        return _joints[i];
    }
    size_t Structure::getNNode(){
        return _nodes.size();
    }
    size_t Structure::getNBody(){
        return _bodies.size();
    }
    size_t Structure::getNCoords(){
        return coordinate;
    }
    vector<double> Structure::getNextTarVelocity(){
        vector<double> output(3,0);
        for (auto vel=_tVels.begin();vel!=_tVels.end();vel++){
            if ((*vel)->start<=currentStep &&(*vel)->end>currentStep){
                vector<double> v=(*vel)->getVelocity();
                for (int i=0;i<3;i++){
                    output[i]+=v[i];
                }
            }
        }
        currentStep++;
        return output;
    }
    void Structure::eulerIntegrate(vector<double>& vel){
        for(auto node=_nodes.begin();node!=_nodes.end();node++){
            (*node)->_position[0]=(*node)->_position[0]+eulerStepSize*vel[(*node)->coordinates[0]];
            (*node)->_position[1]=(*node)->_position[1]+eulerStepSize*vel[(*node)->coordinates[1]];
            (*node)->_position[2]=(*node)->_position[2]+eulerStepSize*vel[(*node)->coordinates[2]];
            std::vector<double>newPosHis = (*node)->_position;
            (*node)->_positionHist.push_back(newPosHis);
        }
    }

    void Structure::simulate(std::string method){
        for(auto node=_nodes.begin();node!=_nodes.end();node++){
            (*node)->_positionHist.reserve(max_step);
        }
        while (currentStep<=max_step){
            genConstraints();
            linAlg linlib; 
            vector<double> targetVel = getNextTarVelocity();
            vector<double> vel;
            vector<double> projector;
            vector<double> pinv;
            projector.resize(coordinate*coordinate);
            pinv.resize(coordinate,n_const);
            vel.resize(coordinate);
            linlib.matPseudoInv(n_const,coordinate,cnst_mat,pinv);
            linlib.matMult(coordinate,coordinate,n_const,pinv,cnst_mat,projector);
            for (int i=0;i<coordinate;i++){
                for(int j=0;j<coordinate;j++){
                    int ident;
                    if (i==j){
                        ident=1;
                    }else{
                        ident=0;
                    }
                    vel[i]=(ident-projector[i*coordinate+j])*targetVel[j];
                }

            }
            if(method=="euler"){
                eulerIntegrate(vel);
            }
            for(auto edge=_edges.begin();edge!=_edges.end();edge++){
                (*edge)->updatePos();
            }
            
            
        }
    }

    vector<double> Structure::getSimStep(int i){
        vector<double> output;
        output.resize(coordinate);
        for(auto node=_nodes.begin();node!=_nodes.end();node++){
            output[(*node)->coordinates[0]]=(*node)->_positionHist[i][0];
            output[(*node)->coordinates[1]]=(*node)->_positionHist[i][1];
            output[(*node)->coordinates[2]]=(*node)->_positionHist[i][2];
        }
        return output;
    }


}
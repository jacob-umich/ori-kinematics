#include "structure.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "linAlg.hpp"
#include "lapacke.h"
#include <regex>
#include <string>
#include <iostream>
#include <fstream>


namespace Okin {
    Structure::Structure(const std::string inputFile, double size): parser(inputFile), eulerStepSize(size){
        currentStep = 0;
        parser.parse();
        root = parser.root;
        jStructure = root->returnObjectPtr();
        JSONList jBodies =  (*jStructure)["bodies"]->returnList();
        JSONList* jBodiesPtr =  (*jStructure)["bodies"]->returnListPtr();
        size_t numBodies =jBodies.size();

        for (size_t i=0;i<numBodies;i++){
            JSONObject* jBodyPtr=(*jBodiesPtr)[i]->returnObjectPtr();
            Body *newBody = new Body(jBodyPtr);
            _bodies.push_back(newBody);
            for (auto node=newBody->_nodes.begin();node!=newBody->_nodes.end();node++){
                _nodes.push_back(*node);
            }
            for (auto edge=newBody->_edges.begin();edge!=newBody->_edges.end();edge++){
                _edges.push_back(*edge);
            }
        }
        coordinate = 0;
        int idg= 0;
        JSONList jJoints =  (*jStructure)["joints"]->returnList();
        for (auto body=_bodies.begin();body!=_bodies.end();body++){
            for (auto node=(*body)->_nodes.begin();node!=(*body)->_nodes.end();node++){
                if (!(*node)->coordinated){
                    (*node)->coordinates.resize(3);
                    (*node)->coordinates[0]=coordinate++;
                    (*node)->coordinates[1]=coordinate++;
                    (*node)->coordinates[2]=coordinate++;
                    (*node)->coordinated=true;
                    (*node)->idg=idg++;
                    (*node)->updateJSON();

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
                        _bodies[int(joinedBodies[j]->returnNumber())]->_nodes[nodeNumLocal]->idg =(*body)->_nodes[nodeNumCopy]->idg ;
                        _bodies[int(joinedBodies[j]->returnNumber())]->_nodes[nodeNumLocal]->coordinated = true;
                        _bodies[int(joinedBodies[j]->returnNumber())]->_nodes[nodeNumLocal]->updateJSON();

                    }
                }
            }
        }

        std::shared_ptr<JSONNode> ndof = std::make_shared<JSONNode>();
        ndof->setNumber(coordinate);
        (*jStructure).insert(std::pair<std::string,std::shared_ptr<JSONNode>>("n_dof",ndof));
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
        
        JSONList jVelocities =  (*jStructure)["target_velocities"]->returnList();
        
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
        vector<double> output(coordinate,0);
        for (auto vel=_tVels.begin();vel!=_tVels.end();vel++){
            if ((*vel)->start<=currentStep &&(*vel)->end>currentStep){
                vector<double> v=(*vel)->getVelocity();
                for (int i=0;i<3;i++){
                    output[(*vel)->coords[i]]+=v[i];
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
            vector<double> vel(coordinate,0.0);
            vector<double> projector(coordinate*coordinate,0.0);
            vector<double> pinv(coordinate*n_const,0.0);
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
                    vel[i]=vel[i]+(ident-projector[j*coordinate+i])*targetVel[j];
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

    void Structure::save(std::string filename){
        JSONList *jhistory=new JSONList();
        for(int i=0;i<max_step;i++){
            vector<double> output = getSimStep(i);
            JSONList *jOutput=new JSONList();
            for(auto pos=output.begin();pos!=output.end();pos++){
                std::shared_ptr<JSONNode> posI = std::make_shared<JSONNode>();
                posI->setNumber(*pos);
                (*jOutput).push_back(posI);
            }
            std::shared_ptr<JSONNode> listNode = std::make_shared<JSONNode>();
            listNode->setList(jOutput);
            jhistory->push_back(listNode);
        }
        std::shared_ptr<JSONNode> histNode = std::make_shared<JSONNode>();
        histNode->setList(jhistory);
        (*root->returnObjectPtr()).insert(std::pair<std::string,std::shared_ptr<JSONNode>>("history",histNode));
        std::string output = root->toString(4,false,1);
        std::ofstream outFile;
        outFile.open(filename,std::ios::out | std::ios::binary);
        outFile<<output;
        outFile.close();

    }


}
#include "body.hpp"
#include <iostream>
#include <algorithm>

bool compareNodePnt(Okin::Node*i,Okin::Node*j){
    return (*i)<(*j);
}

double myDet(std::vector<std::vector<double>>& input){
    double ret = input[0][0]*(input[1][1]*input[2][2]-input[1][2]*input[2][1])+input[0][1]*(input[1][2]*input[2][0]-input[1][0]*input[2][2])+input[0][2]*(input[1][0]*input[2][1]-input[1][1]*input[2][0]);
    return ret;
}
double myDot(std::vector<double>& a, std::vector<double>& b){
    
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

double toSize_t(std::shared_ptr<JSONNode> posNode){
    return size_t(posNode->returnNumber());
}
namespace Okin{
    Body::Body(JSONObject* jBodyPtr){
        JSONObject jBody= *jBodyPtr;
        _id=int(jBody["id"]->returnNumber());
        JSONList jNodes =  jBody["nodes"]->returnList();
        JSONList *jNodesPtr =  (*jBodyPtr)["nodes"]->returnListPtr();
        size_t numNodes =jNodes.size();

        for (size_t i=0;i<numNodes;i++){
            JSONObject *jNodePtr=(*jNodesPtr)[i]->returnObjectPtr();
            Node *newNode=new Node(jNodePtr);
            _nodes.push_back(newNode);
        }
        size_t numMeshedNodes = 0;
        std::vector<Node*> nodeCopy=_nodes;
        std::vector<Node*> meshedNodes;
        meshedNodes.reserve(numNodes);
        while (numMeshedNodes<numNodes){
            size_t numUnmeshedNodes = nodeCopy.size();
            for (size_t i=0;i<numUnmeshedNodes;i++){
                Node *currentNode=nodeCopy[i];
                if(currentNode->_id==6){
                }
                if (numMeshedNodes>2){
                    bool reset_i=false;
                    for (size_t j=numMeshedNodes-1;j>1;j--){
                        std::vector<double> a = (*meshedNodes[j])-(*currentNode);
                        std::vector<double> b = (*meshedNodes[j-1])-(*currentNode);
                        std::vector<double> c = (*meshedNodes[j-2])-(*currentNode);
                        std::vector<std::vector<double>>*matrix = new std::vector<std::vector<double>>({a,b,c});
                        double det = myDet(*matrix);
                        if (det>0.001||det<-0.001){
                            for (size_t k=j-2; k<j+1;k++){
                                (*meshedNodes[k]).neighbors.push_back(currentNode);
                                currentNode->neighbors.push_back(meshedNodes[k]);
                            }
                            meshedNodes.push_back(currentNode);
                            nodeCopy.erase(nodeCopy.begin()+i);
                            numMeshedNodes++;
                            reset_i=true;
                            break;
                        }
                        delete(matrix);
                    }
                    if(reset_i){
                        break;
                    }
                }
                if (numMeshedNodes==2){
                    std::vector<double> a = (*meshedNodes[0])-(*currentNode);
                    std::vector<double> b = (*meshedNodes[1])-(*currentNode);
                    double dot = myDot(a,b);
                    if (dot>1e-8||dot<-1e-8){
                        numMeshedNodes++;
                        currentNode->neighbors.push_back(meshedNodes[0]);
                        currentNode->neighbors.push_back(meshedNodes[1]);
                        meshedNodes[0]->neighbors.push_back(currentNode);
                        meshedNodes[1]->neighbors.push_back(currentNode);
                        meshedNodes.push_back(currentNode);
                        nodeCopy.erase(nodeCopy.begin()+i);
                        break;
                    }

                }
                if (numMeshedNodes<2){
                    meshedNodes.push_back(currentNode);
                    numMeshedNodes++;
                    if (numMeshedNodes==2){
                        currentNode->neighbors.push_back(meshedNodes[0]);
                        meshedNodes[0]->neighbors.push_back(currentNode);
                    }
                    nodeCopy.erase(nodeCopy.begin()+i);
                    break;

                }
            }
        }
        connectivityLocal.resize(numNodes);
        for(size_t i=0;i<numNodes;i++){
            Node * currentNode=_nodes[i];
            connectivityLocal[currentNode->_id].resize(numNodes);
            connectivityLocal[currentNode->_id]=std::vector<int>(numNodes,0);
            for (auto j=currentNode->neighbors.begin(); j!=currentNode->neighbors.end();j++){
                connectivityLocal[currentNode->_id][(*j)->_id]=1;
            }
        }

        std::sort(_nodes.begin(),_nodes.end(),compareNodePnt);

        for (size_t i=0; i<numNodes;i++){
            for (size_t j=i+1; j<numNodes;j++){
                if (connectivityLocal[i][j]==1){
                    Edge *newEdge=new Edge(_nodes[i],_nodes[j]);
                    _edges.push_back(newEdge);
                    nEdges++;
                }
            }
        }
        JSONList jFaces =  jBody["faces"]->returnList();
        for (auto face=jFaces.begin();face!=jFaces.end();face++){
            JSONList tempList = (*face)->returnList();
            vector<size_t> newFace;
            newFace.resize(tempList.size());
            std::transform(tempList.begin(),tempList.end(),newFace.begin(),toSize_t);
            faces.push_back(newFace);
            
        }

    }

    void Body::printConnectivity(){
        std::cout<<"[";
        size_t numNodes = _nodes.size();
        for (size_t i=0;i<numNodes;i++){
            if (i!=0){
                std::cout<<" ";

            }
            for (size_t j=0;j<numNodes;j++){
                std::cout<< connectivityLocal[i][j];
                if (!(j==(numNodes-1) && i==(numNodes-1))){
                    std::cout<<", ";
                }else{
                    std::cout<<" ]";
                }
                if(j==(numNodes-1)){
                    std::cout<<"\n";
                }
            }
        }
    }
    void Body::printFace(vector<size_t>& face){
        std::cout<<"[";
        size_t numNodes = face.size();
        for (size_t i=0;i<numNodes;i++){
            std::cout<< face[i];
            if (!(i==(numNodes-1))){
                std::cout<<", ";
            }else{
                std::cout<<" ]"<<std::endl;
            }
        }
    }
}
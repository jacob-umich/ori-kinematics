#include "body.hpp"

double myDet(std::vector<std::vector<double>> input){
    double ret = input[0][0]*(input[1][1]*input[2][2]-input[1][2]*input[2][1]);
    return ret;
}
double myDot(std::vector<double>a, std::vector<double> b){
    
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}
namespace Okin{
    Body::Body(JSONObject jBody){
        _id=int(jBody["id"]->returnNumber());
        JSONList jNodes =  jBody["nodes"]->returnList();
        size_t numNodes =jNodes.size();

        for (size_t i=0;i<numNodes;i++){
            JSONObject jNode=jNodes[i]->returnObject();
            Node *newNode=new Node(jNode);
            _nodes.push_back(newNode);
        }
        size_t numMeshedNodes = 0;
        std::vector<Node*> nodeCopy=_nodes;
        std::vector<Node*> meshedNodes;
        while (numMeshedNodes<numNodes){
            size_t numUnmeshedNodes = nodeCopy.size();
            for (size_t i=0;i<numUnmeshedNodes;i++){
                Node *currentNode=nodeCopy[i];
                if (numMeshedNodes>2){
                    for (size_t j=numMeshedNodes-1;j>1;j--){
                        std::vector<double> a = (*meshedNodes[j])-(*currentNode);
                        std::vector<double> b = (*meshedNodes[j-1])-(*currentNode);
                        std::vector<double> c = (*meshedNodes[j-2])-(*currentNode);
                        std::vector<std::vector<double>>matrix({a,b,c});
                        double det = myDet(matrix);
                        if (det>0.001||det<-0.001){
                            for (size_t k=j-2; k<j+1;k++){
                                (*meshedNodes[k]).neighbors.push_back(currentNode);
                                currentNode->neighbors.push_back(meshedNodes[k]);
                            }
                            meshedNodes.push_back(currentNode);
                            nodeCopy.erase(nodeCopy.begin()+i);
                            numMeshedNodes++;
                            break;
                        }
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

                }
                break;
            }
        }
    }
}
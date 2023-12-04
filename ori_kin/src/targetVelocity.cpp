#include "targetVelocity.hpp"
void normalize(std::vector<double>& a){
    double dot=0;
    for (int i =0;i<3;i++){
        dot+=a[i]*a[i];
    }
    dot = sqrt(dot);
    for (int i =0;i<3;i++){
        a[i]=a[i]/dot;
    }
}

std::vector<double> project(std::vector<double>a,std::vector<double>j){
    std::vector<double> ret;
<<<<<<< HEAD
    double dot=0;
=======
    double dot;
    double dotSq;
>>>>>>> ec7d5c56b883d08f43657f9618da5c5fdb0f6ee5
    ret.resize(3);
    normalize(j);
    normalize(a);
    for (int i =0;i<3;i++){
        dot+=a[i]*j[i];
    }

    for (int i =0;i<3;i++){
        ret[i]=a[i]-dot*j[i];
    }
    return ret;
}
namespace Okin{

    tVelocity::tVelocity(JSONObject jVel, Structure& parentIn):
        start(jVel["start_step"]->returnInt()), 
        end(jVel["end_step"]->returnInt()),
        stepSize(jVel["step_size"]->returnNumber()),
        parent(parentIn)
        
    {
        type = jVel["type"]->returnString();
        targetVelocity.resize(3);
        if (type=="angular"){
            refNode = parent.getBody(jVel["ref_body"]->returnInt())->_nodes[jVel["ref_node"]->returnInt()];
            targetNode = parent.getBody(jVel["target_body"]->returnInt())->_nodes[jVel["target_node"]->returnInt()];
            joint1 = parent.getBody(parent.getJoint(jVel["joint_1"]->returnInt()).first.first)->_nodes[parent.getJoint(jVel["joint_1"]->returnInt()).second.first];
            joint2 = parent.getBody(parent.getJoint(jVel["joint_2"]->returnInt()).first.first)->_nodes[parent.getJoint(jVel["joint_2"]->returnInt()).second.first];
            targetRatio= jVel["ratio"]->returnNumber();
        } else if (type=="vector"){
            for(int i=0;i<3;i++){
                targetVelocity[i] = jVel["velocity"]->returnList()[i]->returnNumber();
            }
        } else{
            throw std::logic_error("Improper velocity");
        }

    }
    void tVelocity::updateFromAngle(){
        std::vector<double> a = project(*targetNode-*joint1,*joint2-*joint1);
        std::vector<double> b = project(*refNode-*joint1,*joint2-*joint1);
        std::vector<double> c = *joint2-*joint1;
        normalize(c);
        targetVelocity = {a[1]*c[2]-a[2]*c[1],a[2]*c[0]-a[0]*c[2],a[0]*c[1]-a[1]*c[0]};
        double dotOld=0;
        double dotNew=0;
        normalize(a);
        normalize(b);
        for (int i =0;i<3;i++){
            dotOld+=a[i]*b[i];
            dotNew+=(targetVelocity[i]/100+a[i])*b[i];
        }

        if(abs(targetRatio-dotNew)>abs(targetRatio-dotOld)){
            if((targetRatio-dotNew)*(targetRatio-dotOld)>0){
                for (int i =0;i<3;i++){
                    targetVelocity[i]=-targetVelocity[i];
                }
            }
        }

        if(abs(targetRatio-dotOld)<1e-3){
            targetVelocity={0,0,0};
        }
    }
    vector<double> tVelocity::getVelocity(){
        if (type=="angular"){
            updateFromAngle();
        }
        return targetVelocity;
    };
}
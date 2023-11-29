#include "ori_ken.hpp"
#include <vector>
using std::vector;

namespace Okin{

    void Structure::generateConstraints(){
	    // rigid-body origami-kinematics assumes all edges are rigid
        int nodeIdx[2];
        size_t nedges = _edges.size();
	    // resize constraint matrix (C) to required size
	    C_vals.resize(nedges*6);
        colIdx.resize(nedges*6);
        rowIdx.resize(nedges*6);

        // C matrix is nedgesx3nnodes
        // x vector is 3nnodesX1
        // b vector is nedgesx1
        // note C matrix is sparse, stored in COO format
        // C_vals is nedgesX3nnodesX2 long
        // loop through all edges
	    for (size_t iedge=0; iedge<_edges.size(); iedge++){
            nodeIdx[0] = _edges[iedge]->_nodes[0]->coordinates[0];
			nodeIdx[1] = _edges[iedge]->_nodes[1]->coordinates[0];
            // loop through cooridnates
		    for(size_t icoord=0; icoord<3; icoord++){ 
                //for first node on edge
			    C_vals[iedge*6+icoord*2] = _edges[iedge]->start_pos[icoord]-_edges[iedge]->end_pos[icoord];
				rowIdx[iedge*6+icoord*2] = iedge;
				colIdx[iedge*6+icoord*2] = nodeIdx[0]+icoord;
                //for second node on edge
			    C_vals[iedge*6+icoord*2+1]=_edges[iedge]->end_pos[icoord]-_edges[iedge]->start_pos[icoord];;
				rowIdx[iedge*6+icoord*2+1] = iedge;
				colIdx[iedge*6+icoord*2+1] = nodeIdx[1]+icoord;
			}
        }
        size_t nnz=_edges.size()*6;
        size_t nConstriants=_edges.size();
        for (auto node=_nodes.begin();node!=_nodes.end();node++){
            for(size_t i=0; i<3; i++){ 
                if ((*node)->_fixities[i]==1) {
                    C_vals.push_back(1);
                    colIdx.push_back((*node)->coordinates[i]);
                    rowIdx.push_back(nConstriants++);
                    nnz++;
                }
            }
	    }
    }
} // namespace Okin
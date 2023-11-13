#include "ori_ken.hpp"

namespace std
{
    void structure::generateConstraints(vector<vector<double>> &nodes,
     vector<vector<int>> &edges, size_t nnodes,size_t nedges,
     vector<vector<int>> &const_nodes,vector<double> &C_vals, vector<int> &colIdx, vector<int> &rowIdx){
	    // rigid-body origami-kinematics assumes all edges are rigid
        double nodeIdx[2];
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
	    for (int iedge=0; iedge<nedges; iedge++){
            nodeIdx[0] = edges[iedge][0];
			nodeIdx[1] = edges[iedge][1];
            // loop through cooridnates
		    for(int icoord=0; icoord<3; icoord++){ 
                //for first node on edge
                // check if node is constrained, otherwise assigned value
                if (const_nodes[nodeIdx[0]][icoord]==1) C_vals[iedge*6+icoord*2] = 1;
			    else C_vals[iedge*6+icoord*2]=nodes[nodeIdx[0]][icoord]-nodes[nodeIdx[1]][icoord];
				colIdx[iedge*6+icoord*2] = iedge;
				rowIdx[iedge*6+icoord*2] = nodeIdx[0]*3+icoord;
                //for second node on edge
                if (const_nodes[nodeIdx[1]][icoord]==1) C_vals[iedge*6+icoord*2+1] = 1;
			    else C_vals[iedge*6+icoord*2+1]=nodes[nodeIdx[1]][icoord]-nodes[nodeIdx[0]][icoord];
				colIdx[iedge*6+icoord*2+1] = iedge;
				rowIdx[iedge*6+icoord*2+1] = nodeIdx[1]*3+icoord;
			}
	    }

    }
} // namespace Okin
#include "structure.hpp"
namespace Okin
{
    void Structure::genConstraints(vector<vector<double>> &nodes,
    vector<vector<int>> &edges,vector<int> &glob_cnst, vector<double> cnst_mat){
        int nedges = edges.size();
        int nnodes = nodes.size();
        cnst_mat.resize(nedges*3*nnodes);
        glob_cnst.resize(nnodes);
        vector<vector<double>> full_cnst_mat;
        full_cnst_mat.resize(nedges,vector<double>(3*nnodes,0.0));//initialize all to zero
        int nodeIdxL,nodeIdxR,xIdxL,yIdxL,zIdxL,xIdxR,yIdxR,zIdxR;
        // C matrix is nedgesx3nnodes
        // x vector is 3nnodesX1
        // b vector is nedgesx1
        for (int i=0; i<nedges; i++){
                nodeIdxL = edges[i][0];
                nodeIdxR = edges[i][1];
                xIdxL = nodeIdxL*3;
                yIdxL = nodeIdxL*3+1;
                zIdxL = nodeIdxL*3+2;
                xIdxR = nodeIdxL*3;
                yIdxR = nodeIdxL*3+1;
                zIdxR = nodeIdxL*3+2;
                full_cnst_mat[i][xIdxL] = nodes[nodeIdxL][0]-nodes[nodeIdxL][0];
                full_cnst_mat[i][xIdxR] = -(nodes[nodeIdxR][0]-nodes[nodeIdxR][0]);
                full_cnst_mat[i][yIdxL] = nodes[nodeIdxL][1]-nodes[nodeIdxL][1];
                full_cnst_mat[i][yIdxR] = -(nodes[nodeIdxR][1]-nodes[nodeIdxR][1]);
                full_cnst_mat[i][zIdxL] = nodes[nodeIdxL][2]-nodes[nodeIdxL][2];
                full_cnst_mat[i][zIdxR] = -(nodes[nodeIdxR][2]-nodes[nodeIdxR][2]);
        }
        // convert to column-major storage
        for (int i=0; i<3*nnodes; i++){
            for (int j=0; j<3*nedges; j++){
                cnst_mat[i*nedges+j] = full_cnst_mat[j][i];
            }
        }
    }
    // void Structure::generateConstraints(vector<vector<double>> &nodes,
    //  vector<vector<int>> &edges, size_t nnodes,size_t nedges,
    //  vector<vector<int>> &const_nodes,vector<double> &C_vals, vector<int> &colIdx, vector<int> &rowIdx){
	//     // rigid-body origami-kinematics assumes all edges are rigid
    //     double nodeIdx[2];
	//     // resize constraint matrix (C) to required size
	//     C_vals.resize(nedges*6);
    //     colIdx.resize(nedges*6);
    //     rowIdx.resize(nedges*6);

    //     // C matrix is nedgesx3nnodes
    //     // x vector is 3nnodesX1
    //     // b vector is nedgesx1
    //     // note C matrix is sparse, stored in COO format
    //     // C_vals is nedgesX3nnodesX2 long
    //     // loop through all edges
	//     for (int iedge=0; iedge<nedges; iedge++){
    //         nodeIdx[0] = edges[iedge][0];
	// 		nodeIdx[1] = edges[iedge][1];
    //         // loop through cooridnates
	// 	    for(int icoord=0; icoord<3; icoord++){ 
    //             //for first node on edge
    //             // check if node is constrained, otherwise assigned value
    //             if (const_nodes[nodeIdx[0]][icoord]==1) C_vals[iedge*6+icoord*2] = 1;
	// 		    else C_vals[iedge*6+icoord*2]=nodes[nodeIdx[0]][icoord]-nodes[nodeIdx[1]][icoord];
	// 			colIdx[iedge*6+icoord*2] = iedge;
	// 			rowIdx[iedge*6+icoord*2] = nodeIdx[0]*3+icoord;
    //             //for second node on edge
    //             if (const_nodes[nodeIdx[1]][icoord]==1) C_vals[iedge*6+icoord*2+1] = 1;
	// 		    else C_vals[iedge*6+icoord*2+1]=nodes[nodeIdx[1]][icoord]-nodes[nodeIdx[0]][icoord];
	// 			colIdx[iedge*6+icoord*2+1] = iedge;
	// 			rowIdx[iedge*6+icoord*2+1] = nodeIdx[1]*3+icoord;
    //             // Dense format
    //             C[iedge*6+icoord*2]=nodes[nodeIdx[0]][icoord]-nodes[nodeIdx[1]][icoord];
	// 		}
	//     }

    // }
} // namespace Okin
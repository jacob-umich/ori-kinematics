#include "ori_kin.hpp"
#include <vector>
using std::vector;

namespace Okin
{
    void Structure::genConstraints(){
        size_t nedges = _edges.size();
        size_t nDOF = size_t(coordinate);

        cnst_mat.resize(nedges*nDOF);
        vector<vector<double>> full_cnst_mat;
        full_cnst_mat.resize(nedges,vector<double>(nDOF,0.0));//initialize all to zero
        // C matrix is nedgesx3nnodes
        // x vector is 3nnodesX1
        // b vector is nedgesx1
        int xIdxL,yIdxL,zIdxL,xIdxR,yIdxR,zIdxR;
        for (size_t i=0; i<nedges; i++){
            xIdxL = _edges[i]->_nodes[0]->coordinates[0];
            yIdxL = _edges[i]->_nodes[0]->coordinates[1];
            zIdxL = _edges[i]->_nodes[0]->coordinates[2];
            xIdxR = _edges[i]->_nodes[1]->coordinates[0];
            yIdxR = _edges[i]->_nodes[1]->coordinates[1];
            zIdxR = _edges[i]->_nodes[1]->coordinates[2];
            full_cnst_mat[i][xIdxL] = _edges[i]->start_pos[0]-_edges[i]->end_pos[0];
            full_cnst_mat[i][xIdxR] = -(_edges[i]->start_pos[0]-_edges[i]->end_pos[0]);
            full_cnst_mat[i][yIdxL] = _edges[i]->start_pos[1]-_edges[i]->end_pos[1];
            full_cnst_mat[i][yIdxR] = -(_edges[i]->start_pos[1]-_edges[i]->end_pos[1]);
            full_cnst_mat[i][zIdxL] = _edges[i]->start_pos[2]-_edges[i]->end_pos[2];
            full_cnst_mat[i][zIdxR] = -(_edges[i]->start_pos[2]-_edges[i]->end_pos[2]);
        }

        // add boundary conditions
        for (auto node=_nodes.begin();node!=_nodes.end();node++){
            for(size_t i=0; i<3; i++){ 
                if ((*node)->_fixities[i]==1) {
                    vector<double>newRow(nDOF,0.0);
                    newRow[(*node)->coordinates[i]]=1;
                    full_cnst_mat.push_back(newRow);
                }
            }
	    }

        // convert to column-major storage
        for (size_t i=0; i<nDOF; i++){
            for (size_t j=0; j<nedges; j++){
                cnst_mat[i*nedges+j] = full_cnst_mat[j][i];
            }
        }
    }

} // namespace Okin
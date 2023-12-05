// Testing library required for testing (Always include this last!)
#include "ori_kin.hpp"
#include "unit_test_framework.hpp"
// Test if number of rows is equal to fixities+number of edges
TEST_CASE(checkCols) {
  Okin::Structure tester("testMesh.json");
  tester.genConstraints();
  size_t nBodies = tester.getNBody();
  size_t nedges=0;
  for (size_t ibod=0; ibod<nBodies; ibod++) {
    Okin::Body *test = tester.getBody(ibod);
    nedges += test->_edges.size();}
  size_t nDOF = tester.getNCoords();
  size_t matSize =tester.cnst_mat.size();
  size_t nFixities=6;
  assert(matSize/nDOF==nedges+nFixities);
}

// Test to see if first edge is correctly calculated
TEST_CASE(checkRow) {
  Okin::Structure tester("testMesh.json");
  tester.genConstraints();
  size_t nBodies = tester.getNBody();
  size_t nedges=0;
  for (size_t ibod=0; ibod<nBodies; ibod++) {
    Okin::Body *test = tester.getBody(ibod);
    nedges += test->_edges.size();}
  size_t nDOF = tester.getNCoords();
  size_t nFixities=6;
  vector<double> test_row;
  test_row.resize(nDOF,0.0);
  test_row[0] = -1;test_row[3] = 1;
  for (size_t i=0; i<nDOF; i++){
    ASSERT_NEAR(tester.cnst_mat[i*(nedges+nFixities)],test_row[i],1e-5);
    //std::cout<<tester.cnst_mat[i*(nedges+nFixities)]<<std::endl;
  }

}

int main()
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  TEST(checkCols);
  TEST(checkRow);
  return 0; 
}
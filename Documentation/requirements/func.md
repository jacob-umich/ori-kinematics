# Problem Definition
Develop a library to simulate the motion of constrained rigid bodies based on a given input motion.

## High Level Requirements
- The library shall be able to simulate the kinematics any convex rigid body that can be sufficiently described by its vertices.
- The library shall make use of performant linear algebra libraries

- The library shall make use of Object-Oriented Design

- The library shall make use of design patterns

- The library shall be extensible

- The library shall use a CMake based build system

- The library shall use C++ mainly and C if needed

## Detailed Functional Requirements
- The library shall generate a complete mesh of a system of rigid bodies given an input file of each rigid body's relevant vertices. 
- The library shall define a constraint matrix for a defined rigid body based only on the defined mesh and current position.
- The library shall compute a constrained set of velocities given a general velocity of interest and update the mesh positions based on a specified integration scheme.
- The library shall support calculations in double precicions arithmetic.
- The library shall provide a method for visualizing the constrained motion
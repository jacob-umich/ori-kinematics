#pragma once

namespace Okin{
    class Node{

        protected:
            char * name;
            double * _position;
            Node * neighbors;
            int * fixities;
        private:
            size_t n_bodies;
        public:
            Node(double * position);
            Node();
            void printPos();

    };
}
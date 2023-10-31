#pragma once

namespace Okin{
    class node{

        protected:
            char * name;
            double * position;
            node * neighbors;
            int * fixities;
        private:
            size_t n_bodies;

    };
}
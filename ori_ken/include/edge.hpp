namespace Okin{
    class Edge{

        protected:
            char * name;
            double * position;
            Edge * neighbors;
            int * fixities;
        private:
            size_t n_bodies;

    };
}
namespace Okin{
    class Body{

        protected:
            char * name;
            double * position;
            Body * neighbors;
            int * fixities;
        private:
            size_t n_bodies;

    };
}
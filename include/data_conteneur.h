#ifndef DATA_CONTENEUR_H
#define DATA_CONTENEUR_H
#include <vector>
#include "data_format.h"

using std::vector;

class data_conteneur
{
    public:
        data_conteneur();
        virtual ~data_conteneur();
        int Getiterateur() { return iterateur; }
        int Getiterateurmax() { return iterateur_max; }
        void Setiterateur(int val) { iterateur = val; }
        void create_new_data(int KP,unsigned char KD,unsigned char mem_a,unsigned char mem_b);
        data_format get_current_data();
        void set_current(int i){iterateur=i;}
        void next(){if(iterateur<iterateur_max)iterateur++;}
        void last(){if(iterateur>0)iterateur--;}
        void diff_toggle();
    protected:
    private:
        int iterateur_max;
        int iterateur;
        vector<data_format> conteneur;
        bool real_time;


};

#endif // DATA_CONTENEUR_H

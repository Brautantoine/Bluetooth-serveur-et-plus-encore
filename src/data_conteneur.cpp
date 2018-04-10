#include "data_conteneur.h"

data_conteneur::data_conteneur()
{
    //ctor
    iterateur=0;
    iterateur_max=0;
    real_time=true;
}

data_conteneur::~data_conteneur()
{
    //dtor
}

void data_conteneur::create_new_data(int KP,unsigned char KD,unsigned char mem_a,unsigned char mem_b)
{
    conteneur.emplace_back(KP,KD,mem_a,mem_b);
    iterateur_max++;
    if(real_time)iterateur++;
}

data_format data_conteneur::get_current_data()
{
    return conteneur[iterateur];
}

void data_conteneur::diff_toggle()
{
    real_time=!real_time;
    iterateur=iterateur_max;
}

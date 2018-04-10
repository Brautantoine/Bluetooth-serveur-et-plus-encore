#include "data_format.h"

data_format::data_format(int ext_KP,unsigned char ext_KD,unsigned char mem_a,unsigned char mem_b)
{
    //ctor
//    tab[mem_a]=255;
//    tab[mem_b+64]=255;
    tab=std::vector<unsigned char>(sizeof(unsigned char)*127,0);
    tab.at(mem_a)=255;
    tab.at(mem_b)=255;
    pic_a=mem_a;
    pic_b=mem_b;
    KD=ext_KD;
    KP=ext_KP;
}

data_format::~data_format()
{
    //dtor
}

#include "remote_adress.h"
#include "iostream"
#include "fstream"
#include "cerrno"
#include "cstring"


remote_adress::remote_adress(int x,std::string y,std::string z): ordre(x),remote_adr(y),chain_info(z)
{
    fullstring = std::to_string(ordre)+ " " + remote_adr + chain_info;
    flag_use=false;
}

remote_adress::~remote_adress()
{
    //dtor
}



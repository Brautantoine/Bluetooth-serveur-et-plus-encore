#include "gestion_fichier.hpp"
#include "iostream"
#include "fstream"
#include "cerrno"
#include "cstring"
#include "../const_and_def.hpp"

void stdfile::void_log_file()
{
    std::ofstream flux(LOG_FILE,std::ios::trunc);
    flux.close();
}

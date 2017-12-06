#include "B2th.h"
#include "iostream"
#include "fstream"
#include "cerrno"
#include "cstring"
#include "ctime"
#include "Erreur.hpp"
#include "imgui.h"


using std::string;



B2th::B2th() //constructeur par defualt
{
    //struct sockaddr_rc addr = { 0 };
    B2th::flag_Connect=0;
    current_addr=0;
}

B2th::~B2th()
{
    //dtor
}


/**    Fonctions            **/
/**    Connecte le socket   **/
/**                         **/
int B2th::connection()
{
    while(!(*(rAddr[current_addr].Getflag_use())))
    {
        ftrig_adress();
    }


    // allocate a socket
    std::cout << "Allocate socket :";//printf("Allocate socket :");//
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    //printf(" Ready\n");//std:cout << " Ready" << std::endl;

    // set the connection parameters (who to connect to)
    //printf("Set connection parameters :");//std::cout << "Set connection parameters :";
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(rAddr[current_addr].Getremote_adr(), &addr.rc_bdaddr );  //str2ba( dest, &addr.rc_bdaddr );
    //printf(" Ready\n");//std::cout << " Ready";



    // connect to server
    //printf("Connect to device ...");//std::cout << "Connect to device ...";
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    if( status == 0 )
    {
        std::cout << " Connection OK" << std::endl;
        flag_Connect=true;
    }

    if( status < 0 ) {perror("uh oh");flag_Connect=-1;}

    //puting socket in non-blocking mode
    sock_flags = fcntl(s,F_GETFL,0);
    fcntl(s,F_SETFL, sock_flags | O_NONBLOCK);

    data_in.clear();
    stdfile::void_log_file();

    return 0;
}

bool B2th::get_connect_status()
{
    return flag_Connect;
}

void B2th::send_to_remote()
{
    if(rtx!="/EOT"&&rtx!="/RCV")
        {
            status = write(s, rtx.c_str(), rtx.length());
            if(status<0) perror("Echec :");
        }
        else if(rtx=="/EOT")
        {
            status = write(s, "EOT", rtx.length());
            if(status<0) perror("Echec :");
        }
}

void B2th::send_to_remote(char msg)
{
    if(rtx!="/EOT"&&rtx!="/RCV")
        {
            status = write(s, &msg, 1);
            if(status<0) perror("Echec :");
        }
        else if(rtx=="/EOT")
        {
            status = write(s, "EOT", rtx.length());
            if(status<0) perror("Echec :");
        }
}

void B2th::set_rtx(char* data)
{
    rtx=data;
}

std::string B2th::recv_from_remote()
{
    if(flag_Connect)
    {


            if(stat_rec=read(s,&buff_random,99)>0)
            {
                buff_random[stat_rec]='\0';
                data_in.append(buff_random);
                std::ofstream flux(LOG_FILE,std::ios::app);
                if(!flux.fail())
                {
                    flux << buff_random;// << std::endl;
                    flux.close();
                }

            }
            //else data_in="N/A\0";

            return data_in;
    }
    else return "déconnecter";
}

std::string B2th::recv_from_remote(int lenght) // OOD
{

            if(stat_rec=read(s,&buff_random,lenght)>0)
            {
                buff_random[stat_rec]='\0';
                data_in.append(buff_random);
                /*std::ofstream flux("config/log",std::ios::app);
                if(!flux.fail())
                {
                    flux << data_in << std::endl;
                    flux.close();
                }*/

            }
            //else data_in="N/A\0";

            return data_in;
}

void B2th::clear_data_in()
{
    data_in.clear();
}

void B2th::close_connection()
{
    if (flag_Connect)
    {
    close(s);
    flag_Connect=false;
    }
}

const char* B2th::get_dest()
{
    if (*(rAddr[current_addr].Getflag_use()))
    return rAddr[current_addr].Getremote_adr();
    else return "Selectionnez une adresse";
}



void B2th::create_default_txt()
{
    std::ofstream flux(ADRESS_FILE,std::ios::trunc);
    if(!flux.fail())
        {
            flux << 1 << " 00:06:66:6E:00:C6" << " péripheriques bluetooth par defaut \"iut\"" << std::endl
                 << 2 << " 00:06:66:7D:5C:AC" << " péripheriques bluetooth par defaut \"Mr.Verbe\"";
            flux.close();
        }
    else std::cerr << "erreur lors de la création du fichier par défaut :" << std::strerror(errno);
}

int B2th::load_from_txt()
{
    float load_start_at(ImGui::GetTime());
    int tempreturn(0);
    int tempInt;
    std::string tempString1;
    std::string tempString2;
    std::ifstream flux(ADRESS_FILE,std::ios::in);
    if(!flux.fail())
    {
        bool term(flux.eof());
        while(!term)
        {
            flux >> tempInt >> tempString1;
            if(tempString1.size()!=17)throw Erreur(ImGui::GetTime(),MAC_INVALIDE,"Fichier invalide (Adresse  MAC erroné)\nCréation d'un nouveau fichier d'adresse par défaut");
            getline(flux,tempString2);
            rAddr.emplace_back(tempInt,tempString1.data(),tempString2.data());
            term=flux.eof();
        }
        flux.close();
    }
    else if(errno==ENOENT)throw Erreur(ImGui::GetTime(),FICHIER_ABSENT,"Fichier introuvable\nCréation d'un nouveau fichier d'adresse par défaut");
    return tempreturn;
}

void B2th::trig_adress(int pos)
{
    if(*(rAddr[pos].Getflag_use()))
    {
        for (int i=0;i<rAddr.size();i++)
        {
            if(i!=pos)
            {
                rAddr[i].Setflag_use(false);
            }
        }
        current_addr=pos;
    }
}

void B2th::ftrig_adress()
{
    rAddr[current_addr].Setflag_use(true);
    if(*(rAddr[current_addr].Getflag_use()))
    {
        for (int i=0;i<rAddr.size();i++)
        {
            if(i!=current_addr)
            {
                rAddr[i].Setflag_use(false);
            }
        }
    }
}

void B2th::create_new_addr(std::string adresse,std::string detail)
{
    std::ofstream flux(ADRESS_FILE,std::ios::app);
    if(!flux.fail())
        {
            flux << std::endl << rAddr.size()+1 << " " << adresse << " " << detail ;
            flux.close();
        }
    else std::cerr << "erreur lors de l'ajout de l'adresse :" << std::strerror(errno);

    rAddr.emplace_back(rAddr.size()+1,adresse," "+detail);
}

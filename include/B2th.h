#ifndef B2TH_H
#define B2TH_H

#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <vector>
#include <queue>
#include <string>
#include <imgui.h>
#include "remote_adress.h"
#include "gestion_fichier.hpp"
#include "../const_and_def.hpp"
#include "thread"
#include "mutex"
#include "robot_data.h"
#include "data_conteneur.h"


#define ATTENTE_START 0
#define ATTENTE_STOP 1
#define VALIDATION 2

class B2th
{
    public:

        ///Constructeur
        //B2th(char* adrr);  //DEPRECATED
        B2th();

        virtual ~B2th();

        ///Get
        bool get_connect_status();                                                                 // Renvoie la valeur actuelle du flag de connection
        bool* get_current_flag_control(int current){return rAddr[current].Getflag_use();}          // Renvoie la valeur du flag d'activation de l'adresse actuellement manipulé
        const char* get_dest();                                                                    // Renvoie l'adresse actuellement actif
        int get_adr_number(){return rAddr.size();}                                                 // Renvoie le nombre total d'adresse charge
        int get_current_addr_pos(){return current_addr;}                                           // Renvoie le rang de l'adresse actuellement active
        std::string get_current_addr_detail(int current){return rAddr[current].get_fullstring();}  // Renvoie le chaine complete de definition de l'adresse (rang + adresse Mac + detail)
        std::string get_data_raw(){return data_in;}
        std::string get_buff_rec(){return buff_rec;}
     //   std::queue <char> get_data_log(){return log;}
        ImGuiTextBuffer get_log(){return log;}

        ///Manipulation Bluetooth
        int connection();                                                                          // Connection vers l'adresse active (adresse 0 si aucune adresse active) et passage du socket en non bloquant /!\ AUCUN RETURN
        void send_to_remote();                                                                     // Envoie des donnees contenue dans rtx vers le peripherique bluetooth
        void send_to_remote(char msg);
        void send_to_remote(std::queue <char> msg);                                                                   // Envoie des donnees contenue dans rtx vers le peripherique bluetooth
        void set_rtx(char* data);                                                                  // Modification des donnees de rtx
        void close_thread();
        void clear_data_in();                                                                      // Efface le buffer de reception
        void clear_buff_rec();
        void close_connection();                                                                   // Ferme la connection
       // std::string recv_from_remote();                                                     // Ecoute les donnees entrante et les places dans le buffer
       // std::vector<float> recv_from_remote(int lenght);                                           // Surcharge pour recevoir un nombre precis de donnees Out Of date
        void recv_from_remote_thread();
        void detection_trame();
        //void format_data_in();

        ///Gestion du repertoire
        void create_default_txt();                                                                 // Creer un fichier texte ../config/adresse contenant les valeurs
        int load_from_txt();                                                                       // Charge toutes les adresses presente dans ../config/adresse return 0 si reussi -1 si echec
        void create_new_addr(std::string adresse,std::string detail);                              // Enregistre une nouvelle adresse et l'ecris dans le fichier de sauvegarde

        ///Manipulation de l'adresse
        void trig_adress(int pos);                                                                 // Rend une adresse active
        void ftrig_adress();                                                                       // Force l'activation de l'adresse 0

        ///Fonctions statiques
        uint16_t fletcher16(const char* data,size_t len);
        uint16_t ip_like_checksum(const char *data,size_t len); //Tout pourri
        ///fonction de manipulation de données
        // std::string vf_2_string(std::vector<float> data);                                          //Renvoie l'équivalent Ascii d'un tableau de float
        // void close_thread();
        // std::queue<int> raw_2_fifo(unsigned char* data,int lenght);                                          //Remplie un vecteur de int avec les données en argument

///        void bind_robot(robot_data& new_rob);

    protected:
    private:

        char* dest;
        std::string rtx;
        std::string data_in;
        std::string buff_rec;
        //unsigned char buff_random[100];

        struct sockaddr_rc addr;

        int s, status,stat_rec, sock_flags;
        int current_addr;

        bool flag_Connect;
        bool soft_is_running;

        std::vector <remote_adress> rAddr;

        //std::queue <char> log;
        ImGuiTextBuffer log;
        int lines;

      //  robot_data& otc;

        std::thread reception;
        std::thread trame_detect;
        std::mutex trame_block;
      //  int trame_cpt;
        data_conteneur buffer_format;
        //std::string msg_rec;
};

#endif // B2TH_H

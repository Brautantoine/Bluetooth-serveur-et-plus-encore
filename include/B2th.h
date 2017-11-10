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
#include <string>
#include "remote_adress.h"
#include "gestion_fichier.hpp"
#include "../const_and_def.hpp"


class B2th
{
    public:

        //Constructeur
        //B2th(char* adrr);  //DEPRECATED
        B2th();

        virtual ~B2th();

        //Get
        bool get_connect_status();                                                                 // Renvoie la valeur actuelle du flag de connection
        bool* get_current_flag_control(int current){return rAddr[current].Getflag_use();}          // Renvoie la valeur du flag d'activation de l'adresse actuellement manipulé
        const char* get_dest();                                                                    // Renvoie l'adresse actuellement actif
        int get_adr_number(){return rAddr.size();}                                                 // Renvoie le nombre total d'adresse charge
        int get_current_addr_pos(){return current_addr;}                                           // Renvoie le rang de l'adresse actuellement active
        std::string get_current_addr_detail(int current){return rAddr[current].get_fullstring();}  // Renvoie le chaine complete de definition de l'adresse (rang + adresse Mac + detail)

        //Manipulation Bluetooth
        int connection();                                                                          // Connection vers l'adresse active (adresse 0 si aucune adresse active) et passage du socket en non bloquant /!\ AUCUN RETURN
        void send_to_remote();                                                                     // Envoie des donnees contenue dans rtx vers le peripherique bluetooth
        void set_rtx(char* data);                                                                  // Modification des donnees de rtx
        void clear_data_in();                                                                      // Efface le buffer de reception
        void close_connection();                                                                   // Ferme la connection
        std::string recv_from_remote();                                                            // Ecoute les donnees entrante et les places dans le buffer
        std::string recv_from_remote(int lenght);                                                  // Surcharge pour recevoir un nombre precis de donnees

        //Gestion du repertoire
        void create_default_txt();                                                                 // Creer un fichier texte ../config/adresse contenant les valeurs
        int load_from_txt();                                                                       // Charge toutes les adresses presente dans ../config/adresse return 0 si reussi -1 si echec
        void create_new_addr(std::string adresse,std::string detail);                              // Enregistre une nouvelle adresse et l'ecris dans le fichier de sauvegarde

        //Manipulation de l'adresse
        void trig_adress(int pos);                                                                 // Rend une adresse active
        void ftrig_adress();                                                                       // Force l'activation de l'adresse 0

    protected:
    private:

        struct sockaddr_rc addr;

        int s, status,stat_rec, sock_flags;
        int current_addr;

        char* dest;
        std::string rtx;
        std::string data_in;
        char buff_random[100];

        bool flag_Connect;

        std::vector <remote_adress> rAddr;
};

#endif // B2TH_H

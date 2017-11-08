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


class B2th
{
    public:
        B2th(char* adrr);
        B2th();
        virtual ~B2th();
        int connection();
        bool get_connect_status();
        void send_to_remote();
        void set_rtx(char* data);
        std::string recv_from_remote();
        std::string recv_from_remote(int lenght);
        void clear_data_in();
        void close_connection();
        const char* get_dest();
        void create_default_txt();
        void load_from_txt();
        int get_adr_number(){return rAddr.size();}
        std::string get_current_addr_detail(int current){return rAddr[current].get_fullstring();}
        bool* get_current_flag_control(int current){return rAddr[current].Getflag_use();}
        int get_current_addr_pos(){return current_addr;}
        void trig_adress(int pos);
        void ftrig_adress(); //Force une connection vers l'adresse 0
    protected:
    private:
        struct sockaddr_rc addr;
        int s, status,stat_rec, sock_flags;
        char* dest;
        std::string rtx;
        std::string data_in;
        char buff_random[100];
        bool flag_Connect;
        int current_addr;

        std::vector <remote_adress> rAddr;
};

#endif // B2TH_H

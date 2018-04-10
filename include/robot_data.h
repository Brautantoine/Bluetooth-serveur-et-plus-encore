#ifndef ROBOT_DATA_H
#define ROBOT_DATA_H

#include <string>
#include "queue"
#include "B2th.h"

using std::string;
using std::queue;

class robot_data
{
    public:
        robot_data();
        //robot_data(B2th& remote);
        virtual ~robot_data();

        //GET
        bool get_running(){return running;}
        queue<char> set_command(char id_command,char valeur);
        //void
    protected:
    private:
        bool running;
        int vit;
        int kp;
        //B2th remote_device;
};

#endif // ROBOT_DATA_H

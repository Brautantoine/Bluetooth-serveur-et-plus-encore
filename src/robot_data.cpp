#include "robot_data.h"


robot_data::robot_data()
{
    //ctor
    //remote_device=remote;
    //remote_device.bind_robot(this);
    running=false;

}
/*robot_data::robot_data(B2th& remote)
{
    //ctor
    remote_device=remote;
    remote_device.bind_robot(&this);
    running=false;

}*/

robot_data::~robot_data()
{
    //dtor
}

queue <char> robot_data::set_command(char id_command,char valeur)
{
    queue<char> msg;
    msg.emplace(id_command);
    msg.emplace(valeur);
    msg.emplace('\0');
    return msg;
}

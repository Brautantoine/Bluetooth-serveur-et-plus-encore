#ifndef API_HPP_INCLUDED
#define API_HPP_INCLUDED
#include "B2th.h"
#include "robot_data.h"
#include "Erreur.hpp"
#include "imgui.h"
#include "ImGui_log_struct.hpp"
#include <SFML/Graphics.hpp>


namespace API
{
   void load_rep(B2th& remote_device,AppLog& debug_log);
   void Barre_principale(bool* P_show_log_debug);
   bool parametre(sf::Color& bgColor,float color[3],char windowTitle[255]);
   void commande_led_window(B2th& remote_device,bool& show_window);
   void raw_data(B2th& remote_device,bool& show_raw_data,std::string& data);
   void set_up(B2th& remote_device,robot_data& otc,bool& show_data_robot);
   void test_rapide(B2th& remote_device);
}


#endif // API_HPP_INCLUDED

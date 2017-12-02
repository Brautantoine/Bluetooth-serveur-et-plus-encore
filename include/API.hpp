#ifndef API_HPP_INCLUDED
#define API_HPP_INCLUDED
#include "B2th.h"
#include "Erreur.hpp"
#include "imgui.h"
#include "ImGui_log_struct.hpp"
#include <SFML/Graphics.hpp>


namespace API
{
   void load_rep(B2th& remote_device,AppLog& debug_log);
   void Barre_principale(bool* P_show_log_debug);
   bool parametre(sf::Color& bgColor,float color[3],char windowTitle[255]);
}


#endif // API_HPP_INCLUDED

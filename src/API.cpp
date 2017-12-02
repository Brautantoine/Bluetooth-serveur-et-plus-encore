#include "API.hpp"



void API::load_rep(B2th& remote_device,AppLog& debug_log)
{
    //static AppLog debug_log;
    try
    {
        remote_device.load_from_txt();
    }
    catch (Erreur const& e)
    {
        if(e.getNumero()==FICHIER_ABSENT)
        {
            remote_device.create_default_txt();
            remote_device.load_from_txt();
            debug_log.AddLog("At:%2f: %s  [%d]",e.when(),e.what(),e.getNumero());
        }
        else if (e.getNumero()==MAC_INVALIDE)
        {
            remote_device.create_default_txt();
            remote_device.load_from_txt();
            debug_log.AddLog("At:%2f: %s  [%d]",e.when(),e.what(),e.getNumero());
        }
    }
}

void API::Barre_principale(bool* P_show_log_debug)
{
    if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Window"))
            {
                ImGui::MenuItem("Log/Debug","Beta",P_show_log_debug); //Rappel show_log_debug est devenue un pointeur
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
}
bool API::parametre(sf::Color& bgColor,float color[3],char windowTitle[255])
{
    bool buff = false;
    ImGui::Begin("Parametres"); // begin window

        // Background color edit
        if (ImGui::ColorEdit3("Background color", color))
        {
            // this code gets called if color value changes, so
            // the background color is upgraded automatically!
            bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
            bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
            bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
        }

        // Window title text edit
        ImGui::InputText("Window title", windowTitle, 255);


        if (ImGui::Button("Update window title"))
        {
            // this code gets if user clicks on the button
            // yes, you could have written if(ImGui::InputText(...))
            // but I do this to show how buttons work :)
            //window.setTitle(windowTitle);
            buff=true;
        }
        ImGui::End(); // end window
        return buff;
}

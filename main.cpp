#include "imgui.h"
#include "imgui-master/imgui-sfml-master/imgui-SFML.h"
#include "const_and_def.hpp"
#include "B2th.h"
#include "version.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/OpenGL.hpp>

using personnal_type::U8;
using std::string;

int main()
{
    bool flag_window_shutdown(false);
    B2th remote_device ; //	00:06:66:7D:5C:AC //00:06:66:6E:00:C6


    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "ZONE de Base");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);

    sf::Color bgColor;

    float color[3] = { 206.F/255.f, 206.F/255.f, 206.F/255.f };
    bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
    bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
    bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);

    // let's use char array as buffer, see next part
    // for instructions on using std::string with ImGui
    char windowTitle[255] = "Dev_Bluetooth";
    char rtx[255];
    char version[]="version : ";
    strcat(version,AutoVersion::FULLVERSION_STRING);
    string data_in("N/A");

    window.setTitle(windowTitle);

    sf::Clock deltaClock;
   // remote_device.create_default_txt();
    while(remote_device.load_from_txt());
    //if(!(remote_device.get_connect_status()))remote_device.connection();
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window,deltaClock.restart());

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
            window.setTitle(windowTitle);
        }
        ImGui::End(); // end window


        ImGui::Begin("Bluetooth");
        if( ImGui::BeginMenu("Adresse",!(remote_device.get_connect_status())))
        {
            for(int i=0;i<remote_device.get_adr_number();i++)
            {
                /*if(!(ImGui::Checkbox((remote_device.get_current_addr_detail(i)).c_str(),remote_device.get_current_flag_control(i))));
                {

                }*/
                if(ImGui::MenuItem((remote_device.get_current_addr_detail(i)).c_str(),NULL,remote_device.get_current_flag_control(i),true))
                {
                   remote_device.trig_adress(i);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::MenuItem("conection status",remote_device.get_dest(),remote_device.get_connect_status(),false);
        if(ImGui::Button("Connexion"))
        {
            if(!(remote_device.get_connect_status()))remote_device.connection();
        }
        ImGui::InputText("Texte à envoyer", rtx, 255);
        if (ImGui::Button("Envoyer"))
        {
            // this code gets if user clicks on the button
            // yes, you could have written if(ImGui::InputText(...))
            // but I do this to show how buttons work :)
            remote_device.set_rtx(rtx);
            remote_device.send_to_remote();
            rtx[0]='\0';
        }
        data_in=remote_device.recv_from_remote();
        ImGui::Text(data_in.c_str());
        if (ImGui::Button("Effacer"))
        {
            // this code gets if user clicks on the button
            // yes, you could have written if(ImGui::InputText(...))
            // but I do this to show how buttons work :)
            remote_device.clear_data_in();

        }
        if(ImGui::Button("Deconnecter"))remote_device.close_connection();
        ImGui::End();

        ImGui::Begin("Shutdown");
        ImGui::Text(version);
        if (ImGui::Button("Quitter"))
        {
            // this code gets if user clicks on the button
            // yes, you could have written if(ImGui::InputText(...))
            // but I do this to show how buttons work :)
            flag_window_shutdown=true;

        }
        ImGui::End();
        if(flag_window_shutdown)
        {
            ImGui::Begin("Etes vous sur ?");
            if (ImGui::Button("OUI"))
            {
                // this code gets if user clicks on the button
                // yes, you could have written if(ImGui::InputText(...))
                // but I do this to show how buttons work :)
                window.close();

            }
            ImGui::SameLine();
            if (ImGui::Button("NON"))
            {
                // this code gets if user clicks on the button
                // yes, you could have written if(ImGui::InputText(...))
                // but I do this to show how buttons work :)
                flag_window_shutdown=false;

            }
            ImGui::End();
        }


        window.clear(bgColor); // fill background with color
        ImGui::Render();
        window.display();
    }

    ImGui::SFML::Shutdown();
    remote_device.close_connection();
}

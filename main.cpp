#include "imgui.h"
#include "imgui-master/imgui-sfml-master/imgui-SFML.h"
#include "const_and_def.hpp"
#include "B2th.h"
#include "version.h"
#include "gestion_fichier.hpp"
#include "ImGui_log_struct.hpp"
#include "math.h"
#include "Erreur.hpp"
#include "API.hpp"
//#include "imgui-master/imgui_demo.cpp"
#include <exception>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/OpenGL.hpp>

using personnal_type::U8;
using std::string;

int main()
{

    static bool no_titlebar = false;
    static bool no_border = true;
    static bool no_resize = true;
    static bool no_move = true;
    static bool no_scrollbar = false;
    static bool no_collapse = false;
    static bool no_menu = false;

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;

    static bool show_log_debug (false);
    static bool repeat_transmission(false);
    static bool show_plot_line_window(false);
    static bool show_B2TH_window(true);
    static bool show_popup_ad(false);
    /// bool flag_window_shutdown(false);

    B2th remote_device ; //	00:06:66:7D:5C:AC //00:06:66:6E:00:C6

    sf::Clock repeatT_clock;

    float treshold_repeatT(0.5);
    std::vector<float> baricentre;
    for(int i=0; i<128; i++)
    {
        baricentre.emplace_back(sin((float)i/10));
    }

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "ZONE de Base");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);

    sf::Color bgColor;
    sf::Image icone;


    static AppLog debug_log;


    if(!icone.loadFromFile("IMG/icone.png"))exit(EXIT_FAILURE);

    window.setIcon(32,32,icone.getPixelsPtr());


    float color[3] = { 206.F/255.f, 206.F/255.f, 206.F/255.f };
    bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
    bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
    bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);

    // let's use char array as buffer, see next part
    // for instructions on using std::string with ImGui
    char windowTitle[255] = "Dev_Bluetooth";
    char rtx[255];
    char detail[255];
    static char adresse[18];
    static char version[19]="version : ";
    strcat(version,AutoVersion::FULLVERSION_STRING);
    version[18]='\0';

    string data_in("N/A");

    window.setTitle(windowTitle);

    sf::Clock deltaClock;

    // remote_device.create_default_txt();
    /*try
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
    }*/
    API::load_rep(remote_device,debug_log);
    ///stdfile::void_log_file();
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

        /**if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Window"))
            {
                ImGui::MenuItem("Log/Debug","Beta",&show_log_debug);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }**/
        API::Barre_principale(&show_log_debug);

        /**ImGui::Begin("Parametres"); // begin window

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
        ImGui::End(); // end window**/
        if(API::parametre(bgColor,color,windowTitle))window.setTitle(windowTitle);


        ImGui::Begin("Bluetooth",NULL,window_flags);
        if(ImGui::BeginMenuBar())
        {
            if(ImGui::BeginMenu("Window"))
            {
                ImGui::MenuItem("Baricentre",NULL,&show_plot_line_window);
                ImGui::EndMenu();
            }
            ///ImGui::EndMenuBar();}
            if( ImGui::BeginMenu("Adresse",!(remote_device.get_connect_status())))
            {
                for(int i=0; i<remote_device.get_adr_number(); i++)
                {

                    if(ImGui::MenuItem((remote_device.get_current_addr_detail(i)).c_str(),NULL,remote_device.get_current_flag_control(i),true))
                    {
                        remote_device.trig_adress(i);
                    } ///     ///
                }
                ImGui::Separator();
                if(ImGui::MenuItem("Ajouter une adresse"))
                {
                    show_popup_ad=true;
                }
                ImGui::EndMenu();

            }
            ImGui::EndMenuBar();/// ///
            if(show_popup_ad)
            {
                ImGui::OpenPopup("Nouvelle adresse");

            }
            if(ImGui::BeginPopupModal("Nouvelle adresse"))
            {
                ImGui::InputText("adresse",adresse,18);
                ImGui::InputText("descriptif",detail,255);
                if(ImGui::Button("Enregistrer"))
                {
                    remote_device.create_new_addr(adresse,detail);
                    adresse[0]='\0';
                    detail[0]='\0';
                    ImGui::CloseCurrentPopup();
                    show_popup_ad=false;
                }
                ImGui::SameLine();
                if(ImGui::Button("Annuler"))
                {
                    adresse[0]='\0';
                    detail[0]='\0';
                    ImGui::CloseCurrentPopup();
                    show_popup_ad=false;
                }
                ImGui::EndPopup();
            }
        }
        ImGui::MenuItem("conection status",remote_device.get_dest(),remote_device.get_connect_status(),false);
        if(ImGui::Button("Connexion"))
        {
            if(!(remote_device.get_connect_status()))remote_device.connection();
        }
        ImGui::InputText("Texte à envoyer", rtx, 255);
        if (ImGui::Button("Envoyer")&&remote_device.get_connect_status())
        {
            // this code gets if user clicks on the button
            // yes, you could have written if(ImGui::InputText(...))
            // but I do this to show how buttons work :)
            remote_device.set_rtx(rtx);
            remote_device.send_to_remote();
            rtx[0]='\0';
        }
        ImGui::SliderFloat("secondes",&treshold_repeatT,0.5,5,"%.1f");
        ImGui::SameLine();
        ImGui::Checkbox("Repeter",&repeat_transmission);
        if(repeat_transmission&&repeatT_clock.getElapsedTime().asSeconds()>treshold_repeatT&&remote_device.get_connect_status())
        {
            remote_device.send_to_remote();
            repeatT_clock.restart();
        }
        data_in=remote_device.recv_from_remote();
        ImGui::TextUnformatted(data_in.c_str());
        if (ImGui::Button("Effacer"))
        {
            // this code gets if user clicks on the button
            // yes, you could have written if(ImGui::InputText(...))
            // but I do this to show how buttons work :)
            remote_device.clear_data_in();

        }
        if(ImGui::Button("Deconnecter"))remote_device.close_connection();
        ImGui::Value("TEMPS",ImGui::GetTime());
        ImGui::End();



        ImGui::Begin("Shutdown");
        ImGui::TextUnformatted(version);
        if (ImGui::Button("Quitter"))
        {
            // this code gets if user clicks on the button
            // yes, you could have written if(ImGui::InputText(...))
            // but I do this to show how buttons work :)
            ImGui::OpenPopup("Êtes vous sûr ?");

        }
        if(ImGui::BeginPopupModal("Êtes vous sûr ?"))
        {
            ImGui::Text("Vous allez quitter le programme");
            if(ImGui::Button("Oui"))window.close();
            ImGui::SameLine();
            if(ImGui::Button("Non"))ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::End();

        if(show_plot_line_window)
        {
            ImGui::Begin("Baricentre",&show_plot_line_window);
            ImGui::PlotLines("Baricentre",baricentre.data(),baricentre.size(),0,NULL,FLT_MAX,FLT_MAX,ImVec2(0,126));
            ImGui::End();
        }

        ImGui::ShowTestWindow();



        if(show_log_debug)debug_log.Draw("DEBUG/LOG",&show_log_debug);

        window.clear(bgColor); // fill background with color
        ImGui::Render();
        window.display();
    }

    ImGui::SFML::Shutdown();
    remote_device.close_connection();
}

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
#include "robot_data.h"
//#include "imgui-master/imgui_demo.cpp"
#include <exception>
#include <thread>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/OpenGL.hpp>

using personnal_type::U8;
using std::string;

int main()
{
    /*** Parametre par defaut des nouvelles fenetres ***/
    static bool no_titlebar = false;
    static bool no_border = true;
    static bool no_resize = true;
    static bool no_move = true;
    static bool no_scrollbar = false;
    static bool no_collapse = false;
    static bool no_menu = false;

    ImGuiWindowFlags window_flags = 0; //création de la variable de parametre
    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;

    static bool show_log_debug (false); //Affichage ou non de la fenetre de debug
    static bool repeat_transmission(false); // Repetition ou non du dernier envoie
    static bool show_plot_line_window(false); //Affichage ou non de la fenetre de test des graphes
//    static bool show_B2TH_window(true); //Affichage ou non de la fenetre bluetooth
    static bool show_popup_ad(false); //Affichage ou non du pop up d'ajout d'adresse
    static bool show_led_command(false); //Affichage ou non de la fenetre de commande de led (Demo JPO)
    static bool show_raw_data(false); //Affichage ou non de la fenetre de reception brut (en construction)
    static bool show_data_robot(false); //Affichage ou non de la fenetre de parametrage du robot (en construction)

    B2th remote_device ; //	00:06:66:7D:5C:AC //00:06:66:6E:00:C6
    robot_data old_type_c ; // instance de commande du robot (toujours en construction)

    sf::Clock repeatT_clock; //timer de repetition

    float treshold_repeatT(0.5); //seuil de repetition
    std::vector<float> baricentre; //tableau de sinus pour le test des graphes
    for(int i=0; i<128; i++) //initialisation du tableau
    {
        baricentre.emplace_back(sin((float)i/10));
    }

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "ZONE de Base"); //création de la fenetre
    window.setVerticalSyncEnabled(true); //activation de la v-sync
    ImGui::SFML::Init(window); // initialisation de la fenetre

    sf::Color bgColor; //couleur de fond
    sf::Image icone; // icone du programme (marche pas tout le temps)


    static AppLog debug_log; // journal de debug


    if(!icone.loadFromFile("IMG/icone.png"))exit(EXIT_FAILURE); //test du chargement de l'icone (crash si l'icone n'est pas presente dans les dossiers)

    window.setIcon(32,32,icone.getPixelsPtr()); // parametrage de l'icone

    /** calcul de la couleur de fond (unitule donc indispensable) **/
    float color[3] = { 206.F/255.f, 206.F/255.f, 206.F/255.f };
    bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
    bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
    bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);


    char windowTitle[10] = "Mac'Arena"; //nouveau nom de la fenetre
    static char rtx[255]; //buffer d'emission
    char detail[255]; //buffer detail nouvelle adresse
    static char adresse[18]; // buffer nouvelle adresse
    static char version[19]="version : "; //conteneur de la chaine de caractere de la version actuel
    strcat(version,AutoVersion::FULLVERSION_STRING); //recuperation de la version actuel
    version[18]='\0'; //ajout de fin de chaine

    std::string data_in; //buffer de reception


    window.setTitle(windowTitle); //mise à jour du titre de la fenetre

    sf::Clock deltaClock; //horloge d'affichage pour IMGUI


    API::load_rep(remote_device,debug_log); //chargement des adresse depuis le fichier config/adress.rep

    /*** Boucle d'affichage et de traitement (mainloop) ***/
    while (window.isOpen())
    {

        sf::Event event; //chargement de la liste d'evenement
        while (window.pollEvent(event)) //traitement de tout les evenement
        {
            ImGui::SFML::ProcessEvent(event); //traitement des evenement relatif au bind Imgui

            if (event.type == sf::Event::Closed) //si l'utilisateur ferme la fenetre
            {
                window.close(); //fermeture du programme
            }
        }

        ImGui::SFML::Update(window,deltaClock.restart()); // Rafraichissement de l'horloge Imgui


        API::Barre_principale(&show_log_debug); //affichage de la barre principale

        if(API::parametre(bgColor,color,windowTitle))window.setTitle(windowTitle);  //affichage de la fenetre des parametres

        /*** Debut de la fenetre bluetooth ***/
        ImGui::Begin("Bluetooth",NULL,window_flags);
        /** Barre contextuelle de la fenetre **/
        if(ImGui::BeginMenuBar()) ///Debut de la barre
        {
            if(ImGui::BeginMenu("Window")) //Affichage ou non des differente fenetre
            {
                ImGui::MenuItem("Baricentre",NULL,&show_plot_line_window); // Toggle fenetre de demo du graphe
                ImGui::MenuItem("Commande Led",NULL,&show_led_command); // Toggle fenetre de commande des led (Demo JPO)
                ImGui::MenuItem("Raw Data","Beta",&show_raw_data); //Toggle fenetre de reception brut
                ImGui::MenuItem("Set up robot","Beta",&show_data_robot); //Toggle fenetre de commande a distance (inutile)
                ImGui::EndMenu(); // fin du menu d'affichage
            }
            if( ImGui::BeginMenu("Adresse",!(remote_device.get_connect_status()))) //Menu des adresse (creation et selection)
            {
                for(int i=0; i<remote_device.get_adr_number(); i++) //Parcour de toutes les adresses disponible
                {

                    if(ImGui::MenuItem((remote_device.get_current_addr_detail(i)).c_str(),NULL,remote_device.get_current_flag_control(i),true)) //si l'adresse actuel est selectionne
                    {
                        remote_device.trig_adress(i); //activation de l'adresse
                    }
                }
                ImGui::Separator(); //ligne de separation
                if(ImGui::MenuItem("Ajouter une adresse")) //Ajout d'une nouvelle adresse
                {
                    show_popup_ad=true; //Activation du pop up
                }
                ImGui::EndMenu(); //fin du menu

            }
            ImGui::EndMenuBar();/// Fin de la barre
            if(show_popup_ad) //si le pop up est actif
            {
                ImGui::OpenPopup("Nouvelle adresse"); ///ouverture du pop up

            }
            if(ImGui::BeginPopupModal("Nouvelle adresse")) //si le pop up est ouvert
            {
                /** Recuperation de la nouvelle adresse est de son descriptif **/
                ImGui::InputText("adresse",adresse,18);
                ImGui::InputText("descriptif",detail,255);
                if(ImGui::Button("Enregistrer"))
                {
                    remote_device.create_new_addr(adresse,detail); //enregistrement de la nouvelle adresse
                    adresse[0]='\0';// clean du buffer
                    detail[0]='\0';// clean du buffer
                    ImGui::CloseCurrentPopup(); ///fermeture du pop up
                    show_popup_ad=false; //desactivation du pop up
                }
                ImGui::SameLine();
                if(ImGui::Button("Annuler"))
                {
                    adresse[0]='\0';
                    detail[0]='\0';
                    ImGui::CloseCurrentPopup();
                    show_popup_ad=false;
                }
                ImGui::EndPopup();//mise en pause du pop up entre chaque boucle d'affiche lorsque l'utilisateur n'effectue aucune action
            }
        }
        ImGui::MenuItem("conection status",remote_device.get_dest(),remote_device.get_connect_status(),false); //Affichage de l'etat de connection (non fiable)
        if(ImGui::Button("Connexion"))
        {
            if(!(remote_device.get_connect_status()))remote_device.connection(); ///Connection vers le peripheriques bluetooth
        }
        ImGui::InputText("Texte à envoyer", rtx, 255); //recuperation du message a envoyer
        if (ImGui::Button("Envoyer")&&remote_device.get_connect_status()) //verification de la connection avant envoie
        {
            remote_device.set_rtx(rtx); //preparation du message
            remote_device.send_to_remote(); //envoie du message
            rtx[0]='\0'; //raz du buffer
        }
        ImGui::SliderFloat("secondes",&treshold_repeatT,0.5,5,"%.1f"); //slider pour regler le timer de repetition
        ImGui::SameLine();
        ImGui::Checkbox("Repeter",&repeat_transmission); //activation de la repetition
        if(repeat_transmission&&repeatT_clock.getElapsedTime().asSeconds()>treshold_repeatT&&remote_device.get_connect_status())//Repetition du message
        {
            remote_device.send_to_remote();
            repeatT_clock.restart();
        }
        ImGui::TextUnformatted(remote_device.get_buff_rec().c_str()); //Affichage du buffer de reception
        if (ImGui::Button("Effacer"))
        {
            remote_device.clear_buff_rec(); //effacement du buffer de reception (buffer de l'instance remote_device)
        }
        if(ImGui::Button("Deconnecter"))remote_device.close_connection(); ///Deconnection du peripheriques
        ImGui::Value("TEMPS",ImGui::GetTime()); //Affichage du temps d'execution
        ImGui::End(); ///Fin de la fenetre



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

        if(show_led_command)API::commande_led_window(remote_device,show_led_command);

        if(show_log_debug)debug_log.Draw("DEBUG/LOG",&show_log_debug);

        if(show_raw_data)API::raw_data(remote_device,show_raw_data,data_in);

        if(show_data_robot)API::set_up(remote_device,old_type_c,show_data_robot);

        window.clear(bgColor); // fill background with color
        ImGui::Render();
        window.display();
        std::this_thread::yield();
    }

    ImGui::SFML::Shutdown();
    remote_device.close_connection();
    remote_device.close_thread();
}

#include "API.hpp"
#include "cstring"


/*struct ImGuiTextBuffer
{
    ImVector<char>      Buf;

    ImGuiTextBuffer()   { Buf.push_back(0); }
    inline char         operator[](int i) { return Buf.Data[i]; }
    const char*         begin() const { return &Buf.front(); }
    const char*         end() const { return &Buf.back(); }      // Buf is zero-terminated, so end() will point on the zero-terminator
    int                 size() const { return Buf.Size - 1; }
    bool                empty() { return Buf.Size <= 1; }
    void                clear() { Buf.clear(); Buf.push_back(0); }
    const char*         c_str() const { return Buf.Data; }
    IMGUI_API void      append(const char* fmt, ...) IM_FMTARGS(2);
    IMGUI_API void      appendv(const char* fmt, va_list args) IM_FMTLIST(2);
};*/


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

void API::commande_led_window(B2th& remote_device,bool& show_window)
{
    ImGui::Begin("Commande Led",&show_window);
    if(ImGui::Button("Allumer en rouge")&&remote_device.get_connect_status())test_rapide(remote_device);//remote_device.send_to_remote(1);
    ImGui::SameLine();
    if(ImGui::Button("Allumer en vert")&&remote_device.get_connect_status())remote_device.send_to_remote(1);
    if(ImGui::Button("Allumer en bleu")&&remote_device.get_connect_status())remote_device.send_to_remote(128);
    ImGui::SameLine();
    if(ImGui::Button("Eteindre")&&remote_device.get_connect_status())remote_device.send_to_remote('\0');
    ImGui::End();
}

void API::raw_data(B2th& remote_device,bool& show_raw_data,std::string& data)
{
    //std::queue <char> fifo (remote_device.get_data_log());
    static int lines=0;
    static int last_data_size=0;
    static ImGuiTextBuffer log(remote_device.get_log());
    /*if(remote_device.get_connect_status())for(;last_data_size<data.size();last_data_size++){log.append("%s",remote_device.get_data_raw().at(last_data_size));if(log.size()>89+89*lines)
    {
        log.append("\n");
        lines++;
    }}*/
    /*for(int i=0;i<fifo.size();i++)
    {
        log.append("%c",fifo.front());
        if(log.size()>89+89*lines)
    {
        log.append("\n");
        lines++;
    }
    }*/


    ImGui::Begin("Apperçue données brutes",&show_raw_data);
    if(ImGui::Button("Effacer")){remote_device.clear_data_in();log.clear();}
    ImGui::Separator();
    ImGui::BeginChild("brut");
    if(remote_device.get_connect_status())ImGui::TextUnformatted(log.begin(),log.end());
    else ImGui::TextUnformatted("Déconnecter");
    ImGui::EndChild();
    ImGui::End();
}

void API::set_up(B2th& remote_device,robot_data& otc,bool& show_data_robot)
{
    static string buff_command;
    static int vit,vit_robot,kp,kp_robot;

    buff_command.clear();
    ImGui::Begin("Set up Robot",&show_data_robot);
    if(otc.get_running())
    {
        ImGui::Text("Robot en course");
        if(ImGui::Button("Aller à : Suivie de course"));/// TO DO ///
    }
    else
    {
        ImGui::Text("Vitesse");
        ImGui::InputInt("Valeur",&vit,1,10);
        ImGui::SameLine();
        ImGui::Text(": %d",vit_robot);
        ImGui::SameLine();
        if(ImGui::Button("Set")&&remote_device.get_connect_status())
        {
            remote_device.send_to_remote(otc.set_command(0x81,vit));
            //buff_command.assign(otc.set_command("0X42",5692));
            //char* buff = new char[buff_command.length()+1];
            //std::strcpy(buff,buff_command.c_str());
            //remote_device.set_rtx(buff);
            //remote_device.send_to_remote();
            //delete[] buff;
        }
        ImGui::TextUnformatted(buff_command.c_str());
    }
    ImGui::End();
}

void API::test_rapide(B2th& remote_device)
{
    queue<char> msg;
    msg.emplace(128);
    msg.emplace(1);
    msg.emplace('\0');
    remote_device.send_to_remote(msg);

}


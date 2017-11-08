#ifndef REMOTE_ADRESS_H
#define REMOTE_ADRESS_H
#include <string>

class remote_adress
{
    public:
        remote_adress(int x,std::string y,std::string z);
        virtual ~remote_adress();
        bool* Getflag_use() { return &flag_use; }
        void Setflag_use(bool val) { flag_use = val; }
        std::string Getchain_info() { return chain_info; }
        std::string get_fullstring() { return fullstring; }
        const char* Getremote_adr() { return remote_adr.c_str(); }
    protected:
    private:
        bool flag_use;
        std::string chain_info;
        std::string fullstring;
        int ordre;
        std::string remote_adr;
};

#endif // REMOTE_ADRESS_H

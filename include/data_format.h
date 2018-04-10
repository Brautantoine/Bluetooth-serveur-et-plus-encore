#ifndef DATA_FORMAT_H
#define DATA_FORMAT_H
#include <vector>

using std::vector;


class data_format
{
    public:
        data_format(int ext_KP,unsigned char ext_KD,unsigned char mem_a,unsigned char mem_b);
        virtual ~data_format();
        vector<unsigned char> Gettab() { return tab; }
        //void Settab(unsigned char val,int i) { tab[i] = val; }
        unsigned char Getpic_a() { return pic_a; }
        void Setpic_a(unsigned char val) { pic_a = val; }
        unsigned char Getpic_b() { return pic_b; }
        void Setpic_b(unsigned char val) { pic_b = val; }
        unsigned char GetKD() { return KD; }
        void SetKD(unsigned char val) { KD = val; }
        int GetKP() { return KP; }
        void SetKP(int val) { KP = val; }
    protected:
    private:
        vector<unsigned char> tab;
        unsigned char pic_a;
        unsigned char pic_b;
        int KP;
        unsigned char KD;
};

#endif // DATA_FORMAT_H

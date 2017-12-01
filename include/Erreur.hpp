#ifndef ERREUR_HPP_INCLUDED
#define ERREUR_HPP_INCLUDED

#include <exception>

using namespace std;



class Erreur: public exception

{

public:

    Erreur(float temps, int numero, string const& phrase="", int niveau=0) throw()

         :m_temps(temps),m_numero(numero),m_phrase(phrase),m_niveau(niveau)

    {}



     virtual const char* what() const throw()

     {

         return m_phrase.c_str();

     }



     int getNiveau() const throw()

     {

          return m_niveau;

     }

     int getNumero() const throw()

     {

         return m_numero;

     }

     float when() const throw()
     {
        return m_temps;
     }



    virtual ~Erreur() throw()

    {}



private:

    int m_numero;               //ticket de l'erreur

    string m_phrase;            //Description de l'erreur

    int m_niveau;               //Niveau de l'erreur

    float m_temps;              //temps de l'erreur

};


#endif // ERREUR_HPP_INCLUDED

#ifndef ERREUR_HPP_INCLUDED
#define ERREUR_HPP_INCLUDED

#include <exception>

using namespace std;



class Erreur: public exception

{

public:

    Erreur(float numero=0, string const& phrase="", int niveau=0) throw()

         :m_numero(numero),m_phrase(phrase),m_niveau(niveau)

    {}



     virtual const char* what() const throw()

     {
         std::string buff = std::to_string(m_numero)+ " : " +m_phrase;
         return buff.c_str();

     }



     int getNiveau() const throw()

     {

          return m_niveau;

     }

     float getNumero() const throw()

     {

         return m_numero;

     }



    virtual ~Erreur() throw()

    {}



private:

    float m_numero;               //heure de l'erreur

    string m_phrase;            //Description de l'erreur

    int m_niveau;               //Niveau de l'erreur

};


#endif // ERREUR_HPP_INCLUDED

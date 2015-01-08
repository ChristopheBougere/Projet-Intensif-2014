#include "observable.h"

Observable::Observable()
{
}

void Observable::AddObs(Observer *obs)
{
    //on ajoute l'observateur à notre liste
    m_list.push_back(obs);

    //et on lui donne un nouvel objet observé.
    obs->AddObs(this);
}

void Observable::DelObs(Observer *obs)
{
  //même chose que dans Observateur::DelObs
   iterator it= find(m_list.begin(),m_list.end(),obs);
    if(it != m_list.end())
       m_list.erase(it);
}


Observable::~Observable()
{
       //même chose qu'avec Observateur::~Observateur
       iterator itb=m_list.begin();
       const_iterator ite=m_list.end();

       for(;itb!=ite;++itb)
       {
               (*itb)->DelObs(this);
       }
}

void Observable::Notify(void)
{
        //on prévient chaque observateur que l'on change de valeur
        iterator itb = m_list.begin();
        const_iterator ite = m_list.end();

       for( ; itb!=ite ; ++itb)
       {
               (*itb)->Update(this);
       }
}

#include "observer.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

Observer::Observer()
{


}

void Observer::Update(const Observable* observable) const
{
  //on affiche l'état de la variable
  cout<<observable->Statut()<<endl;
}

Observer::~Observer()
{
       //pour chaque objet observé,
        //on lui dit qu'on doit supprimer l'observateur courant
       const_iterator ite=m_list.end();

       for(iterator itb=m_list.begin();itb!=ite;++itb)
       {
               (*itb)->DelObs(this);
       }
}

void Observer::AddObs( Observable* obs)
{
    m_list.push_back(obs);
}

void Observer::DelObs(Observable* obs)
{
    //on enlève l'objet observé.
   iterator it= std::find(m_list.begin(),m_list.end(),obs);
    if(it != m_list.end())
       m_list.erase(it);
}

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

#include "observer.h"

class Observer;

class Observable
{
 public:
    Observable();
    virtual ~Observable();

    void AddObs(Observer* obs);
    void DelObs(Observer* obs);

    virtual std::string Statut(void) const =0;

 protected:
    void Notify(void);

    std::list<Observer*> m_list;

    typedef std::list<Observer*>::iterator iterator;
    typedef std::list<Observer*>::const_iterator const_iterator;
};

#endif // OBSERVABLE_H

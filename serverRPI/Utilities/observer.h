#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

#include "observable.h"

class Observable;

class Observer
{
public:
    Observer();
    virtual ~Observer() = 0;
    virtual void Update(const Observable* observable) const ;

    void AddObs(Observable* obs);
    void DelObs(Observable* obs);

protected:
  std::list<Observable*> m_list;
  typedef std::list<Observable*>::iterator iterator;
  typedef std::list<Observable*>::const_iterator const_iterator;



};

#endif // OBSERVER_H

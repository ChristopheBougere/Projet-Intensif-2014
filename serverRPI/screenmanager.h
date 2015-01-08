#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <cstring>
#include <string>
#include "Utilities/observer.h"

typedef void (*ScreenCallback)(char *);

class ScreenManager : public Observer
{
public:
    ScreenManager(ScreenCallback _med_notify, ScreenCallback _drawer_notify);
    ~ScreenManager();
    void Update(const Observable* observable) const;
private:
    ScreenCallback med_notify, drawer_notify;

};
#endif

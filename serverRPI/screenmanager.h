#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <cstring>
#include <string>
#include "Utilities/observer.h"
#include "Utilities/observable.h"
#include "screen.h"

typedef void (*ScreenCallback)(char *);

class ScreenManager : public Observer, public Observable
{
public:
    ScreenManager(ScreenCallback _med_notify, ScreenCallback _drawer_notify, ScreenCallback _fall_notify);
    ~ScreenManager();
    void Update(const Observable* observable) const;
    std::string Statut(void) const;
private:
    ScreenCallback med_notify, drawer_notify, fall_notify;
};
#endif

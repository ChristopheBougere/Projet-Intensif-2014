#ifndef FALLDOWN_H
#define FALLDOWN_H

#include "../Utilities/observable.h"

#include <string>
#include <thread>

class Falldown : public Observable
{
public:
    Falldown();
    void Change(int valeur);
    std::string Statut(void) const;

private:
    
    void spoofServer();

    int _criticity;
};

#endif // FALLDOWN_H

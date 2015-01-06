#ifndef FALLDOWN_H
#define FALLDOWN_H

#include "../Utilities/observable.h"

class Falldown : public Observable
{
public:
    Falldown();
    void Change(int valeur);
    int Statut(void) const;

private:
    int _criticity;
};

#endif // FALLDOWN_H
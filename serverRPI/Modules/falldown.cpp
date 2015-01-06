#include "falldown.h"

Falldown::Falldown()
{
}

void Falldown::Change(int valeur)
{
    _criticity = valeur;
    Notify();
}

int Falldown::Statut(void) const
{
       return _criticity;
}

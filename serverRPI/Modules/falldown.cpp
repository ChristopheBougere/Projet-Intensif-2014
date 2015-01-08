#include "falldown.h"

Falldown::Falldown()
{
	std::cout << "FallDown() : demarrage du module" << std::endl;
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

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

std::string  Falldown::Statut(void) const
{
       return "falldown";//std:to_string(_criticity);
}

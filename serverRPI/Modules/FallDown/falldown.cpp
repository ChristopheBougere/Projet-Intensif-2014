#include "falldown.h"

Falldown::Falldown()
{
	std::cout << "FallDown() : demarrage du module" << std::endl;

	std::thread spoofer(&Falldown::spoofServer,this);

	if (spoofer.joinable()) {
	   spoofer.join();
	}

}

void Falldown::Change(int valeur)
{
    _criticity = valeur;
    Notify();
}

std::string Falldown::Statut(void) const
{
       return "falldown";
}

void Falldown::spoofServer() {
   
   std::cout << "spoofer launched" << endl;

   
}

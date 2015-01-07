#include <iostream>

#include "Manager/kernelmanager.h"
#include "Modules/falldown.h"

using namespace std;

int main()
{
	Kernel kernel = KernelManager::Instance();

	std::cout << "main() : lancement du serveur." << endl;
	
	Falldown falldownCaptor;
	falldownCaptor.AddObs(&kernel);

	MedDistrib medDistrib;
	medDistrib.AddObs(&kernel);

    return 0;
}


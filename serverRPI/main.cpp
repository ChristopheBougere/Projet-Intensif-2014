#include <iostream>

#include "Manager/kernelmanager.h"
#include "Modules/falldown.h"
#include "Modules/MedDistrib/meddistrib.h"

#include "screen.h"

using namespace std;

int main()
{
	Kernel kernel = KernelManager::Instance();

	std::cout << "main() : lancement du serveur." << endl;
	
	Falldown falldownCaptor;
	falldownCaptor.AddObs(&kernel);

	MedDistrib medDistrib;
	medDistrib.AddObs(&kernel);

	screen();
	
    return 0;
}



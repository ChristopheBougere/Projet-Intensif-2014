#include <iostream>

#include "Manager/kernelmanager.h"
#include "Modules/falldown.h"
#include "Modules/MedDistrib/meddistrib.h"

#include "screenmanager.h"
#include "screen.h"

using namespace std;

int main()
{
	Kernel kernel = KernelManager::Instance();

	std::cout << "main() : lancement du serveur." << endl;
	
	Falldown falldownCaptor;
	falldownCaptor.AddObs(&kernel);

	ScreenManager screenmanager(*notify_med, *notify_drawer);

	MedDistrib medDistrib;
	medDistrib.AddObs(&kernel);
	medDistrib.AddObs(&screenmanager);

	screen();
	
    return 0;
}


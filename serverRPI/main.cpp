#include <iostream>

#include "Manager/kernelmanager.h"
#include "Modules/FallDown/falldown.h"
#include "Modules/MedDistrib/meddistrib.h"

#include "screenmanager.h"
#include "screen.h"

using namespace std;

int main()
{
	Kernel kernel = KernelManager::Instance();

	std::cout << "main() : lancement du serveur." << endl;

	ScreenManager screenmanager(*notify_med, *notify_drawer, *notify_fall);
		
	Falldown falldownCaptor;
	falldownCaptor.AddObs(&kernel);
        falldownCaptor.AddObs(&screenmanager);
	
	screenmanager.AddObs(&falldownCaptor);
	
	MedDistrib medDistrib;
	medDistrib.AddObs(&kernel);
	medDistrib.AddObs(&screenmanager);

	screen();
	
    return 0;
}


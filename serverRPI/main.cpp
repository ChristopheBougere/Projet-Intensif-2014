#include <iostream>

#include "Manager/kernelmanager.h"
#include "Modules/falldown.h"

using namespace std;

int main()
{
    Falldown falldownCaptor;

   {
       Kernel kernel = KernelManager::Instance();

       falldownCaptor.AddObs(&kernel);
       falldownCaptor.Change(3);
   }


    return 0;
}


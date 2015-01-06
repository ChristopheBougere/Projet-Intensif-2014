#include <iostream>
#include "kernelmanager.h"

using namespace std;

Kernel KernelManager::_kernel=Kernel();

KernelManager::KernelManager()
{
    cout<<"Creation"<<endl;
}

KernelManager::~KernelManager()
{
    cout<<"Destruction"<<endl;
}

Kernel KernelManager::Instance()
{
    return _kernel;
}

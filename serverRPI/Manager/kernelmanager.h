#ifndef KERNELMANAGER_H
#define KERNELMANAGER_H

#include "../Utilities/kernel.h"

class KernelManager
{
public:
    KernelManager();
    ~KernelManager();

    static Kernel Instance();

private:
    KernelManager& operator= (const KernelManager&){}
    KernelManager (const KernelManager&){}

    static Kernel _kernel;

};

#endif // KERNELMANAGER_H

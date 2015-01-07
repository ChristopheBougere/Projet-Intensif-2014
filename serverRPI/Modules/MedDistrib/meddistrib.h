#ifndef MEDDISTRIB_H
#define MEDDISTRIB_H

#include "../../Utilities/observable.h"
#include "posology.h"

#include <string>
#include <vector>
#include <thread>

class MedDistrib : public Observable
{
public:
    MedDistrib();    
    int Statut(void) const;

    void addPosology(std::string name, int quantity, int hour, int minutes, int seconds);
    void checkPosology(Posology *poso);
    void getPosology(void);

private:
    std::vector<Posology> _posologyList;
    std::vector<std::thread> _threadList;


};

#endif // MEDDISTRIB_H

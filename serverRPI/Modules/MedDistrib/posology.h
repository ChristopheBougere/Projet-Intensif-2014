#ifndef POSOLOGY_H
#define POSOLOGY_H

#include <string>
#include <vector>

#include "posologystruct.h"
#include "../../Utilities/timeu.h"

class Posology
{
public:
    Posology();
    void setTimeU(int hour, int minute, int second);
    TimeU getTimeU(void);
    void addPosology(std::string name, int quantity);
    std::vector<PosologyStruct> * getPosology(void);

private:
    TimeU _time;
    std::vector<PosologyStruct> *_posology;
};

#endif // POSOLOGY_H

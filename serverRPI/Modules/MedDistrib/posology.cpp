#include "posology.h"

Posology::Posology()
{
    _posology = new std::vector<PosologyStruct>();
}

void Posology::setTimeU(int hour, int minute, int second)
{
    _time.setHours(hour);
    _time.setMinutes(minute);
    _time.setSeconds(second);
}

TimeU Posology::getTimeU()
{
    return _time;
}

void Posology::addPosology(std::string name, int quantity)
{
    PosologyStruct newMed;
    newMed.setName(name);
    newMed.addQuantity(quantity);

    _posology->push_back(newMed);
}

std::vector<PosologyStruct> *Posology::getPosology()
{
    return _posology;
}

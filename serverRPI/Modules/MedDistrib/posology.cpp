#include "posology.h"
 
Posology::Posology()
{
    _posology = new std::vector<PosologyStruct>();
    _alert = new AlertLevel();
    _alert->setType(1);
}

Posology::~Posology() {
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

std::string Posology::getStringPosology()
{
    std::string res = "";

    for(int c = 0 ; c < (int) _posology->size() ; c++)
    {
        PosologyStruct tmpPoso = _posology->at(c);
        res += std::to_string(tmpPoso.getQuantity()) + " " + tmpPoso.getName() + "\n";
    }

    return res;
}


AlertLevel* Posology::getAlert()
{
    return _alert;
}

#ifndef POSOLOGY_H
#define POSOLOGY_H

#include <string>
#include <vector>

#include "posologystruct.h"
#include "../../Utilities/timeu.h"
#include "../../Utilities/alertlevel.h"

class Posology
{
public:
    Posology();
    ~Posology();
    void setTimeU(int hour, int minute, int second);
    TimeU getTimeU(void);
    void addPosology(std::string name, int quantity);
    std::vector<PosologyStruct> * getPosology(void);
    std::string getStringPosology(void);
    AlertLevel* getAlert(void);

private:
    TimeU _time;
    std::vector<PosologyStruct> *_posology;
    AlertLevel* _alert;
};

#endif // POSOLOGY_H

#include "alertlevel.h"

AlertLevel::AlertLevel()
{
    _criticityLevel = 0;
    _isActive = false;
}

void AlertLevel::disableAlert()
{
    _isActive = false;
    _criticityLevel = 1;
}

int AlertLevel::getCriticityLevel()
{
    if(_isActive)
    {
        return _criticityLevel;
    }

    return 0;
}

int AlertLevel::getType()
{
    return _type;
}

void AlertLevel::setType(int value)
{
    _type = value;
}

void AlertLevel::updateCriticity()
{
    if(!_isActive)
    {
        _isActive = true;
        _criticityLevel = 0;
    }
    if(_isActive)
    {
        if(_criticityLevel < 3)
        {
            _criticityLevel++;
        }
    }
}

bool AlertLevel::getAlert()
{
    return _isActive;
}



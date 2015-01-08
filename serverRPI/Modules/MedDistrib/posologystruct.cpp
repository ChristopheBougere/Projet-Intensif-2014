#include "posologystruct.h"

PosologyStruct::PosologyStruct()
{
}

void PosologyStruct::setName(std::string name)
{
    _name = name;
}

std::string PosologyStruct::getName()
{
    return _name;
}

void PosologyStruct::addQuantity(int quantity)
{
    _quantity = quantity;
}

int PosologyStruct::getQuantity()
{
    return _quantity;
}

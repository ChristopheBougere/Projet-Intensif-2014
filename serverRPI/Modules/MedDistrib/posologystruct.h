#ifndef POSOLOGYSTRUCT_H
#define POSOLOGYSTRUCT_H

#include <string>

class PosologyStruct
{
public:
    PosologyStruct();
    void setName(std::string name);
    std::string getName(void);
    void addQuantity(int quantity);
    int getQuantity(void);

private:
    int _quantity;
    std::string _name;
};

#endif // POSOLOGYSTRUCT_H

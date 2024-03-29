#ifndef ALERTLEVEL_H
#define ALERTLEVEL_H

#include <string>

class AlertLevel
{
public:
    AlertLevel();
    void disableAlert(void);
    int getCriticityLevel(void);
    int getType(void);
    void setType(int value);
    void updateCriticity(void);
    bool getAlert(void);

private:
    int _criticityLevel;
    bool _isActive;
    int _type;
};

#endif // ALERTLEVEL_H

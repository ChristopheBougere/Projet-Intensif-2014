#ifndef ALERTLEVEL_H
#define ALERTLEVEL_H

#include <string>

class AlertLevel
{
public:
    AlertLevel();
    AlertLevel(int type);
    void activateAlert(void);
    void disableAlert(void);
    int getCriticityLevel(void);
    int getType(void);
    void updateCriticity(void);

private:
    int _criticityLevel;
    bool _isActive;
    int _type;
};

#endif // ALERTLEVEL_H

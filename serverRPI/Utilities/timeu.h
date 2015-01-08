#ifndef TIMEU_H
#define TIMEU_H

#include <stdio.h>

class TimeU
{
public:
    TimeU();
    TimeU(int hours, int minutes, int seconds);
    void setRealTime(void);
    void setHours(int hours);
    void setMinutes(int minutes);
    void setSeconds(int seconds);
    int getHours(void);
    int getMinutes(void);
    int getSeconds(void);
    int Difference(TimeU *time2H);
    int calculateSec(void);
    void printTime(void);

private:
    int _seconds;
    int _minutes;
    int _hours;
};

#endif // TIMEU_H

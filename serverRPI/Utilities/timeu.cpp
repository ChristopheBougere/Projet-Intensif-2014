#include "timeu.h"
#include <ctime>
#include <iostream>

TimeU::TimeU()
{
}

TimeU::TimeU(int hours, int minutes, int seconds)
{
    _hours = hours;
    _minutes = minutes;
    _seconds = seconds;
}

void TimeU::setRealTime()
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    _hours = now->tm_hour;
    _minutes = now->tm_min;
    _seconds = now->tm_sec;
}

void TimeU::setHours(int hours)
{
    _hours = hours;
}

void TimeU::setMinutes(int minutes)
{
    _minutes = minutes;
}

void TimeU::setSeconds(int seconds)
{
    _seconds = seconds;
}

int TimeU::getHours()
{
    return _hours;
}

int TimeU::getMinutes()
{
    return _minutes;
}

int TimeU::getSeconds()
{
    return _seconds;
}

int TimeU::Difference(TimeU *time2H)
{

    int timeelapsed = 0;

    if(this->_hours >= 0 && this->_hours <= 2)
    {
        timeelapsed = time2H->calculateSec() - this->calculateSec();
        return timeelapsed;
    }

    if(this->_hours >= 3 && this->_hours <= 23)
    {
        int secDay = 24 * 3600;
        timeelapsed = (secDay - this->calculateSec()) + time2H->calculateSec();
        return timeelapsed;
    }
}

int TimeU::calculateSec() {
    int hour = _hours * 3600;
    int min = _minutes * 60;
    int totalseconds1=hour+min+_seconds;
    return totalseconds1;
}

void TimeU::printTime()
{
    std::cout << _hours << " heu " << _minutes << " min " << _seconds << " sec" << std::endl;
}

#include "SP3NavigationSlot.h"


SP3NavigationSlot::SP3NavigationSlot(GPSTime time){
    this->time = time;
}

void SP3NavigationSlot::addData(std::string prn, double x, double y, double z, double t)
{
    PRN.push_back(prn); X.push_back(x); Y.push_back(y); Z.push_back(z); T.push_back(t);
}

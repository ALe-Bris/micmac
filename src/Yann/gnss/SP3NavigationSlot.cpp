#include "SP3NavigationSlot.h"

SP3NavigationSlot::SP3NavigationSlot(){
}

SP3NavigationSlot::SP3NavigationSlot(GPSTime time){
    this->time = time;
}

SP3NavigationSlot::SP3NavigationSlot(SP3NavigationSlot const& ns){
    this->time=ns.time;
    this->PRN=ns.PRN;
    this->X=ns.X;
    this->Y=ns.Y;
    this->Z=ns.Z;
    this->T=ns.T;
}

void SP3NavigationSlot::addData(std::string const& prn, double x, double y, double z, double t){
    PRN.push_back(prn); X.push_back(x); Y.push_back(y); Z.push_back(z); T.push_back(t);
}

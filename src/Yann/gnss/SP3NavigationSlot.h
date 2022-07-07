#ifndef SP3NAVIGATIONSLOT_H
#define SP3NAVIGATIONSLOT_H

#include "Utils.h"
#include "GPSTime.h"
#include "ECEFCoords.h"

// ---------------------------------------------------------------
// Classe contenant un paquet de données de navigation SP3
// ---------------------------------------------------------------
class SP3NavigationSlot {

	public:

		GPSTime time;

		std::vector<std::string> PRN;

		std::vector<double> X;
		std::vector<double> Y;
		std::vector<double> Z;
		std::vector<double> T;

    SP3NavigationSlot();
    SP3NavigationSlot(GPSTime time);
    SP3NavigationSlot(SP3NavigationSlot const&);

    void addData(std::string const& prn, double x, double y, double z, double t);

};

#endif

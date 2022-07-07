#ifndef GPSTIME_H
#define GPSTIME_H

#include "Utils.h"

// ---------------------------------------------------------------
// Classe permettant de gérer le système de temps GPS
// ---------------------------------------------------------------

class GPSTime {

	public:

		int day = 0;
		int month = 0;
		int year = 0;
		int hour = 0;
		int min = 0;
		int sec = 0;
		int ms = 0;
		int wt_week = 0;
		int wt_sec = 0;

    GPSTime(int year, int month, int day, int hour, int min, int sec, int ms);
	
    GPSTime(int year, int month, int day, int hour, int min, int sec);

    GPSTime(long seconds=0);

    GPSTime(int week, int sec);
	
	
    // Lecture d'une chaîne de caractères
    // Format : "dd*MM*yyyy*hh*mm*ss
    GPSTime(std::string time);

    // Comparaisons de dates
    bool operator< (GPSTime t);
    bool operator> (GPSTime t);
    bool operator== (GPSTime t);
    bool operator<= (GPSTime t);
    bool operator>= (GPSTime t);

    // Differences entre temps en secondes
    double operator- (GPSTime t);

    // Conversion en heure siderale (a greenwich)
    double gast();
	
    double convertToAbsTime();

    // Ajout de secondes
    GPSTime addSeconds(int sec);

    // Conversion en texte
    std::string to_string();
    std::string to_formal_string();
    std::string to_complete_string();

};

// toString override
inline std::ostream & operator<<(std::ostream & Str, GPSTime tps) {
	Str << tps.to_string();
	return Str;
}

#endif

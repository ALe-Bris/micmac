#include "GPSTime.h"



GPSTime::GPSTime(int year, int month, int day, int hour, int min, int sec, int ms){
    
    this->year = year;
    this->month = month;
    this->day = day;
    this->hour = hour;
    this->min = min;
    this->sec = sec;
    this->ms = ms;
    
    this->wt_week = ((int)(this->convertToAbsTime())-TIME_T_ORIGIN)/SECONDS_WEEK;
    this->wt_sec = ((int)(this->convertToAbsTime())-TIME_T_ORIGIN)%SECONDS_WEEK;
}
/*
 GPSTime::GPSTime(GPSTime const& t):
 year(t.year), month(t.month), hour(t.hour), min(t.min), sec(t.sec), ms(t.ms)
 {
        this->wt_week = ((int)(this->convertToAbsTime())-TIME_T_ORIGIN)/SECONDS_WEEK;
        this->wt_sec = ((int)(this->convertToAbsTime())-TIME_T_ORIGIN)%SECONDS_WEEK;
 }
*/
GPSTime::GPSTime(int year, int month, int day, int hour, int min, int sec){
    new (this) GPSTime(year, month, day, hour, min, sec, 0);
}

GPSTime::GPSTime(long seconds){
    
    time_t t = (long)seconds;
    struct tm * ptm = gmtime(&t);
    
    new (this) GPSTime(1900+ptm->tm_year,ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
    
}

GPSTime::GPSTime(int week, int sec){
    
    long t = (long)week*SECONDS_WEEK + TIME_T_ORIGIN + (long)((sec>0.0)?sec+0.5:sec-0.5);
    
    new (this) GPSTime(t);
    
}

// Lecture d'une chaîne de caractères
// Format : "dd*MM*yyyy*hh*mm*ss
GPSTime::GPSTime(std::string time) {
    
    int day = std::stoi(time.substr(0,2));
    int mon = std::stoi(time.substr(3,2));
    int yea = std::stoi(time.substr(6,4));
    int hou = std::stoi(time.substr(11,2));
    int min = std::stoi(time.substr(14,2));
    int sec = std::stoi(time.substr(17,2));
    
    new (this)  GPSTime(yea, mon, day, hou, min, sec);
    
}

// Comparaisons de dates
bool GPSTime::operator< (GPSTime t){return this->convertToAbsTime() < t.convertToAbsTime();}
bool GPSTime::operator> (GPSTime t){return this->convertToAbsTime() > t.convertToAbsTime();}
bool GPSTime::operator== (GPSTime t){return this->convertToAbsTime() == t.convertToAbsTime();}
bool GPSTime::operator<= (GPSTime t){return this->convertToAbsTime() <= t.convertToAbsTime();}
bool GPSTime::operator>= (GPSTime t){return this->convertToAbsTime() >= t.convertToAbsTime();}

// Differences entre temps en secondes
double GPSTime::operator- (GPSTime t){return this->convertToAbsTime() - t.convertToAbsTime();}


// Ajout de secondes
GPSTime GPSTime::addSeconds(int sec){
    GPSTime tps(this->convertToAbsTime() + sec);
    tps.ms = this->ms;
    return tps;
}



double GPSTime::convertToAbsTime(){

    int days_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};

	long days=0L;

	// Nombre de jours depuis le 01/01/1970
	for (int i=TIME_T_BASE_YEAR; i<this->year; i++){
		days += (i%4==0)?366:365;
	}

	// Nombre de jours de l'année courante
	for (int i=1; i<this->month; i++){
		days += days_month[i-1];
		if ((i == 2) && (this->year%4==0)){
			days ++;
		}
	}

	// Nombre de jours du mois courant
	days += this->day-1;

	// Conversion en secondes
	return days*86400 + this->hour*3600 + this->min*60 + this->sec + this->ms/1000.0;

}

// Conversion en heure sidérale (à greenwich)
double GPSTime::gast(){
    GPSTime ref(2000,01,01,12,00,00);
    double TSL0 = (*this - ref)*Utils::DJS + Utils::TSREF;
    double factor = floor(TSL0/24);
    TSL0 = TSL0 - factor*24;
    return TSL0;
}

// Conversion to string
std::string GPSTime::to_string() {
    std::string output = "";
	output += Utils::formatNumber(this->day,"%02d");
	output += "/" + Utils::formatNumber(this->month,"%02d");
	output += "/" + Utils::formatNumber(this->year,"%04d") + " ";
	output += Utils::formatNumber(this->hour,"%02d") + ":";
	output += Utils::formatNumber(this->min,"%02d") + ":";
	output += Utils::formatNumber(this->sec,"%02d");
	return output;
}

// Conversion to string
std::string GPSTime::to_formal_string() {
    std::string output = "";
	output += Utils::formatNumber(this->day,"%02d");
	output += Utils::formatNumber(this->month,"%02d");
	output += Utils::formatNumber(this->year,"%04d");
	output += Utils::formatNumber(this->hour,"%02d");
	output += Utils::formatNumber(this->min,"%02d");
	output += Utils::formatNumber(this->sec,"%02d");
	output += "." + Utils::formatNumber(this->ms,"%03d");
	return output;
}

// Conversion to string with ms
std::string GPSTime::to_complete_string() {
    std::string output = "";
	output += Utils::formatNumber(this->day,"%02d");
	output += "/" + Utils::formatNumber(this->month,"%02d");
	output += "/" + Utils::formatNumber(this->year,"%04d") + " ";
	output += Utils::formatNumber(this->hour,"%02d") + ":";
	output += Utils::formatNumber(this->min,"%02d") + ":";
	output += Utils::formatNumber(this->sec,"%02d") + ".";
	output += Utils::formatNumber(this->ms,"%03d");
	return output;
}

#ifndef AIRPORT
#define AIRPORT

#include <list>
#include "Coordinate.h"
#include "FlyObject.h"

class Airport
{
private:
	Coordinate position;
	char* name;
	unsigned char drones_places;
	unsigned char helicopters_places;
	unsigned char airplanes_places;
	unsigned char current_drones;
	unsigned char current_helicopters;
	unsigned char current_airplanes;
	std::list<FlyingObject*> list_fo;
public:
	Airport();
	~Airport();
	Airport(char* new_n, Coordinate pos, unsigned char dp, unsigned char hp, unsigned char ap,
		unsigned char cd, unsigned char ch, unsigned char ca);
	void landing_fo(FlyingObject* fo, char type);
	void send_fo(FlyingObject* fo, char type);
};
#endif //AIRPORT

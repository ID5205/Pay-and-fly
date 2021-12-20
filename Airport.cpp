#include <list>
#include <iterator>

#include "Airport.h"
#include "Coordinate.h"
#include "FlyObject.h"

Airport::Airport(){};
Airport::~Airport(){};
Airport::Airport(char* new_n, Coordinate pos, unsigned char dp, unsigned char hp, unsigned char ap,
	unsigned char cd, unsigned char ch, unsigned char ca)
{
    name = new char[size_t(new_n)];
    strcpy(name, new_n);
    position = pos;
    drones_places = dp;
	helicopters_places = hp;
	airplanes_places = ap;
	current_drones = cd;
	current_helicopters = ch;
	current_airplanes = ca;
}
void Airport::landing_fo(FlyingObject* fo, char type)
{
    switch(type)
    {
        case 0: current_airplanes--; break;
        case 1: current_helicopters--; break;
        case 2: current_drones--; break;
    }
    list_fo.push_back(fo);
}
void Airport::send_fo(FlyingObject* fo, char type)
{
    printf("%f %f %f %hhu %hhu %hhu %hhu %hhu %hhu %s\n", position.get_x(), position.get_y(), position.get_z(),
            drones_places,helicopters_places, airplanes_places, current_drones, current_helicopters, current_airplanes, name);
    /*std::list <FlyingObject*> :: iterator cur = list_fo.begin();
    while(cur != list_fo.end())
    {
        (*cur)->tell_about_yourself();
        cur++;
    }*/
}

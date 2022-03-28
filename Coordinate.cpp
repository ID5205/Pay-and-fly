#include "coordinate.h"

Coordinate::Coordinate()
{
    x = 0;
    y = 0;
    z = 0;
}
Coordinate::Coordinate(float new_x, float new_y, float new_z)
{
    x = new_x;
    y = new_y;
    z = new_z;
}
void Coordinate::set_xyz(float new_x, float new_y, float new_z)
{
    x = new_x;
    y = new_y;
    z = new_z;
}
void Coordinate::add_xyz(float d_x, float d_y, float d_z)
{
    x += d_x;
    y += d_y;
    z += d_z;
}
float Coordinate::get_x() {return x;}
float Coordinate::get_y() {return y;}
float Coordinate::get_z() {return z;}

bool Coordinate::operator== (Coordinate coor){
    if(x == coor.x && y == coor.y && z == coor.z)
        return 1;
    return 0;
}
bool Coordinate::operator!= (Coordinate coor){
    if(*this == coor)
        return 0;
    return 1;
}

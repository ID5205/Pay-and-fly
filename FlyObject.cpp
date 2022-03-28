#include "FlyObject.h"
#include "coordinate.h"
#include "Request.h"
#include "Product.h"
#include "navigator.h"
#include <cstdio>
#include <QTextStream>
#include <cstring>

FlyingObject::FlyingObject(){}
FlyingObject::~FlyingObject(){delete [] name;}
FlyingObject::FlyingObject(char* new_n, Coordinate cur_pos, Product load, float new_h, float new_r, float c_r, float new_fr)
{
    name = new char[strlen(new_n)+1];
    strcpy(name, new_n);
	position = cur_pos;
	speed_x = speed_y = speed_z = 0;
	max_load = load;
    current_orders = 0;
	max_height = new_h;
	max_range = new_r;
	current_fuel = c_r;
	fuel_rate = new_fr;
	ready_to_fly = 1;
}
void FlyingObject::Set_request(Request* new_req, navigator& _t)
{
    current_orders = new_req;
    track = _t;
}
void FlyingObject::Set_track(navigator& _t)
{
    track = _t;
}
void FlyingObject::Set_request(Request* new_req)
{
    current_orders = new_req;
}
float FlyingObject::get_max_h(){return max_height;}
Product FlyingObject::get_max_load(){return max_load;}
Request* FlyingObject::get_cur_req(){return current_orders;}
char FlyingObject::get_state(){return ready_to_fly;}
void FlyingObject::move_fo()
{
	position.add_xyz(speed_x, speed_y, speed_z);
}
void FlyingObject::change_move(float x, float y, float z)
{
    speed_x += x;
    speed_y += y;
    speed_z += z;
}
void FlyingObject::change_rtof(char rtof){ready_to_fly = rtof;}
navigator* FlyingObject::get_track(){return &track;}

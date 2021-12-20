#include "FlyObject.h"
#include "Coordinate.h"
#include "Request.h"
#include "Product.h"
#include <cstdio>
#include <iostream>
#include <cstring>

FlyingObject::FlyingObject(){}
FlyingObject::~FlyingObject(){delete(name);}
FlyingObject::FlyingObject(char* new_n, Coordinate cur_pos, Product load, float new_h, float new_r, float new_fr)
{

    name = new char[size_t(new_n)];
    strcpy(name, new_n);
	position = cur_pos;
	speed_x = speed_y = speed_z = 0;
	max_load = load;
	current_orders = 0;
	max_height = new_h;
	max_renge = new_r;
	current_fuel = 1;
	fuel_rate = new_fr;
	ready_to_fly = 1;
}
void FlyingObject::Set_request(Request* new_req){current_orders = new_req;}
float FlyingObject::get_max_h(){return max_height;}
Product FlyingObject::get_max_load(){return max_load;}
Request* FlyingObject::get_cur_req(){return current_orders;}
char FlyingObject::get_state(){return ready_to_fly;}
void FlyingObject::mouve()
{
	position.add_xyz(speed_x, speed_y, speed_z);
}
void FlyingObject::tell_about_yourself()
{
    std::cout<<"Name: " << name << std::endl;
    std::cout<<"Request: "<< std::endl;
    if(current_orders !=0)
    {
        std::cout<<"\t From: "<< current_orders->get_start() <<std::endl;
        std::cout<<"\t To: "<< current_orders->get_finish() <<std::endl;
    } else
    {
        std::cout<<"Empty"<< std::endl;
    }
    std::cout<<"Description:"<< std::endl;
    std::cout<<"\t position: "<< position.get_x() <<", "<<  position.get_y() <<", "<< position.get_z() <<std::endl;
    std::cout<<"\t max Load: "<< max_load.weight << "kg" <<std::endl;
    std::cout<<"\t max Height: "<< max_height << "m" <<std::endl;
    std::cout<<"\t max Renge: "<< max_renge << "km" <<std::endl;
}

#include <list>
#include <iostream>
#include <stdio.h>
#include "FlyObject.h"
#include "Airport.h"
#include "Request.h"
#include "Manager.h"
#include "Coordinate.h"

void Manager::a(){(*all_object.begin())->tell_about_yourself();}
Manager::Manager()
{
    FILE* input;
    //Загрузка заказов
    {
        float he, wi, le, we;
        unsigned int nu;
        char start[20] = {0};
        char finish[20] = {0};
        input = fopen("requests.txt", "r");
        while(fscanf(input, "%u %f %f %f %f %s %s\n",&nu, &he, &wi, &le, &we, &start, &finish)>0)
        {

            Product buf(he, wi, le, we);
            Request* req_buf = new Request(nu, buf, start, finish);
            printf("%u %f %f %f %f %s %s\n",req_buf->number,
                                                     req_buf->product.height,
                                                     req_buf->product.width,
                                                     req_buf->product.length,
                                                     req_buf->product.weight,
                                                     req_buf->start_name,
                                                     req_buf->finish_name);
            requests.push_back(req_buf);
        }
        fclose(input);
        printf("Download requets complite!\n");
    }
    //Загрузка самолетов
    {
        char new_n[20] = {0};
        unsigned int num_req;
        Request* cur_req = 0;
        float x, y, z, sx, sy, sz, he, wi, le, we, max_h, max_r, cur_f, f_rate;
        char rtof;

        input = fopen("flyobjects.txt", "r");
        while(fscanf(input, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %i %u %s\n",
                    &x, &y, &z,
                    &sx, &sy, &sz,
                    &he, &wi, &le, &we,
                    &max_h, &max_r, &cur_f, &f_rate,
                    &rtof,
                    &num_req,
                    &new_n) > 0)
        {
            Coordinate pos(x,y,z);
            Product max_l(max_h, max_r, cur_f, f_rate);
            FlyingObject* fo_buf = new FlyingObject(new_n, pos, max_l, max_h, max_r, cur_f, f_rate);
            fo_buf->change_move(sx, sy, sz);
            fo_buf->change_rtof(rtof);
            if(num_req != 0)
            {
                std::list<Request*> :: iterator cur = requests.begin();
                while(cur != requests.end())
                {
                   if((*cur)->number == num_req)
                   {
                       fo_buf->Set_request(*cur);
                       break;
                   }
                    cur++;
                }
            }
        all_object.push_back(fo_buf);
        }
        fclose(input);
        printf("Download flying objects complite!\n");
    }
    //Загрузка аэропортов
    {
        char new_n[20] = {0};
        Coordinate pos;
        float x, y, z;
        unsigned char dp, hp, ap, cd, ch, ca;
        input = fopen("airports.txt", "r");
        while(fscanf(input,"%f %f %f %hhu %hhu %hhu %hhu %hhu %hhu %s\n", &x, &y, &z, &dp, &hp, &ap, &cd, &ch, &ca, &new_n)>0)
        {
            pos.set_xyz(x,y,z);
            Airport* air_buf = new Airport(new_n, pos, dp, hp, ap, cd, ch, ca);
            airports.push_back(air_buf);
        }
        fclose(input);
        printf("Download airports complite!\n");
    }
}
Manager::~Manager()
{
    FILE* output;
    //Выгрузка заказов
    {
        output = fopen("requests.txt", "w");
        std::list<Request*> :: iterator cur = requests.begin();
        while(cur != requests.end())
        {
            fprintf(output, "%u %f %f %f %f %s %s\n",(*cur)->number,
                                                     (*cur)->product.height,
                                                     (*cur)->product.width,
                                                     (*cur)->product.length,
                                                     (*cur)->product.weight,
                                                     (*cur)->start_name,
                                                     (*cur)->finish_name);
            cur++;
            delete *requests.begin();
            requests.pop_front();
        }
        fclose(output);
        printf("Upload requests complite!\n");
    }
    //Выгрузка самолетов
    {
        output = fopen("flyobjects.txt", "w");
        std::list<FlyingObject*> :: iterator cur = all_object.begin();
        while(cur != all_object.end())
        {
            fprintf(output, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %i %u %s\n",
                    (*cur)->position.get_x(), (*cur)->position.get_y(), (*cur)->position.get_z(),
                    (*cur)->speed_x, (*cur)->speed_y, (*cur)->speed_z,
                    (*cur)->max_load.height, (*cur)->max_load.width, (*cur)->max_load.length, (*cur)->max_load.weight,
                    (*cur)->max_height, (*cur)->max_range, (*cur)->current_fuel, (*cur)->fuel_rate,
                    (*cur)->ready_to_fly,
                    (*cur)->current_orders != 0 ? (*cur)->current_orders->number : 0,
                    (*cur)->name);
            cur++;
            delete *all_object.begin();
            all_object.pop_front();
        }
        fclose(output);
        printf("Upload flying objects complite!\n");
    }
    //Выгрузка аэропортов
    {
        output = fopen("airports.txt", "w");
        std::list<Airport*> :: iterator cur = airports.begin();
        while(cur != airports.end())
        {
            fprintf(output, "%f %f %f %hhu %hhu %hhu %hhu %hhu %hhu %s\n",(*cur)->position.get_x(),
                                                     (*cur)->position.get_y(),
                                                     (*cur)->position.get_z(),
                                                     (*cur)->drones_places,
                                                     (*cur)->helicopters_places,
                                                     (*cur)->airplanes_places,
                                                     (*cur)->current_drones,
                                                     (*cur)->current_helicopters,
                                                     (*cur)->current_airplanes,
                                                     (*cur)->name);
            cur++;
            delete *airports.begin();
            airports.pop_front();
        }
        fclose(output);
        printf("Upload airports complite!\n");
    }
}
void Manager::new_request(Request* new_req)
{
    requests.push_back(new_req);
};
void Manager::new_fo(FlyingObject* new_fo)
{
    all_object.push_back(new_fo);
}
void Manager::send_to_order(){};

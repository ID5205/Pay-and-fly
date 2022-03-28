#ifndef FLY_OBJECT
#define FLY_OBJECT

#include "coordinate.h"
#include "Request.h"
#include "Product.h"
#include "navigator.h"
class FlyingObject
{
private:
    char* name;
	Coordinate position;
    float speed_x;
    float speed_y;
    float speed_z;
    Product  max_load;
    Request* current_orders;
    float max_height;
    float max_range;
    float current_fuel;
    float fuel_rate;
    char ready_to_fly;
    friend class Manager;
    navigator track;
    void change_move(float x, float y, float z);
    void change_rtof(char rtof);
public:
    FlyingObject();
    ~FlyingObject();
    FlyingObject(char* new_n, Coordinate cur_pos, Product load, float new_h, float new_r, float c_f, float new_fr);
    void Set_request(Request* new_req, navigator& _t);
    void Set_request(Request* new_req);
    void Set_track(navigator& _t);
    float get_max_h();
    Product get_max_load();
    Request* get_cur_req();
    char get_state();
    void move_fo();
    navigator* get_track();
};

#endif //FLY_OBJECT

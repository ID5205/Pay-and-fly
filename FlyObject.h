#ifndef FLY_OBJECT
#define FLY_OBJECT
#define TEST

#include "Coordinate.h"
#include "Request.h"
#include "Product.h"
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
    float max_renge;
    float current_fuel;
    float fuel_rate;
    char ready_to_fly;
public:

#ifdef TEST
    void tell_about_yourself();
#endif // TEST
    FlyingObject();
    ~FlyingObject();
    FlyingObject(char* new_n, Coordinate cur_pos, Product load, float new_h, float new_r, float new_fr);
    void Set_request(Request* new_req);
    float get_max_h();
    Product get_max_load();
    Request* get_cur_req();
    char get_state();
    void mouve();

};

#endif //FLY_OBJECT

#ifndef COORDINATE_H
#define COORDINATE_H

class navigator;
class Coordinate
{
    float x;
    float y;
    float z;
public:
    Coordinate();
    Coordinate(float new_x, float new_y, float new_z);
    void set_xyz(float new_x, float new_y, float new_z);
    void add_xyz(float d_x, float d_y, float d_z);
    float get_x();
    float get_y();
    float get_z();
    bool operator== (Coordinate coor);
    bool operator!= (Coordinate coor);
    friend navigator;
};

#endif // COORDINATE_H

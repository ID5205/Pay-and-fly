#ifndef NAVIGATOR_H
#define NAVIGATOR_H

class FlyingObject;
class Manager;
class Coordinate;
class map_painter;
class map;

class navigator
{
private:
    typedef struct
    {
        Coordinate *points;
        int n;
    } wave;
    Coordinate *track;
    unsigned int n;

    void new_wave(wave&);
    void wave_swap(wave*&, wave*&);
    void wave_add(wave& ,const Coordinate);
    void wave_clear(wave&);

    void make_track(Coordinate, Coordinate, map&, FlyingObject*);
    bool set_len_around(Coordinate, int** , map&, FlyingObject*, wave&);
public:
    navigator();
    navigator(const navigator&);
    navigator(Coordinate, Coordinate, map&, FlyingObject*);
    navigator(unsigned int, Coordinate*);
    ~navigator();
    float length();
    friend FlyingObject;
    friend Manager;
    friend map_painter;
    navigator &operator= (const navigator&);
    navigator operator+ (const navigator&);
};

#endif // NAVIGATOR_H

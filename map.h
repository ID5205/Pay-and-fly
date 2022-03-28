#ifndef MAP_H
#define MAP_H

class Coordinate;
class navigator;
class map_painter;
class Manager;

class map
{
private:
    Coordinate** matrix;
    int weidth, lendth;
public:
    map();
    void load_map(const char*);
    friend Manager;
    friend navigator;
    friend map_painter;
};

#endif // MAP_H

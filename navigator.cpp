#include "navigator.h"
#include "coordinate.h"
#include "FlyObject.h"
#include "map.h"

#include <cmath>
navigator::navigator()
{
    track = 0;
    n = 0;
}
navigator::navigator(unsigned int _n, Coordinate* a)
{
    n = _n;
    track = new Coordinate[n];
    for(unsigned int i = 0; i < n; i++)
        track[i] = a[i];
}
navigator::navigator(Coordinate a, Coordinate b, map &map, FlyingObject* fo)
{
    make_track(a, b, map, fo);
}
navigator::~navigator()
{
    delete [] track;
}
float navigator::length()
{
    float summ = 0;
    for(int i = 0; i < n-1; i++)
        summ+= sqrt((track[i].get_y()-track[i+1].get_y())*
                    (track[i].get_y()-track[i+1].get_y())+
                    (track[i].get_y()-track[i+1].get_y())*
                    (track[i].get_y()-track[i+1].get_y()));
    return summ;
}
navigator::navigator(const navigator& a)
{
    n = a.n;
    if(!track)
        delete [] track;
    track = new Coordinate[n];
    for(unsigned int i = 0; i < n; i++)
        track[i] = a.track[i];
}
navigator& navigator::operator= (const navigator& a)
{
    n = a.n;
    if(!track)
        delete [] track;
    track = new Coordinate[n];
    for(unsigned int i = 0; i < n; i++)
        track[i] = a.track[i];
    return *this;
}
navigator navigator::operator+ (const navigator& a)
{
    unsigned int dn = n + a.n-1;
    Coordinate* dt = new Coordinate[dn];
    unsigned int i;
    for(i = 0; i < n-1; i++)
        dt[i] = track[i];
    for(i = 0; i < a.n; i++)
        dt[i+n-1] = a.track[i];
    navigator buf(dn, dt);
    delete [] dt;
    return buf;
}

void navigator::wave_add(wave& w,const Coordinate p)
{
    w.n++;
    w.points = (Coordinate*)realloc(w.points, sizeof(Coordinate)*w.n);
    w.points[w.n-1] = p;
}
void navigator::new_wave(wave& w)
{
    w.n = 0;
    w.points = 0;
}
void navigator::wave_swap(wave* &w1, wave* &w2)
{
    void* buf = w1;
    w1 = w2;
    w2 = (wave*)buf;
}
void navigator::wave_clear(wave &w)
{
    free(w.points);
    w.points = 0;
    w.n = 0;
}
void navigator::make_track(Coordinate a, Coordinate b, map &map, FlyingObject *fo)
{
    #define L 20
    /// одна клетка подразумеватся, как 20 единиц
    /// создание массива, в котором будет отмечатся расстояние
    int** step_map = new int*[map.lendth];
    for(int i = 0; i < map.lendth; i++)
    {
        step_map[i] = new int[map.weidth];
        for(int j = 0; j < map.weidth; j++)
            step_map[i][j] = -1;
    }
    /// создание точки конца маршрута
    step_map[(int)b.x/L+map.lendth/2][map.weidth/2-(int)b.y/L] = -2;

    /// создание точки отсчета
    step_map[(int)a.x/L+map.lendth/2][map.weidth/2-(int)a.y/L] = 0;

    wave buf_wave1, buf_wave2;
    new_wave(buf_wave1); new_wave(buf_wave2);
    wave* w1 = &buf_wave1;
    wave* w2 = &buf_wave2;
    Coordinate step_coord_start((int)a.x/L+map.lendth/2, map.weidth/2-a.y/L, 0);
    wave_add(*w1, step_coord_start);

    /// пока не найдем точку конца маршрута
    bool end = 0;
    while(!end)
    {
        for(int i = 0; i < w1->n && !end; i++)
            end = set_len_around(w1->points[i], step_map, map, fo, *w2);
        wave_clear(*w1);
        wave_swap(w1, w2);
    }
    wave_clear(*w1);
    wave_clear(*w2);
    /// строим маршрут

    track = 0;
    n = 0;
    /// записываем в маршрут точку конца
    /// использую волну, потому что у нее есть функция добавления элемента,
    /// она использует realloc, и чиститься free, а navigator использует new и чиститься delete
    /// маршрут будет записан задом на перед, поэтмоу из волны в track перепишу в правильном порядке
    Coordinate buf((int)b.x/L+map.lendth/2,map.weidth/2-(int)b.y/L,0);
    wave_add(*w1, buf);
    int min = 999999999;
    while(min != 0)
    {
        int _i = 0, _j = 0; ///для записи приращения до минимальной точки
        for(int i = -1; i < 2; i++)
            for(int j = -1; j < 2; j++)
                ///находим минимальное значение вокруг клетки
                ///это означает кратчайшее направление от конца к началу
                if(step_map[(int)buf.x+i][(int)buf.y+j] >=0)
                    if(step_map[(int)buf.x+i][(int)buf.y+j] < min)
                    {
                        min = step_map[(int)buf.x+i][(int)buf.y+j];
                        _i = i;
                        _j = j;
                    }
        buf.add_xyz(_i, _j, 0);
        ///записываем точку в волну
        wave_add(*w1, buf);
    }
    /// переписываем из волны в track в правильном порядке
    track = new Coordinate [w1->n];
    n = w1->n;
    for(int i = 0; i < w1->n; i++)
    {
        float x = (w1->points[n-i-1].x-map.lendth/2)*L;
        float y = (map.weidth/2-w1->points[n-i-1].y)*L;
        Coordinate buf(x, y, 0);
        track[i] = buf;
    }
    wave_clear(*w1);
    ///освобождение памяти
    for(int i = 0; i < map.lendth; i++)
        delete [] step_map[i];
    delete [] step_map;
    #undef L
}
bool navigator::set_len_around(Coordinate a, int** s_m,
                               map &map, FlyingObject* fo, wave& w)
{
    for(int i = -1; i < 2; i++)
        for(int j = -1; j < 2; j++)
            /// если координата в пределах карты
            /// и если высота клетки на 200 больше, чем может пролететь fo
            /// и она не текущая
            if((j !=0 || i != 0)
               && a.x+i < map.lendth && a.y+j < map.lendth
               && a.x+i >= 0 && a.y+j >=0
               && map.matrix[(int)a.y+j][(int)a.x+i].get_z()+200 < fo->get_max_h())
            {
                int len;
                if(i && j)/// если до клетки диагональ
                    len = s_m[(int)a.x][(int)a.y] + 28;
                else
                    len = s_m[(int)a.x][(int)a.y] + 20;
                /// если найден конечный пункт возвращаем 1
                if(s_m[(int)a.x+i][(int)a.y+j] == -2)
                    return 1;
                /// если в клетке нет значения
                /// или новый путь меньше (более оптимальный путь)
                /// то меняем
                if(s_m[(int)a.x+i][(int)a.y+j] == -1
                   || s_m[(int)a.x+i][(int)a.y+j] > len)
                {
                    s_m[(int)a.x+i][(int)a.y+j] = len;
                    Coordinate b = a;
                    b.add_xyz(i,j,0);
                    wave_add(w, b);
                }
            }
    return 0;
}

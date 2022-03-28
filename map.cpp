#include "map.h"
#include "coordinate.h"

#include <QTextStream>
#include <QFile>

map::map()
{
    lendth = 1;
    weidth = 1;
    matrix = 0;
}
void map::load_map(const char* name)
{
    QFile file(name);
    file.open(QIODevice::ReadOnly);
    QTextStream input(&file);
    input >> weidth >> lendth;
    double h;
    if(weidth && lendth)
    {
        matrix = new Coordinate*[weidth];
        for(int i = 0; i < weidth; i++)
        {
            matrix[i] = new Coordinate[lendth];
            for(int j = 0; j < lendth; j++)
            {
                input >> h;
                matrix[i][j].set_xyz(j, i, h);
            }
        }
    }
    file.close();
}

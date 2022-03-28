#ifndef MAP_PAINTER_H
#define MAP_PAINTER_H

#include <QOpenGLWidget>

class Manager;
class navigator;

class map_painter  : public QOpenGLWidget
{
protected:
     void initializeGL( ) ;
     void resizeGL (int nWidth, int nHeight);

     Manager* m;
     float scale;
     float center_x;
     float center_y;
     navigator* cur_track;
public:
    void paintGL ();
    map_painter(QWidget* pwgt, Manager*);
    void set_track(navigator*);
};

#endif // MAP_PAINTER_H*/


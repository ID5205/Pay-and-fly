#include "map_painter.h"
#include "Airport.h"
#include "Manager.h"
#include "navigator.h"

#include <QOpenGLWidget>

map_painter::map_painter(QWidget* pwgt, Manager* manager):QOpenGLWidget(pwgt)
{
    m = manager;
    scale = 0.9;
    center_y = center_x = 0;
    cur_track = 0;
}
void map_painter::set_track(navigator* ct)
{
    cur_track = ct;
}
void map_painter::initializeGL()
{
   glClearColor(0.59, 0.66, 0.4, 1.0);
   glEnable(GL_POINT_SMOOTH);
   glEnable(GL_LINE_SMOOTH);
   glPointSize(15);
   glLineWidth(5);
}
void map_painter::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-nWidth/2., nWidth/2., -nHeight/2., nHeight/2., -1.0, 1.0);
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void square(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h)
{
    LOGFONT lf;
    if(z < 101)
        glColor3f(0.52, 0.69, 0.43);
    else if(z < 410)
        glColor3f(0.59, 0.66, 0.4);
    else if(z < 650)
        glColor3f(0.68, 0.72, 0.49);
    else if(z < 760)
        glColor3f(0.83, 0.71, 0.49);
    else if(z < 941)
        glColor3f(0.8, 0.59, 0.27);
    else if(z < 1170)
        glColor3f(0.81, 0.56, 0.33);
    else if(z < 1450)
        glColor3f(0.8, 0.51, 0.24);
    else if(z < 1801)
        glColor3f(0.78, 0.45, 0.43);
    else if(z < 2100)
        glColor3f(0.82, 0.38, 0.37);
    else
        glColor3f(0.8, 0.25, 0.26);

    glBegin(GL_TRIANGLE_STRIP);
        glVertex2d(x,y);
        glVertex2d(x+w,y);
        glVertex2d(x,y+h);
        glVertex2d(x+w,y+h);
    glEnd();
}
void map_painter::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glScalef(scale, scale, scale);
        for(int i = 0; i < m->map.weidth; i++)
           for(int j = 0; j < m->map.lendth; j++)
               square(center_x+m->map.matrix[i][j].get_x()*20-m->map.lendth*20/2.,
                      center_y-m->map.matrix[i][j].get_y()*20+m->map.weidth*20/2.,
                      m->map.matrix[i][j].get_z(), 20, 20);

    QList<Airport*> :: iterator cur = m->airports.begin();
    QList<FlyingObject*> :: iterator f_cur = m->used_objects.begin();
    while(cur != m->airports.end())
    {
        glColor3f(0.2,0.8,0.3);
        Coordinate coord = (*cur)->position;
        glBegin(GL_POINTS);
            glVertex2d(coord.get_x(), coord.get_y());
        glEnd();
        cur++;
    }
    /*отрисовка всех маршрутов*/
    while(f_cur != m->used_objects.end())
    {
        glLineWidth(1);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0xf000);
        glColor3f(0.2,0.5,0.1);
        glBegin(GL_LINE_STRIP);
            for(int i = 0; i < (*f_cur)->get_track()->n; i++)
                glVertex2f((*f_cur)->get_track()->track[i].get_x(), (*f_cur)->get_track()->track[i].get_y());
            f_cur++;
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }
    /*выделенный маршрут*/
    if(cur_track)
    {
        glLineWidth(3);
        glColor3f(0.05, 0.13, 0.94);
        glBegin(GL_LINE_STRIP);
            for(int i = 0; i < cur_track->n; i++)
                glVertex2f(cur_track->track[i].get_x(), cur_track->track[i].get_y());

        glEnd();
    }
    glPopMatrix();
    for(int i = 0; i < 10; i++)
    {
        square(-650, 470-i*30, (i < 8?100:250)+210*i, 80, 30);
    }
    glColor3f(0,0,0);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
        glVertex2d(-650,500);
        glVertex2d(-650,200);
        glVertex2d(-570,200);
        glVertex2d(-570,500);
    glEnd();
    update();
}

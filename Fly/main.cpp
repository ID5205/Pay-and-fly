#include "mainwindow.h"
#include "Product.h"
#include "FlyObject.h"
#include "Request.h"
#include "Airport.h"
#include "Manager.h"
#include "map_painter.h"

#include <cstdio>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Manager manager;
    map_painter painter(w.get_GL(), &manager);

    w.set_menager(&manager);
    w.set_painter(&painter);

    painter.resize(w.get_GL()->width(), w.get_GL()->height());
    w.show();
        manager.parse_requests();
        painter.show();
        manager.fill_table(w);
    return a.exec();
}


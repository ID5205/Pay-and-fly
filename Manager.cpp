#include "FlyObject.h"
#include "Airport.h"
#include "Request.h"
#include "Manager.h"
#include "navigator.h"
#include "map.h"

#include <QFile>
#include <QTextStream>
#include <QList>
#include <cmath>            //для рассчета расстояния
//активно кодю
//еще активней кодю
Manager::Manager()
{
    map.load_map("map.txt");
    QTextStream out(stdout);
    int num_found;
    //Загрузка заказов
    {

        num_found = 0;
        QFile file1("requests.txt");
        QFile file2("executable_requests.txt");
        file1.open(QIODevice::ReadOnly);

        float he, wi, le, we;   //размер заказа
        unsigned int nu;        //номер заказа
        char start[20] = {0};   //имя начальной точки
        char finish[20] = {0};  //имя конечной точки

        QTextStream input(&file1);
        while(!input.atEnd())
        {
            num_found++;
            input >> nu >> he >> wi >> le >> we >> start >> finish;
            Product buf(he, wi, le, we);
            Request* req_buf = new Request(nu, buf, start, finish);
            requests.push_back(req_buf);
        }
        file1.close();
        file2.open(QIODevice::ReadOnly);
        input.setDevice(&file2);
        while(!input.atEnd())
        {
            num_found++;
            input >> nu >> he >> wi >> le >> we >> start >> finish;
            Product buf(he, wi, le, we);
            Request* req_buf = new Request(nu, buf, start, finish);
            executable_requests.push_back(req_buf);
        }
        file2.close();
    }
    //Загрузка самолетов
    {
        num_found = 0;
        QFile file("flyobjects.txt");
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        char new_n[20] = {0};
        unsigned int num_req;
        float x, y, z, sx, sy, sz, he, wi, le, we, max_h, max_r, cur_f, f_rate;
        char rtof;

        QTextStream input(&file);
        while(!input.atEnd())
        {
            num_found++;
            input >> x >> y >> z >> sx >> sy >> sz
                  >> he >> wi >> le >> we
                  >> max_h >> max_r >> cur_f >> f_rate >> rtof
                  >> rtof >> num_req >> new_n;
            Coordinate pos(x,y,z);
            Product max_l(he, wi, le, we);
            FlyingObject* fo_buf = new FlyingObject(new_n, pos, max_l, max_h, max_r, cur_f, f_rate);
            fo_buf->change_move(sx, sy, sz);
            fo_buf->change_rtof(rtof);
            if(num_req != 0)
            {
                QList<Request*> :: iterator cur = executable_requests.begin();
                while(cur != executable_requests.end())
                {
                   if((*cur)->number == num_req)
                   {
                       fo_buf->Set_request(*cur);
                       break;
                   }
                    cur++;
                }
                used_objects.push_back(fo_buf);
            }
            else
                free_objects.push_back(fo_buf);
        }
        file.close();
        //загрузка маршрутов
        {
            QFile file_track("track_points.txt");
            file_track.open(QIODevice::ReadOnly | QIODevice::Text);
            input.setDevice(&file_track);

            unsigned int n, num;
            float x, y, z;
            Coordinate* points = 0;

            while(!input.atEnd())
            {
                input >> num >> n;
                points = new Coordinate[n];
                for(int i = 0; i < n; i++)
                {
                    input >> x >> y >> z;
                    points[i].set_xyz(x,y,z);
                }
                navigator track(n, points);
                QList<FlyingObject*>::iterator cur = used_objects.begin();
                while(cur != used_objects.end())
                {
                    if((*cur)->current_orders->number == num)
                    {
                        (*cur)->Set_track(track);
                        break;
                    }
                    cur++;
                }
                delete [] points;
            }
            file_track.close();
        }
    }
    //Загрузка аэропортов
    {
        num_found = 0;
        QFile file("airports.txt");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        char new_n[20] = {0};
        Coordinate pos;
        float x, y, z;
        unsigned int dp, hp, ap, cd, ch, ca;

        QTextStream input(&file);
        QList<FlyingObject*> :: iterator cur = 0;
        while(!input.atEnd())
        {
            num_found++;
            input >> x >> y >> z >> dp >> hp >> ap >> cd >> ch >> ca >> new_n;
            pos.set_xyz(x,y,z);
            Airport* air_buf = new Airport(new_n, pos, dp, hp, ap, cd, ch, ca);
            airports.push_back(air_buf);
            cur = free_objects.begin();
            while(cur != free_objects.end())
            {
                if((*cur)->position == air_buf->position)
                    air_buf->landing_fo(*cur, 0);
                cur++;
            }
        }
        file.close();
    }
}
Manager::~Manager()
{
    //Выгрузка аэропортов
    {
        QFile file("airports.txt");
        file.open(QIODevice::Truncate | QIODevice::WriteOnly);
        QTextStream output(&file);

        while(!airports.isEmpty())
        {
            output << Qt::endl
                   << airports.front()->position.get_x()    << ' '
                   << airports.front()->position.get_y()    << ' '
                   << airports.front()->position.get_z()    << ' '
                   << airports.front()->drones_places       << ' '
                   << airports.front()->helicopters_places  << ' '
                   << airports.front()->airplanes_places    << ' '
                   << airports.front()->current_drones      << ' '
                   << airports.front()->current_helicopters << ' '
                   << airports.front()->current_airplanes   << ' '
                   << airports.front()->name                ;
            delete airports.front();
            airports.pop_front();
        }
        file.close();
    }
    //Выгрузка самолетов
    {
        QFile file_fly("flyobjects.txt");
        QFile file_track("track_points.txt");

        char flag = 1;

        //Выгрузка маршрутов
        {
            QList<FlyingObject*>::iterator cur = used_objects.begin();
            file_track.open(QIODevice::Truncate | QIODevice::WriteOnly);
            QTextStream output(&file_track);
            while(cur != used_objects.end())
            {
                output << Qt::endl
                       << (*cur)->current_orders->number << ' '
                       << (*cur)->track.n;
                for(int i = 0; i < (*cur)->track.n; i++)
                    output << ' ' << (*cur)->track.track[i].get_x() << ' '
                           << (*cur)->track.track[i].get_y() << ' '
                           << (*cur)->track.track[i].get_z();
                cur++;
            }
            file_track.close();
        }
        file_fly.open(QIODevice::Truncate | QIODevice::WriteOnly);
        QTextStream output(&file_fly);
        QList<FlyingObject*> *a = &free_objects;
        while(!(free_objects.isEmpty() && used_objects.isEmpty()))
        {
            output << Qt::endl
                   << a->front()->position.get_x()  << ' '
                   << a->front()->position.get_y()  << ' '
                   << a->front()->position.get_z()  << ' '
                   << a->front()->speed_x           << ' '
                   << a->front()->speed_y           << ' '
                   << a->front()->speed_z           << ' '
                   << a->front()->max_load.height   << ' '
                   << a->front()->max_load.width    << ' '
                   << a->front()->max_load.length   << ' '
                   << a->front()->max_load.weight   << ' '
                   << a->front()->max_height        << ' '
                   << a->front()->max_range         << ' '
                   << a->front()->current_fuel      << ' '
                   << a->front()->fuel_rate         << ' '
                   << a->front()->ready_to_fly      << ' ';
            if(a->front()->current_orders != 0)
            {           //тут почему то не работал тернарный оператор ?, пришлось изгаляться
                output
                   << a->front()->current_orders->number << ' '
                   << a->front()->name;
            }
            else
            {
                output
                   << 0 << ' '
                   << a->front()->name;
            }
            delete a->front();
            a->pop_front();
            if(free_objects.isEmpty() && flag)
            {
                flag = 0;
                a = &used_objects;
            }
        }
        file_fly.close();
    }
    //Выгрузка заказов
    {
        QFile file1("requests.txt");                //фалй, в который будут записываться неисполняемые заказы
        QFile file2("executable_requests.txt");     //фалй, в который будут записываться исполняемые заказы
        file1.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
        QTextStream output(&file1);      //создать поток в файл
        QList<Request*> *a = &requests;
        char flag = 1;
        while(!(requests.isEmpty() && executable_requests.isEmpty()))      //пока в списке что-то есть
        {                               //записать
            output << Qt::endl
                   << a->front()->number          <<  ' '
                   << a->front()->product.height  <<  ' '
                   << a->front()->product.width   <<  ' '
                   << a->front()->product.length  <<  ' '
                   << a->front()->product.weight  <<  ' '
                   << a->front()->start_name      <<  ' '
                   << a->front()->finish_name     ;
            delete a->front();    //удалить объект
            a->pop_front();       //удалить из списка
            if(requests.isEmpty() && flag)
            {
                flag = 0;
                file1.close();
                a = &executable_requests;
                file2.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
                output.setDevice(&file2);
            }
        }
        file2.close();
    }
}
void Manager::new_request(Request* new_req)
{
    requests.push_back(new_req);
};
void Manager::new_fo(FlyingObject* new_fo)
{
    free_objects.push_back(new_fo);
}
void Manager::send_to_order(){}
void Manager::parse_requests()
{
    Request* flag = 0;
    while(!requests.isEmpty() && flag != requests.front())
    {
        Airport* start_ap = 0;
        Airport* end_ap = 0;
        //нахождение аэропортов старта и конца по имени
        {
            char find = 0;      //надо найти 2 эаропорта.
            QList<Airport*>::Iterator cur_ap = airports.begin();
            QList<Request*>::Iterator cur_re = requests.begin();
            while(find < 2)
            {
                if(strcmp((*cur_ap)->name, requests.front()->start_name) == 0)
                {   //если имя совпало названием аэропорта начала маршрута
                    start_ap = *cur_ap;
                    find++;
                }
                if(strcmp((*cur_ap)->name, requests.front()->finish_name) == 0)
                {   //если имя совпало названием аэропорта конца маршрута
                    end_ap = *cur_ap;
                    find++;
                }
                cur_ap++;
            }
            if(!start_ap && !end_ap)
                return;
        }
        navigator track;
        FlyingObject* courier = 0;
        QList<FlyingObject*>::Iterator cur = start_ap->list_fo.begin();
        QList<FlyingObject*> sutible_fo; // тут будут храниться подходящие самолеты
                                        //из которых потом выберут лучшие
        if(*cur)
        //поиск подходящих самолетов в текущем аэропорту
        {
            while(cur != start_ap->list_fo.end())           //поиск самолетов в стартовом аэропорту
            {
                if((*cur)->max_load >= requests.front()->product && //поиск по максимальной нагрузке
                  !(*cur)->current_orders)                          // и не занятости
                    sutible_fo.push_back((*cur));
                cur++;
            }
        }
        if(sutible_fo.isEmpty())
        //если в аэропорте старта не нашлось подходящих самолетов
            //ищем во всех аэропортах
        {
            QList<Airport*>::Iterator cur_ap = airports.begin();
            if(*cur)
                while(cur_ap != airports.end())
                {
                    cur = (*cur_ap)->list_fo.begin();
                    while(cur != (*cur_ap)->list_fo.end())           //поиск самолетов в аэропорту
                    {
                        if(*cur_ap != start_ap)
                            if((*cur)->max_load >= requests.front()->product && //поиск по максимальной нагрузке
                              !(*cur)->current_orders)                          // и не занятости
                            {
                                //рассчет длины маршрута перелета до базы старта
                                navigator pre_track((*cur_ap)->position, start_ap->position, map, *cur);
                                //если долетает -> добавляем
                                if((*cur)->max_range >= pre_track.length())
                                    sutible_fo.push_back((*cur));
                            }
                        cur++;
                    }
                    cur_ap++;
                }
        }
        if(sutible_fo.isEmpty())
            //если в аэропортах не нашлось подходящих или свободных самолетов, откладываем заказ
        {
            if (!flag)
            {
                flag = requests.front();
                requests.removeFirst();
                requests.push_back(flag);
            }
            else
            {
                requests.push_back(requests.front());
                requests.removeFirst();
            }
            continue;
        }

        cur = sutible_fo.begin();
        //поиск по дальности полета среди подходящих
        if(*cur)
            while(cur != sutible_fo.end())
            {
                track.make_track(start_ap->position, end_ap->position, map, *cur);
                if((*cur)->max_range >= track.length())//Расстояние между аэропортами
                {
                    courier = *cur;
                    break;
                }
                cur++;
            }
        if(courier)
        //перенос самолета из списка свободных в список занятых
        {
            free_objects.removeOne(courier);                //удалить самолет из свободных
            if(courier->position != start_ap->position)
            {
                navigator d_track(courier->position, start_ap->position, map, courier);
                track = d_track + track;
                courier->Set_request(requests.front(), track);
            }
            else
                courier->Set_request(requests.front(), track);  //дать ему задание
            executable_requests.push_back(requests.front());//перенести задание в список исполняемых
            requests.removeFirst();                         //удалить задание из списка базовых
            used_objects.push_back(courier);                //перенести самолет в список занятых
        }
        else if (!flag)
        {
            flag = requests.front();
            requests.removeFirst();
            requests.push_back(flag);
        }
        else
        {
            requests.push_back(requests.front());
            requests.removeFirst();
        }
    }
}

FlyingObject* Manager::fill_field(MainWindow& w, int n){
    if(used_objects.count() != 0)
    {
        QList<FlyingObject*>::iterator cur = used_objects.begin();
        for(int i = 0; i < n; i++)
            cur++;
        FlyingObject* a = *cur;
        w.get_list()->clear();
        w.get_list()->addItem(MainWindow::tr("Name: %1").arg(a->name));
        w.get_list()->addItem(MainWindow::tr("Requests:"));
        if(a->current_orders !=0)
        {
            w.get_list()->addItem(MainWindow::tr("\tNumber: %1").arg(a->current_orders->number));
            w.get_list()->addItem(MainWindow::tr("\tFrom: %1").arg(a->current_orders->start_name));
            w.get_list()->addItem(MainWindow::tr("\tTo: %1").arg(a->current_orders->finish_name));
        } else
        {
            w.get_list()->addItem("\tEmpty");
        }
        w.get_list()->addItem(MainWindow::tr("Description:"));
        w.get_list()->addItem(MainWindow::tr("\tposition (x, y, z): %1, %2, %3").arg(a->position.get_x()).
                                                                                   arg(a->position.get_y()).
                                                                                   arg(a->position.get_z()));
        w.get_list()->addItem(MainWindow::tr("\tmax Load: %1").arg(a->max_load.weight));
        w.get_list()->addItem(MainWindow::tr("\tmax Height: %1").arg(a->max_height));
        w.get_list()->addItem(MainWindow::tr("\tmax Renge: %1").arg(a->max_range));
        return a;
    }
    return 0;
}
void Manager::fill_table(MainWindow& w){
    QList<FlyingObject*>::iterator cur = used_objects.begin();
    w.get_table()->setRowCount(0);
    while(cur != used_objects.end())
    {
        w.get_table()->insertRow(w.get_table()->rowCount());
        w.get_table()->setItem(w.get_table()->rowCount()-1, 0,
                               new QTableWidgetItem(MainWindow::tr("%1").arg((*cur)->name)));
        w.get_table()->setItem(w.get_table()->rowCount()-1, 1,
                               new QTableWidgetItem(MainWindow::tr("%1 %2 %3").
                                        arg((*cur)->position.get_x()).
                                        arg((*cur)->position.get_y()).
                                        arg((*cur)->position.get_z())));
        w.get_table()->setItem(w.get_table()->rowCount()-1, 2,
                               new QTableWidgetItem(MainWindow::tr("%1").arg((*cur)->current_orders->number)));
        w.get_table()->setItem(w.get_table()->rowCount()-1, 3,
                               new QTableWidgetItem(MainWindow::tr("%1").arg((*cur)->ready_to_fly)));
        cur++;
    }
    for(int i=0; i< w.get_table()->rowCount(); i+=2)
        for (int j=0; j<w.get_table()->columnCount(); j++)
            w.get_table()->item(i, j)->setBackground(QColor(QString::fromUtf8("#383838")));
    w.get_table()->setStyleSheet(" QTableCornerButton::section {background-color:#4f4f4f; border:1px solid #383838;} QTableWidget::item{ selection-background-color: rgba(30, 82, 112, 0.4); color:white}");
}

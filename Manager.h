#ifndef MANAGER
#define MANAGER

#include <QList>
#include "FlyObject.h"
#include "Airport.h"
#include "Request.h"
#include "map.h"
#include <mainwindow.h>

class map_painter;
class Manager
{
private:
    QList<FlyingObject*> free_objects;
    QList<FlyingObject*> used_objects;
    QList<Airport*> airports;
    QList<Request*> requests;
    QList<Request*> executable_requests;
    map map;
public:
	Manager();
	~Manager();
	void new_request(Request*);
    void send_to_order();
	void parse_requests();
	void new_fo(FlyingObject*); 	//new FlyingObject
    void add_fo();
    void fill_table(MainWindow&);
    FlyingObject* fill_field(MainWindow&, int);
	void new_ap(); 	//Airport
	void add_ap(Request*);

    friend map_painter;
};

#endif //MANAGER

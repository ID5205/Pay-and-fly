#ifndef MANAGER
#define MANAGER

#include <list>
#include "FlyObject.h"
#include "Airport.h"
#include "Request.h"

class Manager
{
private:
	std::list<FlyingObject*> all_object;
	std::list<Airport*> airports;
	std::list<Request*> requests;
public:
	Manager();
	~Manager();
	void new_request(Request*);
	void send_to_order();
	void new_fo(FlyingObject*); 	//new FlyingObject
	void add_fo();
	void a();
	void new_ap(); 	//Airport
	void add_ap(Request*);
};

#endif //MANAGER

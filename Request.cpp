#include "Request.h"
#include "Product.h"
#include <cstring>

    Request::Request()
    {
        start_name = 0;
        finish_name = 0;
    }
	Request::Request(Product new_product, char* start, char* finish)
	{
	    start_name = new char[size_t(start)];
	    finish_name = new char[size_t(finish)];
		strcpy(start_name, start);
		strcpy(finish_name, finish);
		product = new_product;
	}
	Request::~Request()
	{
	    delete(start_name);
	    delete(finish_name);
	}
	char* Request::get_start(){return start_name;}
	char* Request::get_finish(){return finish_name;}

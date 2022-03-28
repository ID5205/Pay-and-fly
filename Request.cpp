#include "Request.h"
#include "Product.h"
#include <cstring>
#include <stdio.h>
///типа что-то изменил
    Request::Request()
    {
        start_name = 0;
        finish_name = 0;
    }
    Request::Request(unsigned int num, Product new_product, char* start, char* finish)
	{
        number = num;
        start_name = new char[strlen(start)+1];
        finish_name = new char[strlen(finish)+1];
        strcpy(start_name, start);
        strcpy(finish_name, finish);
		product = new_product;
	}
	Request::~Request()
    {
        delete[] finish_name;
        delete[] start_name;
	}
	char* Request::get_start(){return start_name;}
	char* Request::get_finish(){return finish_name;}
    unsigned int Request::get_num(){return number;}

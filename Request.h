#ifndef REQUEST
#define REQUEST

#include "Product.h"
#include <string.h>

class Request
{
	Product product;
    unsigned int number;
	char* start_name;    
	char* finish_name;   
	friend class Manager;
public:
    Request();
    ~Request();
    Request(unsigned int num, Product product, char* start, char* finish);
	char* get_start();
	char* get_finish();
    unsigned int get_num();
};

#endif //REQUEST

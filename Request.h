#ifndef REQUEST
#define REQUEST

#include "Product.h"
#include <string.h>

class Request
{
	Product product;
	char* start_name;
	char* finish_name;
public:
    Request();
    ~Request();
	Request(Product product, char* start, char* finish);
	char* get_start();
	char* get_finish();
};

#endif //REQUEST

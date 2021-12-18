#include "Product.h"

Product::Product()
{
	height = 0;
    width  = 0;
    length  = 0;
    weight = 0;
}
Product::Product(float new_height, float new_width, float new_length, float new_weight)
{
	height = new_height;
    width  = new_width;
    length  = new_length;
    weight = new_weight;
}

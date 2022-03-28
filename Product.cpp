#include "Product.h"

Product::Product()
{
	height = 0;
    width  = 0;
    length = 0;
    weight = 0;
}
Product::Product(float new_height, float new_width, float new_length, float new_weight)
{
	height = new_height;
    width  = new_width;
    length  = new_length;
    weight = new_weight;
}
bool Product::operator< (Product a)
{
    if(*this == a || *this > a)
        return 0;
    return 1;
}
bool Product::operator> (Product a)
{
    if(!(*this == a) &&
        height > a.height &&
        (width  > a.width || length > a.length) && //можно как обычно
        (length > a.width || width  > a.length) && //а можно повернуть груз
        weight > a.weight)
        return 1;
    return 0;
}
bool Product::operator>= (Product a)
{
    if(*this == a || *this > a)
        return 1;
    return 0;
}
bool Product::operator== (Product a)
{
    if(	height == a.height &&
        width  == a.width &&
        length == a.length &&
        weight == a.weight)
        return 1;
    return 0;
}

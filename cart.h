#ifndef CART_H
#define CART_H

struct ShoppingCart {
    int product_id;
    double price;
    int quantity;
};

#include <occi.h>
using namespace oracle::occi;

int addToCart(Connection* conn, ShoppingCart cart[]);
void displayProducts(ShoppingCart cart[], int count);

#endif // CART_H


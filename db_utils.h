#ifndef DB_UTILS_H
#define DB_UTILS_H

#include <occi.h>
#include "cart.h"  

int customerLogin(oracle::occi::Connection* conn, int customerId);
double findProduct(oracle::occi::Connection* conn, int product_id);
int checkout(oracle::occi::Connection* conn, ShoppingCart cart[], int customerId, int productCount);
void displayOrderStatus(oracle::occi::Connection* conn, int orderId, int customerId);
void cancelOrder(oracle::occi::Connection* conn, int orderId, int customerId);

#endif

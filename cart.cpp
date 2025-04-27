#include "cart.h"
#include <iostream>
#include <occi.h>
#include "db_utils.h"  

using namespace std;
using namespace oracle::occi;

// lets user add up to 5 items to the cart
int addToCart(Connection* conn, ShoppingCart cart[]) {
    int count = 0;
    int choice = 1;

    cout << "-------------- Add Products to Cart --------------" << endl;

    while (count < 5 && choice == 1) {
        int pid;
        cout << "Enter the product ID: ";
        cin >> pid;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again..." << endl;
            continue;
        }

        double price = findProduct(conn, pid);

        if (price == 0) {
            cout << "The product does not exist. Try again..." << endl;
            continue;
        }

        cout << "Product Price: " << price << endl;

        int qty;
        cout << "Enter the product quantity: ";
        cin >> qty;

        cart[count].product_id = pid;
        cart[count].price = price;
        cart[count].quantity = qty;
        count++;

        if (count < 5) {
            cout << "Enter 1 to add more products or 0 to checkout: ";
            cin >> choice;
        }
    }

    return count;
}

// displays cart contents and total price
void displayProducts(ShoppingCart cart[], int count) {
    double total = 0;
    cout << "------- Ordered Products ---------" << endl;
    for (int i = 0; i < count; i++) {
        cout << "--- Item " << (i + 1) << endl;
        cout << "Product ID: " << cart[i].product_id << endl;
        cout << "Price: $" << cart[i].price << endl;
        cout << "Quantity: " << cart[i].quantity << endl;
        total += cart[i].price * cart[i].quantity;
    }
}

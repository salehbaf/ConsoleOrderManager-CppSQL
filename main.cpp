#include <iostream>
#include <occi.h>
#include "cart.h"
#include "db_utils.h"

using namespace oracle::occi;
using namespace std;

// basic menu for customer actions
int subMenu() {
    int option = -1;
    cout << "\n******************** Customer Service Menu ********************" << endl;
    cout << "1) Place an order" << endl;
    cout << "2) Check an order status" << endl;
    cout << "3) Cancel an order" << endl;
    cout << "0) Logout" << endl;
    cout << "Enter an option (0-3): ";
    cin >> option;

    while (option < 0 || option > 3) {
        cout << "Invalid input. Enter a number between 0 and 3: ";
        cin >> option;
    }

    return option;
}

int main() {
    Environment* env = nullptr;
    Connection* conn = nullptr;

    // database login credentials
    string user = "dbs311_251nkk01";
    string pass = "30899161";
    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

    try {
        // create Oracle environment + DB connection
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);

        int option = -1;

        while (option != 0) {
            cout << "\n******************** Main Menu ********************" << endl;
            cout << "1) Login as Customer" << endl;
            cout << "0) Exit" << endl;
            cout << "Enter an option (0-1): ";
            cin >> option;

            if (option == 1) {
                int customerId;
                cout << "Enter your customer ID: ";
                cin >> customerId;

                if (customerLogin(conn, customerId)) {
                    int subOption = -1;

                    while (subOption != 0) {
                        subOption = subMenu();

                        if (subOption == 1) {
                            // place an order
                            ShoppingCart cart[5];
                            int count = addToCart(conn, cart);
                            displayProducts(cart, count);
                            checkout(conn, cart, customerId, count);
                        }
                        else if (subOption == 2) {
                            int orderId;
                            cout << "Enter your order ID: ";
                            cin >> orderId;
                            displayOrderStatus(conn, orderId, customerId);
                        }
                        else if (subOption == 3) {
                            int orderId;
                            cout << "Enter your order ID: ";
                            cin >> orderId;
                            cancelOrder(conn, orderId, customerId);
                        }
                    }
                }
                else {
                    cout << "Customer ID not found. Try again." << endl;
                }
            }
            else if (option != 0) {
                cout << "Invalid option. Please try again." << endl;
            }
        }

        // clean up
        cout << "\nGoodbye!" << endl;
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    }
    catch (SQLException& e) {
        cerr << "Connection error: " << e.getErrorCode() << " - " << e.getMessage() << endl;
    }

    return 0;
}

#include <iostream>
#include <occi.h>
#include "db_utils.h"
#include "cart.h" 
using namespace oracle::occi;
using namespace std;

// handles customer login using stored procedure
int customerLogin(Connection* conn, int customerId) {
    int found = 0;
    try {
        Statement* stmt = conn->createStatement();
        stmt->setSQL("BEGIN find_customer(:1, :2); END;");
        stmt->setInt(1, customerId);
        stmt->registerOutParam(2, Type::OCCIINT);
        stmt->executeUpdate();
        found = stmt->getInt(2);
        conn->terminateStatement(stmt);
    }
    catch (SQLException& e) {
        cerr << "login error: " << e.getErrorCode() << " - " << e.getMessage() << endl;
    }
    return found;
}

// finds a product by id and returns its price
double findProduct(Connection* conn, int product_id) {
    try {
        string name;
        double price = 0;
        Statement* stmt = conn->createStatement();
        stmt->setSQL("BEGIN find_product(:1, :2, :3); END;");
        stmt->setInt(1, product_id);
        stmt->registerOutParam(2, Type::OCCISTRING, 100);
        stmt->registerOutParam(3, Type::OCCIDOUBLE);
        stmt->executeUpdate();
        name = stmt->getString(2);
        price = stmt->getDouble(3);
        conn->terminateStatement(stmt);
        if (price == 0) return 0;
        cout << "product name: " << name << endl;
        return price;
    }
    catch (SQLException& e) {
        cerr << "error finding product: " << e.getErrorCode() << " - " << e.getMessage() << endl;
        return 0;
    }
}

// checks out all cart items and inserts into database
int checkout(Connection* conn, ShoppingCart cart[], int customerId, int productCount) {
    char confirm;
    int orderId = 0;
    cout << "would you like to checkout? (Y/y or N/n) ";
    cin >> confirm;
    while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n') {
        cout << "wrong input. try again..." << endl;
        cout << "would you like to checkout? (Y/y or N/n) ";
        cin >> confirm;
    }
    if (confirm == 'N' || confirm == 'n') {
        cout << "the order is cancelled." << endl;
        return 0;
    }
    try {
        Statement* stmt = conn->createStatement();
        stmt->setSQL("BEGIN add_order(:1, :2); END;");
        stmt->setInt(1, customerId);
        stmt->registerOutParam(2, Type::OCCIINT);
        stmt->executeUpdate();
        orderId = stmt->getInt(2);
        conn->terminateStatement(stmt);

        for (int i = 0; i < productCount; i++) {
            stmt = conn->createStatement();
            stmt->setSQL("BEGIN add_order_item(:1, :2, :3, :4, :5); END;");
            stmt->setInt(1, orderId);
            stmt->setInt(2, i + 1);
            stmt->setInt(3, cart[i].product_id);
            stmt->setInt(4, cart[i].quantity);
            stmt->setDouble(5, cart[i].price);
            stmt->executeUpdate();
            conn->terminateStatement(stmt);
        }
        cout << "the order is successfully completed." << endl;
        return 1;
    }
    catch (SQLException& e) {
        cerr << "checkout error: " << e.getErrorCode() << " - " << e.getMessage() << endl;
        return -1;
    }
}

// displays order status for a customer
void displayOrderStatus(Connection* conn, int orderId, int customerId) {
    try {
        Statement* stmt = conn->createStatement();
        stmt->setSQL("BEGIN customer_order(:1, :2); END;");
        stmt->setInt(1, customerId);
        stmt->registerOutParam(2, Type::OCCIINT);
        stmt->setInt(2, orderId);
        stmt->executeUpdate();

        int validOrder = stmt->getInt(2);
        conn->terminateStatement(stmt);

        if (validOrder == 0) {
            cout << "order id is not valid." << endl;
        }
        else {
            string status;
            stmt = conn->createStatement();
            stmt->setSQL("BEGIN display_order_status(:1, :2); END;");
            stmt->setInt(1, validOrder);
            stmt->registerOutParam(2, Type::OCCISTRING, 100);
            stmt->executeUpdate();
            status = stmt->getString(2);
            conn->terminateStatement(stmt);

            if (status.empty()) {
                cout << "order does not exist." << endl;
            }
            else {
                cout << "order is " << status << "." << endl;
            }
        }
    }
    catch (SQLException& e) {
        cerr << e.getErrorCode() << ": " << e.getMessage() << endl;
    }
}

// cancels an order
void cancelOrder(Connection* conn, int orderId, int customerId) {
    try {
        Statement* stmt = conn->createStatement();
        stmt->setSQL("BEGIN customer_order(:1, :2); END;");
        stmt->setInt(1, customerId);
        stmt->registerOutParam(2, Type::OCCIINT);
        stmt->setInt(2, orderId);
        stmt->executeUpdate();

        int validOrder = stmt->getInt(2);
        conn->terminateStatement(stmt);

        if (validOrder == 0) {
            cout << "order id is not valid." << endl;
        }
        else {
            int cancelResult;
            stmt = conn->createStatement();
            stmt->setSQL("BEGIN cancel_order(:1, :2); END;");
            stmt->setInt(1, validOrder);
            stmt->registerOutParam(2, Type::OCCIINT);
            stmt->executeUpdate();
            cancelResult = stmt->getInt(2);
            conn->terminateStatement(stmt);

            if (cancelResult == 1) {
                cout << "order is canceled." << endl;
            }
            else {
                cout << "order does not exist." << endl;
            }
        }
    }
    catch (SQLException& e) {
        cerr << e.getErrorCode() << ": " << e.getMessage() << endl;
    }
}

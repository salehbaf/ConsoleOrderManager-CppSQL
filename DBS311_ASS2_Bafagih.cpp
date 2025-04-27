/*
Name: Saleh Bafagih
ID: 116009242

Name: Haseeb Ullah danish
ID: 103815247

Saugat Gautam
129366233
*/




// ========================
//  customerLogin
// ========================

#include <iostream>
#include <occi.h>
using namespace oracle::occi;
using namespace std;

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

// ========================
// findProduct
// ========================

struct ShoppingCart {
    int product_id;
    double price;
    int quantity;
};

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
    catch (SQLException& sqlExcp) {
        cerr << "error finding product: " << sqlExcp.getErrorCode() << " - " << sqlExcp.getMessage() << endl;
        return 0;
    }
}
// ========================
// addToCart
// ========================
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
            cout << "The product does not exists. Try again..." << endl;
            continue;
        }

        cout << "Product Price: " << price << endl;

        int qty;
        cout << "Enter the product Quantity: ";
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


// ========================
// displayProducts
// ========================

void displayProducts(ShoppingCart cart[], int count) {
    double total = 0;
    cout << "------- ordered products ---------" << endl;
    for (int i = 0; i < count; i++) {
        cout << "--- item " << (i + 1) << endl;
        cout << "product id: " << cart[i].product_id << endl;
        cout << "price: " << cart[i].price << endl;
        cout << "quantity: " << cart[i].quantity << endl;
        total += cart[i].price * cart[i].quantity;
    }
    cout << "----------------------------------" << endl;
    cout << "total: " << total << endl;
}

// ========================
//  checkout
// ========================

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

// ========================
//  subMenu
// ========================
int subMenu() {
    int option = -1;
    cout << "******************** Customer Service Menu ********************" << endl;
    cout << "1) Place an order" << endl;
    cout << "2) Check an order status" << endl;
    cout << "3) Cancel an order" << endl;
    cout << "0) Exit" << endl;
    cout << "Enter an option (0-3): ";
    cin >> option;

    while (option < 0 || option > 3) {
        cout << "You entered a wrong value. Enter an option (0-3): ";
        cin >> option;
    }

    return option;
}

// ========================
//  displayOrderStatus
// ========================
void displayOrderStatus(Connection* conn, int orderId, int customerId) {
    try {
        // first: check if this order belongs to this customer
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
            // now get the order status
            string status;
            stmt = conn->createStatement();
            stmt->setSQL("BEGIN display_order_status(:1, :2); END;");
            stmt->setInt(1, validOrder); // use the confirmed order ID
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

// ========================
//  cancelOrder
// ========================

void cancelOrder(Connection* conn, int orderId, int customerId) {
    try {
        // step 1: verify order belongs to customer
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
            // step 2: try to cancel the order
            int cancelResult;
            stmt = conn->createStatement();
            stmt->setSQL("BEGIN cancel_order(:1, :2); END;");
            stmt->setInt(1, validOrder); // pass the confirmed order ID
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


// ========================
// main test
// ========================

int main() {
    Environment* env = nullptr;
    Connection* conn = nullptr;
    string user = "dbs311_251nkk01";
    string pass = "30899161";
    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

    try {
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);
     
        
        int option = -1;
        while (option != 0) {
            cout << "******************** Main Menu ********************" << endl;
            cout << "1) Login" << endl;
            cout << "0) Exit" << endl;
            cout << "Enter an option (0-1): ";
            cin >> option;

            if (option == 1) {
                int customerId;
                cout << "Enter the customer ID: ";
                cin >> customerId;

                if (customerLogin(conn, customerId)) {
                    int subOption = -1;
                    while (subOption != 0) {
                        subOption = subMenu();

                        if (subOption == 1) {
                            // place order
                            ShoppingCart cart[5];
                            int count = addToCart(conn, cart);
                            displayProducts(cart, count);
                            checkout(conn, cart, customerId, count);
                        }
                        else if (subOption == 2) {
                            int orderId;
                            cout << "Enter the order ID: ";
                            cin >> orderId;
                            displayOrderStatus(conn, orderId, customerId);
                        }
                        else if (subOption == 3) {
                            int orderId;
                            cout << "Enter the order ID: ";
                            cin >> orderId;
                            cancelOrder(conn, orderId, customerId);
                        }
                    }
                }
                else {
                    cout << "The customer does not exist." << endl;
                }
            }
            else if (option != 0) {
                cout << "You entered a wrong value. Try again..." << endl;
            }
        }

        cout << "Good bye...!" << endl;
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);

    }
    catch (SQLException& e) {
        cerr << "connection error: " << e.getErrorCode() << " - " << e.getMessage() << endl;
    }

    return 0;
}

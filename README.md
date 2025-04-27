# Console-Based E-commerce Order Manager (C++ + Oracle SQL)

This is a personal project I built to simulate a basic e-commerce ordering system using C++ and Oracle SQL. It allows users to log in, browse products, add items to a cart, place orders, and track their order status — all through a clean command-line interface backed by stored procedures and a relational database.

---

## ✅ Features

- **Customer Login** – Simulates authentication using customer IDs
- **Product Lookup** – Validates products and prices from a SQL database
- **Dynamic Shopping Cart** – Users can add multiple products and quantities
- **Checkout Process** – Places an order and stores it using stored procedures
- **Order Status Viewer** – Allows customers to check or cancel existing orders
- **Modular Design** – All C++ logic split into clean header/source files

---

## 🛠 Tech Stack

| Layer        | Technology                         |
|--------------|-------------------------------------|
| Language     | C++ (modular structure, CLI app)    |
| Database     | Oracle 12c (via SQL Developer)      |
| DB Access    | Oracle C++ Call Interface (OCCI)    |
| IDE          | Visual Studio 2022                  |

---

## 🗂 Project Structure

- /src → main.cpp, db_utils.cpp/h, cart.cpp/h
- /sql → setup.sql (stored procedures)
- README.md → project documentation

---

## ⚙️ Setup Instructions

1. Clone the repository
2. Set up your Oracle DB instance and import the `setup.sql` script
3. Update your Oracle DB credentials inside `main.cpp`:
   
   string user = "your_username";
   string pass = "your_password";
   string constr = "your_connection_string";

---

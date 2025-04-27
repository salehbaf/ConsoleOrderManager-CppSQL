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

/src ├── main.cpp ├── db_utils.cpp ├── db_utils.h ├── cart.cpp └── cart.h

/sql └── setup.sql (stored procedures)

README.md → project documentation

---

## ⚙️ Setup Instructions

1. **Clone this repository:**

```bash
git clone https://github.com/salehbaf/ConsoleOrderManager-CppSQL.git
``` 

2. **Set up your Oracle database:**

- Open SQL Developer or SQL*Plus.
- Run the sql/setup.sql script to create required stored procedures.

3. **Update database credentials:**

- In main.cpp, update the following lines with your own Oracle user information:
   - string user = "your_username";
   - string pass = "your_password";
   - string constr = "your_connection_string";

4. **Build and run the project:**

- Open the solution in Visual Studio 2022.
- Build the project and run the console application.

---

## 🚀 Why I Built This

I wanted to gain hands-on experience building a modular, database-driven C++ application that simulates a real-world business process.
This project helped strengthen my skills in backend logic development, SQL stored procedures, OCCI database integration, and modular software design.
It also mirrors the kind of transactional workflows seen in real e-commerce systems, allowing me to practice both front-end logic (CLI-based interactions) and backend database management.

---

## 🧠 Key Skills Demonstrated

- Object-oriented programming (C++)
- SQL stored procedure design
- Oracle database operations
- Modular project architecture
- Database integration using C++ OCCI
- source control (Git/GitHub)




-- =========================
-- Database Setup: E-commerce Order Manager
-- Author: Saleh Bafagih
-- =========================

-- =========================
-- Table: Customers
-- =========================
CREATE TABLE customers (
    customer_id NUMBER PRIMARY KEY,
    first_name VARCHAR2(50),
    last_name VARCHAR2(50)
);

-- =========================
-- Table: Products
-- =========================
CREATE TABLE products (
    product_id NUMBER PRIMARY KEY,
    product_name VARCHAR2(100),
    list_price NUMBER,
    category_id NUMBER
);

-- =========================
-- Table: Orders
-- =========================
CREATE TABLE orders (
    order_id NUMBER PRIMARY KEY,
    customer_id NUMBER REFERENCES customers(customer_id),
    order_date DATE,
    status VARCHAR2(20) DEFAULT 'Pending'
);

-- =========================
-- Table: Order Items
-- =========================
CREATE TABLE order_items (
    order_id NUMBER REFERENCES orders(order_id),
    item_id NUMBER,
    product_id NUMBER REFERENCES products(product_id),
    quantity NUMBER,
    unit_price NUMBER,
    PRIMARY KEY (order_id, item_id)
);

-- =========================
-- Stored Procedure: Find Customer
-- =========================
CREATE OR REPLACE PROCEDURE find_customer(
    customer_id IN NUMBER,
    found OUT NUMBER
) AS
BEGIN
    SELECT 1 INTO found
    FROM customers
    WHERE customer_id = find_customer.customer_id;
EXCEPTION
    WHEN no_data_found THEN
        found := 0;
    WHEN too_many_rows THEN
        found := 0;
    WHEN others THEN
        found := 0;
END;
/

-- =========================
-- Stored Procedure: Find Product
-- =========================
CREATE OR REPLACE PROCEDURE find_product(
    productid IN NUMBER,
    productname OUT VARCHAR2,
    price OUT NUMBER
) AS
    catid NUMBER;
    monthnum NUMBER;
BEGIN
    SELECT product_name, list_price, category_id
    INTO productname, price, catid
    FROM products
    WHERE product_id = productid;

    SELECT EXTRACT(MONTH FROM SYSDATE) INTO monthnum FROM dual;

    IF (catid = 2 OR catid = 5) AND (monthnum = 11 OR monthnum = 12) THEN
        price := price * 0.9; -- apply 10% discount for selected categories in Nov/Dec
    END IF;
EXCEPTION
    WHEN no_data_found THEN
        productname := NULL;
        price := 0;
    WHEN too_many_rows THEN
        productname := NULL;
        price := 0;
    WHEN others THEN
        productname := NULL;
        price := 0;
END;
/

-- =========================
-- Stored Procedure: Add Order
-- =========================
CREATE OR REPLACE PROCEDURE add_order(
    customer_id IN orders.customer_id%TYPE,
    new_order_id OUT orders.order_id%TYPE
) AS
BEGIN
    SELECT NVL(MAX(order_id), 0) + 1 INTO new_order_id
    FROM orders;

    INSERT INTO orders(order_id, customer_id, order_date)
    VALUES (new_order_id, customer_id, SYSDATE);
EXCEPTION
    WHEN others THEN
        DBMS_OUTPUT.PUT_LINE('add_order failed: ' || SQLERRM);
END;
/

-- =========================
-- Stored Procedure: Add Order Item
-- =========================
CREATE OR REPLACE PROCEDURE add_order_item(
    p_order_id IN order_items.order_id%TYPE,
    p_item_id IN order_items.item_id%TYPE,
    p_product_id IN order_items.product_id%TYPE,
    p_quantity IN order_items.quantity%TYPE,
    p_price IN order_items.unit_price%TYPE
) AS
BEGIN
    INSERT INTO order_items(order_id, item_id, product_id, quantity, unit_price)
    VALUES (p_order_id, p_item_id, p_product_id, p_quantity, p_price);
EXCEPTION
    WHEN others THEN
        DBMS_OUTPUT.PUT_LINE('add_order_item failed: ' || SQLERRM);
END;
/

-- =========================
-- Stored Procedure: Customer Order (verify ownership)
-- =========================
CREATE OR REPLACE PROCEDURE customer_order(
    customerid IN NUMBER,
    orderid IN OUT NUMBER
) AS
    n NUMBER;
BEGIN
    SELECT COUNT(*) INTO n
    FROM orders
    WHERE order_id = orderid
    AND customer_id = customerid;

    IF n = 0 THEN
        orderid := 0;
    END IF;
EXCEPTION
    WHEN others THEN
        orderid := 0;
END;
/

-- =========================
-- Stored Procedure: Display Order Status
-- =========================
CREATE OR REPLACE PROCEDURE display_order_status(
    orderid IN NUMBER,
    status OUT orders.status%TYPE
) AS
BEGIN
    SELECT status INTO status
    FROM orders
    WHERE order_id = orderid;
EXCEPTION
    WHEN no_data_found THEN
        status := NULL;
    WHEN others THEN
        status := NULL;
END;
/

-- =========================
-- Stored Procedure: Cancel Order
-- =========================
CREATE OR REPLACE PROCEDURE cancel_order(
    orderid IN NUMBER,
    cancel OUT NUMBER
) AS
BEGIN
    UPDATE orders
    SET status = 'Canceled'
    WHERE order_id = orderid;

    IF SQL%ROWCOUNT = 0 THEN
        cancel := 0;
    ELSE
        cancel := 1;
    END IF;
EXCEPTION
    WHEN others THEN
        cancel := 0;
END;
/

# Grandeur Restaurant — Restaurant Management System

A console-based Restaurant Management System written in C++ using
Object-Oriented Programming (OOP). It supports customer registration,
table & order management, GST-based billing, and multiple payment modes,
with separate portals for Customers, Waiters, and the Manager.

## Features

- **Customer ID**: every registered customer gets a unique, auto-incrementing
  `Customer ID`, alongside their name and phone number. Customers can look
  up their profile and order history later using their registered phone
  number.
- **Table Management**: 6 tables with different seating capacities; each
  table tracks whether it's `FREE` or `OCCUPIED` and which order it's tied to.
- **Order Management**: waiters open an order against a table, optionally
  link it to a registered customer, add menu items with quantities, and
  update its status (`Pending` → `Preparing` → `Served` → `Billed`).
- **Billing**:
  - **Subtotal** — sum of all ordered items
  - **CGST (2.5%)** and **SGST (2.5%)** calculated automatically (5% GST
    total, matching typical Indian restaurant billing — adjustable via the
    `CGST_RATE` / `SGST_RATE` constants)
  - **Grand Total** = Subtotal + CGST + SGST
  - Printed as a formatted itemized bill
- **Payment Options**: **UPI**, **Cash**, or **Other** — chosen when marking
  a bill as paid. Once paid, the table is automatically freed and, if the
  order was linked to a customer, their visit count and total spend are
  updated.
- **Manager Panel** (username: `ADMIN`, password: `ADMIN123`):
  - Manage the menu (add items, toggle availability, update prices)
  - View all orders and their bills
  - View table status
  - View a daily revenue report broken down by payment mode (UPI/Cash/Other)
  - View all registered customers and their spend

## Class Structure (OOP Design)

| Class       | Responsibility                                                          |
|-------------|--------------------------------------------------------------------------|
| `MenuItem`  | A dish/drink on the menu (id, name, category, price, availability)      |
| `OrderItem` | A single ordered line item (name, price, quantity)                      |
| `Order`     | An order for a table — holds items, computes GST/grand total, payment   |
| `Table`     | A restaurant table (number, capacity, occupied status, current order)   |
| `Customer`  | A registered customer (id, name, phone, visit count, total spend)       |

Each class encapsulates its own data behind private members with public
getters/setters, and the GST/payment logic lives entirely inside `Order`,
keeping billing calculations in one place.

## How to Compile & Run

```bash
g++ -std=c++17 -o restaurant_management restaurant_management.cpp
./restaurant_management
```

On Windows (with g++/MinGW installed):

```bash
g++ -std=c++17 -o restaurant_management.exe restaurant_management.cpp
restaurant_management.exe
```

## Login Credentials (for testing)

| Role    | Username | Password    |
|---------|----------|-------------|
| Manager | `ADMIN`  | `ADMIN123`  |
| Waiter  | any name | `waiter123` |

## Sample Flow

1. **Customer Portal** → Register with name & phone → get a Customer ID.
2. **Waiter Login** → take a new order for a table → link it to the
   customer's phone number → add menu items and quantities.
3. **Waiter Login → Generate Bill** → enter the Order ID → view the itemized
   bill with CGST, SGST, and Grand Total → mark as paid, choosing UPI, Cash,
   or Other → table is freed and the customer's spend is updated.
4. **Manager Login** → view daily revenue split by payment mode, or look up
   any customer's total visits and spend.

## Possible Extensions

- Persist customers/orders/menu to a file or database so data survives restarts
- Add item-level GST rates (e.g. different rates for food vs beverages)
- Add discounts, loyalty points, or coupon codes
- Support order cancellation / item removal
- Export bills as printable PDF/text receipts
- Add reservation handling for tables

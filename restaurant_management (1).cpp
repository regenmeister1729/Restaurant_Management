// ============================================================
//  Restaurant Management System — Single File C++ OOP Project
//  Classes: MenuItem, OrderItem, Order, Table, Customer
// ============================================================
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;
// ─── Global Counters ─────────────────────────────────────────
int nextOrderId    = 1001;
int nextMenuId     = 1;
int nextCustomerId = 1;
// GST rates (CGST + SGST = 2.5% each = 5% total, as per Indian restaurant norms)
const double CGST_RATE = 0.025;   // 2.5%
const double SGST_RATE = 0.025;   // 2.5%
// ─── Class: MenuItem ────────────────────────────────────────
class MenuItem {
private:
    int    id;
    string name;
    string category;   // Starter / Main / Beverage / Dessert
    double price;
    bool   available;
public:
    MenuItem(int id, const string& name, const string& category, double price)
        : id(id), name(name), category(category), price(price), available(true) {}
    int    getId()       const { return id;        }
    string getName()     const { return name;      }
    string getCategory() const { return category;  }
    double getPrice()    const { return price;     }
    bool   isAvailable() const { return available; }
    void setAvailable(bool a)       { available = a; }
    void setPrice(double p)         { price = p;     }
    void setName(const string& n)   { name = n;      }
};
// ─── Class: Customer ────────────────────────────────────────
class Customer {
private:
    int    id;
    string name;
    string phone;
    int    totalOrders;
    double totalSpent;
public:
    Customer(int id, const string& name, const string& phone)
        : id(id), name(name), phone(phone), totalOrders(0), totalSpent(0.0) {}
    int    getId()          const { return id;          }
    string getName()        const { return name;        }
    string getPhone()       const { return phone;       }
    int    getTotalOrders() const { return totalOrders; }
    double getTotalSpent()  const { return totalSpent;  }
    void recordOrder(double amount) {
        totalOrders++;
        totalSpent += amount;
    }
    void showDetails() const {
        cout << "  Customer ID: " << id << endl;
        cout << "  Customer   : " << name << endl;
        cout << "  Phone      : " << phone << endl;
        cout << "  Visits     : " << totalOrders << endl;
        cout << "  Total Spent: Rs." << fixed << setprecision(2) << totalSpent << endl;
    }
};
// ─── Class: OrderItem ───────────────────────────────────────
class OrderItem {
private:
    string itemName;
    double itemPrice;
    int    quantity;
public:
    OrderItem(const string& name, double price, int qty)
        : itemName(name), itemPrice(price), quantity(qty) {}
    string getName()     const { return itemName;             }
    double getPrice()    const { return itemPrice;            }
    int    getQty()      const { return quantity;             }
    double getSubtotal() const { return itemPrice * quantity; }
};
// ─── Class: Order ───────────────────────────────────────────
class Order {
private:
    int    orderId;
    int    tableNumber;
    string waiterName;
    int    customerId;     // -1 if no linked customer (Walk-in)
    string customerName;   // "Walk-in" if no customer
    string customerPhone;
    vector<OrderItem> items;
    string status;         // Pending / Preparing / Served / Billed
    string paymentMode;    // UPI / Cash / Other
    bool   paid;
public:
    Order(int id, int tableNum, const string& waiter)
        : orderId(id), tableNumber(tableNum), waiterName(waiter),
          customerId(-1), customerName("Walk-in"), customerPhone("-"),
          status("Pending"), paymentMode(""), paid(false) {}
    // Setters
    void addItem(const OrderItem& item)        { items.push_back(item);   }
    void setStatus(const string& s)            { status = s;              }
    void setCustomer(int id, const string& n,
                     const string& p)          { customerId = id; customerName = n; customerPhone = p; }
    void markPaid(const string& mode)          { paid = true; status = "Billed"; paymentMode = mode; }
    // Getters
    int    getOrderId()       const { return orderId;       }
    int    getTableNumber()   const { return tableNumber;   }
    string getWaiter()        const { return waiterName;    }
    int    getCustomerId()    const { return customerId;    }
    string getCustomerName()  const { return customerName;  }
    string getStatus()        const { return status;        }
    string getPaymentMode()   const { return paymentMode;   }
    bool   isPaid()           const { return paid;          }
    bool   isEmpty()          const { return items.empty(); }
    // Financial calculations
    double getSubtotal()   const {
        double sub = 0;
        for (const auto& item : items) sub += item.getSubtotal();
        return sub;
    }
    double getCGST()       const { return getSubtotal() * CGST_RATE; }
    double getSGST()       const { return getSubtotal() * SGST_RATE; }
    double getGrandTotal() const { return getSubtotal() + getCGST() + getSGST(); }
    // Keep getTotal() as alias so manager revenue works the same way
    double getTotal()      const { return getGrandTotal(); }
    void showBill() const {
        double sub   = getSubtotal();
        double cgst  = getCGST();
        double sgst  = getSGST();
        double grand = getGrandTotal();
        cout << "\n  ============================================" << endl;
        cout << "           GRANDEUR RESTAURANT" << endl;
        cout << "  ============================================" << endl;
        cout << "  Order #" << orderId
             << "            Table: " << tableNumber << endl;
        cout << "  Customer   : " << customerName;
        if (customerId != -1) cout << "  (ID: " << customerId << ")";
        if (customerPhone != "-") cout << "  Ph: " << customerPhone;
        cout << endl;
        cout << "  Waiter     : " << waiterName << endl;
        cout << "  Status     : " << status << endl;
        cout << "  --------------------------------------------" << endl;
        cout << "  " << left << setw(22) << "Item"
             << setw(5)  << "Qty"
             << right << setw(10) << "Amount" << endl;
        cout << "  --------------------------------------------" << endl;
        for (const auto& item : items) {
            cout << "  " << left  << setw(22) << item.getName()
                 << setw(5)  << item.getQty()
                 << right << setw(10) << fixed << setprecision(2)
                 << item.getSubtotal() << endl;
        }
        cout << "  --------------------------------------------" << endl;
        cout << "  " << left  << setw(27) << "Subtotal"
             << right << setw(10) << sub   << endl;
        cout << "  " << left  << setw(27) << "CGST (2.5%)"
             << right << setw(10) << cgst  << endl;
        cout << "  " << left  << setw(27) << "SGST (2.5%)"
             << right << setw(10) << sgst  << endl;
        cout << "  --------------------------------------------" << endl;
        cout << "  " << left  << setw(27) << "GRAND TOTAL"
             << right << setw(10) << grand << endl;
        cout << "  --------------------------------------------" << endl;
        if (paid)
            cout << "  Payment Mode: " << paymentMode << endl;
        cout << "  Paid        : " << (paid ? "Yes" : "No") << endl;
        cout << "  ============================================" << endl;
        cout << "      Thank you! Please visit again :)" << endl;
        cout << "  ============================================\n" << endl;
    }
};
// ─── Class: Table ───────────────────────────────────────────
class Table {
private:
    int  tableNumber;
    int  capacity;
    bool occupied;
    int  currentOrderId;
public:
    Table(int num, int cap)
        : tableNumber(num), capacity(cap), occupied(false), currentOrderId(-1) {}
    int  getNumber()         const { return tableNumber;    }
    int  getCapacity()       const { return capacity;       }
    bool isOccupied()        const { return occupied;       }
    int  getCurrentOrderId() const { return currentOrderId; }
    void occupy(int orderId) { occupied = true;  currentOrderId = orderId; }
    void free()              { occupied = false; currentOrderId = -1;      }
};
// ─── Global Vectors ─────────────────────────────────────────
vector<MenuItem> menu;
vector<Order>    orders;
vector<Table>    tables;
vector<Customer> customers;
// ─── Helper Functions ───────────────────────────────────────
int findTableIndex(int tableNum) {
    for (int i = 0; i < (int)tables.size(); ++i)
        if (tables[i].getNumber() == tableNum) return i;
    return -1;
}
int findOrderIndex(int orderId) {
    for (int i = 0; i < (int)orders.size(); ++i)
        if (orders[i].getOrderId() == orderId) return i;
    return -1;
}
int findCustomerByPhone(const string& phone) {
    for (int i = 0; i < (int)customers.size(); ++i)
        if (customers[i].getPhone() == phone) return i;
    return -1;
}
int findCustomerById(int id) {
    for (int i = 0; i < (int)customers.size(); ++i)
        if (customers[i].getId() == id) return i;
    return -1;
}
void viewMenu() {
    string categories[] = { "Starter", "Main", "Beverage", "Dessert" };
    cout << "\n  =============================================" << endl;
    cout << "             GRANDEUR RESTAURANT MENU         " << endl;
    cout << "  =============================================" << endl;
    for (const auto& cat : categories) {
        bool headerPrinted = false;
        for (const auto& item : menu) {
            if (item.getCategory() == cat && item.isAvailable()) {
                if (!headerPrinted) {
                    cout << "\n  --- " << cat << "s ---" << endl;
                    headerPrinted = true;
                }
                cout << "  [" << item.getId() << "] "
                     << left << setw(22) << item.getName()
                     << "Rs." << fixed << setprecision(2) << item.getPrice() << endl;
            }
        }
    }
    cout << "  =============================================\n" << endl;
}
// Prompts waiter to select payment mode; returns the chosen string
string selectPaymentMode() {
    cout << "\n  Select Payment Mode:" << endl;
    cout << "  1. UPI" << endl;
    cout << "  2. Cash" << endl;
    cout << "  3. Other" << endl;
    cout << "  Enter choice: ";
    int m;
    cin >> m;
    switch (m) {
        case 1: return "UPI";
        case 2: return "Cash";
        default: return "Other";
    }
}
// ─── Function Declarations ───────────────────────────────────
void showHome();
void waiterPanel();
void managerPanel();
void customerPanel();
void initData();
// ─── Main ────────────────────────────────────────────────────
int main() {
    initData();
    showHome();
    return 0;
}
// ─── Seed Data ───────────────────────────────────────────────
void initData() {
    menu.emplace_back(nextMenuId++, "Spring Rolls",    "Starter",  120.0);
    menu.emplace_back(nextMenuId++, "Paneer Tikka",    "Starter",  180.0);
    menu.emplace_back(nextMenuId++, "Chicken Soup",    "Starter",  150.0);
    menu.emplace_back(nextMenuId++, "Butter Chicken",  "Main",     320.0);
    menu.emplace_back(nextMenuId++, "Dal Makhani",     "Main",     240.0);
    menu.emplace_back(nextMenuId++, "Veg Biryani",     "Main",     220.0);
    menu.emplace_back(nextMenuId++, "Grilled Salmon",  "Main",     480.0);
    menu.emplace_back(nextMenuId++, "Mango Lassi",     "Beverage",  90.0);
    menu.emplace_back(nextMenuId++, "Masala Chai",     "Beverage",  50.0);
    menu.emplace_back(nextMenuId++, "Fresh Lime Soda", "Beverage",  70.0);
    menu.emplace_back(nextMenuId++, "Gulab Jamun",     "Dessert",  110.0);
    menu.emplace_back(nextMenuId++, "Ice Cream",       "Dessert",  130.0);
    for (int i = 1; i <= 6; ++i)
        tables.emplace_back(i, (i <= 2 ? 2 : i <= 4 ? 4 : 6));
}
// ─── Home Screen ─────────────────────────────────────────────
void showHome() {
    while (true) {
        cout << "\n\t\t==========================================" << endl;
        cout << "\t\t    WELCOME TO GRANDEUR RESTAURANT" << endl;
        cout << "\t\t==========================================" << endl;
        cout << "1. View Menu" << endl;
        cout << "2. Customer Portal" << endl;
        cout << "3. Waiter Login" << endl;
        cout << "4. Manager Login" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        switch (choice) {
            case 1: viewMenu();      break;
            case 2: customerPanel(); break;
            case 3: waiterPanel();   break;
            case 4: managerPanel();  break;
            case 0: cout << "Thank you! Visit again!" << endl; return;
            default: cout << "Invalid choice! Try again." << endl;
        }
    }
}
// ─── Customer Portal ─────────────────────────────────────────
void customerPanel() {
    int choice;
    cout << "\n  --- Customer Portal ---" << endl;
    cout << "1. Register" << endl;
    cout << "2. View My Details & Order History" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    if (choice == 1) {
        string name, phone;
        cout << "  Enter Your Name : ";
        cin >> name;
        cout << "  Enter Phone No  : ";
        cin >> phone;
        if (findCustomerByPhone(phone) != -1) {
            cout << "  Phone number already registered!" << endl;
            return;
        }
        int newId = nextCustomerId++;
        customers.emplace_back(newId, name, phone);
        cout << "  Registered successfully! Welcome, " << name << "!" << endl;
        cout << "  Your Customer ID is: " << newId << endl;
    } else if (choice == 2) {
        string phone;
        cout << "  Enter Your Phone No: ";
        cin >> phone;
        int cIdx = findCustomerByPhone(phone);
        if (cIdx == -1) {
            cout << "  Phone number not found. Please register first." << endl;
            return;
        }
        cout << "\n  --- Your Profile ---" << endl;
        customers[cIdx].showDetails();
        // Show order history for this customer
        bool anyOrders = false;
        cout << "\n  --- Your Orders ---" << endl;
        for (const auto& order : orders) {
            if (order.getCustomerId() == customers[cIdx].getId()) {
                cout << "  Order #" << order.getOrderId()
                     << " | Table " << order.getTableNumber()
                     << " | Status: " << order.getStatus()
                     << " | Total: Rs." << fixed << setprecision(2) << order.getGrandTotal() << endl;
                anyOrders = true;
            }
        }
        if (!anyOrders) cout << "  No orders found." << endl;
    } else {
        cout << "  Invalid choice." << endl;
    }
}
// ─── Waiter Panel ────────────────────────────────────────────
void waiterPanel() {
    string waiterName, password;
    cout << "Enter Waiter Name: ";
    cin >> waiterName;
    cout << "Enter Password: ";
    cin >> password;
    if (password != "waiter123") {
        cout << "Invalid credentials!" << endl;
        return;
    }
    char choice;
    while (true) {
        cout << "\n  --- Waiter Panel (" << waiterName << ") ---" << endl;
        cout << "T. View Tables" << endl;
        cout << "N. Take New Order" << endl;
        cout << "A. Add Item to Order" << endl;
        cout << "U. Update Order Status" << endl;
        cout << "B. Generate Bill" << endl;
        cout << "E. Exit to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        // ── View Tables ──────────────────────────────────────
        if (choice == 'T' || choice == 't') {
            cout << "\n  --- TABLE STATUS ---" << endl;
            for (const auto& t : tables) {
                cout << "  Table " << t.getNumber()
                     << " (" << t.getCapacity() << " seats) - ";
                if (t.isOccupied())
                    cout << "OCCUPIED (Order #" << t.getCurrentOrderId() << ")" << endl;
                else
                    cout << "FREE" << endl;
            }
        // ── Take New Order ───────────────────────────────────
        } else if (choice == 'N' || choice == 'n') {
            int tableNum;
            cout << "Enter Table Number: ";
            cin >> tableNum;
            int tIdx = findTableIndex(tableNum);
            if (tIdx == -1) { cout << "Table not found." << endl; continue; }
            if (tables[tIdx].isOccupied()) { cout << "Table is already occupied!" << endl; continue; }
            Order newOrder(nextOrderId, tableNum, waiterName);
            // ── Link a customer ──────────────────────────────
            char regChoice;
            cout << "Is the customer registered? (Y/N): ";
            cin >> regChoice;
            if (regChoice == 'Y' || regChoice == 'y') {
                string phone;
                cout << "Enter customer phone: ";
                cin >> phone;
                int cIdx = findCustomerByPhone(phone);
                if (cIdx == -1) {
                    cout << "Phone not found. Proceeding as Walk-in." << endl;
                } else {
                    newOrder.setCustomer(customers[cIdx].getId(),
                                        customers[cIdx].getName(),
                                        customers[cIdx].getPhone());
                    cout << "Welcome back, " << customers[cIdx].getName()
                         << " (ID: " << customers[cIdx].getId() << ")!" << endl;
                }
            }
            // ── Add items ────────────────────────────────────
            viewMenu();
            cout << "Add items (enter 0 as ID when done):" << endl;
            while (true) {
                int itemId;
                cout << "  Enter Item ID: ";
                cin >> itemId;
                if (itemId == 0) break;
                bool found = false;
                for (const auto& item : menu) {
                    if (item.getId() == itemId && item.isAvailable()) {
                        int qty;
                        cout << "  Enter quantity for " << item.getName() << ": ";
                        cin >> qty;
                        newOrder.addItem(OrderItem(item.getName(), item.getPrice(), qty));
                        cout << "  Added: " << item.getName() << " x" << qty << endl;
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "  Item not found or unavailable." << endl;
            }
            if (newOrder.isEmpty()) {
                cout << "No items added. Order cancelled." << endl;
                continue;
            }
            tables[tIdx].occupy(nextOrderId);
            orders.push_back(newOrder);
            cout << "Order #" << nextOrderId << " created for Table " << tableNum << "!" << endl;
            nextOrderId++;
        // ── Add Item to Existing Order ───────────────────────
        } else if (choice == 'A' || choice == 'a') {
            int orderId;
            cout << "Enter Order ID: ";
            cin >> orderId;
            int oIdx = findOrderIndex(orderId);
            if (oIdx == -1) { cout << "Order not found." << endl; continue; }
            if (orders[oIdx].getStatus() == "Billed") { cout << "Order already billed." << endl; continue; }
            viewMenu();
            while (true) {
                int itemId;
                cout << "  Enter Item ID (0 to stop): ";
                cin >> itemId;
                if (itemId == 0) break;
                bool found = false;
                for (const auto& item : menu) {
                    if (item.getId() == itemId && item.isAvailable()) {
                        int qty;
                        cout << "  Quantity: ";
                        cin >> qty;
                        orders[oIdx].addItem(OrderItem(item.getName(), item.getPrice(), qty));
                        cout << "  " << item.getName() << " added." << endl;
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "  Item not found." << endl;
            }
        // ── Update Order Status ──────────────────────────────
        } else if (choice == 'U' || choice == 'u') {
            int orderId;
            cout << "Enter Order ID: ";
            cin >> orderId;
            int oIdx = findOrderIndex(orderId);
            if (oIdx == -1) { cout << "Order not found." << endl; continue; }
            if (orders[oIdx].isPaid()) { cout << "Order already billed." << endl; continue; }
            cout << "Current Status: " << orders[oIdx].getStatus() << endl;
            cout << "1. Pending" << endl;
            cout << "2. Preparing" << endl;
            cout << "3. Served" << endl;
            cout << "Enter new status: ";
            int s;
            cin >> s;
            string statuses[] = { "", "Pending", "Preparing", "Served" };
            if (s >= 1 && s <= 3) {
                orders[oIdx].setStatus(statuses[s]);
                cout << "Status updated to: " << statuses[s] << endl;
            } else {
                cout << "Invalid choice." << endl;
            }
        // ── Generate Bill ────────────────────────────────────
        } else if (choice == 'B' || choice == 'b') {
            int orderId;
            cout << "Enter Order ID: ";
            cin >> orderId;
            int oIdx = findOrderIndex(orderId);
            if (oIdx == -1) { cout << "Order not found." << endl; continue; }
            orders[oIdx].showBill();
            if (!orders[oIdx].isPaid()) {
                cout << "Mark as paid? (Y/N): ";
                char c;
                cin >> c;
                if (c == 'Y' || c == 'y') {
                    string mode = selectPaymentMode();
                    int tIdx = findTableIndex(orders[oIdx].getTableNumber());
                    orders[oIdx].markPaid(mode);
                    if (tIdx != -1) tables[tIdx].free();
                    // Update linked customer's order count & spend, if any
                    int cIdx = findCustomerById(orders[oIdx].getCustomerId());
                    if (cIdx != -1) {
                        customers[cIdx].recordOrder(orders[oIdx].getGrandTotal());
                    }
                    cout << "Payment received via " << mode << "!" << endl;
                    cout << "Table " << orders[oIdx].getTableNumber() << " is now free." << endl;
                    // Print final paid bill
                    orders[oIdx].showBill();
                }
            }
        } else if (choice == 'E' || choice == 'e') {
            break;
        } else {
            cout << "Invalid choice." << endl;
        }
    }
}
// ─── Manager Panel ───────────────────────────────────────────
void managerPanel() {
    string username, password;
    cout << "Enter Admin Username: ";
    cin >> username;
    if (username != "ADMIN") {
        cout << "Invalid username!" << endl;
        return;
    }
    cout << "Enter Admin Password: ";
    cin >> password;
    if (password != "ADMIN123") {
        cout << "Invalid password!" << endl;
        return;
    }
    int choice;
    while (true) {
        cout << "\n  --- Manager Panel ---" << endl;
        cout << "1.  View Full Menu (incl. unavailable)" << endl;
        cout << "2.  Add Menu Item" << endl;
        cout << "3.  Toggle Item Availability" << endl;
        cout << "4.  Update Item Price" << endl;
        cout << "5.  View All Orders" << endl;
        cout << "6.  View Table Status" << endl;
        cout << "7.  View Daily Revenue" << endl;
        cout << "8.  View All Customers" << endl;
        cout << "0.  Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1) {
            viewMenu();
            bool anyUnavail = false;
            for (const auto& item : menu) {
                if (!item.isAvailable()) {
                    if (!anyUnavail) {
                        cout << "  --- Unavailable Items ---" << endl;
                        anyUnavail = true;
                    }
                    cout << "  [" << item.getId() << "] " << item.getName()
                         << " (" << item.getCategory() << ") - UNAVAILABLE" << endl;
                }
            }
            if (!anyUnavail) cout << "  All items are currently available." << endl;
        } else if (choice == 2) {
            string name, cat;
            double price;
            cout << "Item Name: ";       cin >> name;
            cout << "Category (Starter / Main / Beverage / Dessert): "; cin >> cat;
            cout << "Price: ";           cin >> price;
            menu.emplace_back(nextMenuId++, name, cat, price);
            cout << "Item added to menu!" << endl;
        } else if (choice == 3) {
            viewMenu();
            int id;
            cout << "Enter Item ID to toggle availability: ";
            cin >> id;
            bool found = false;
            for (auto& item : menu) {
                if (item.getId() == id) {
                    item.setAvailable(!item.isAvailable());
                    cout << item.getName() << " is now "
                         << (item.isAvailable() ? "AVAILABLE" : "UNAVAILABLE") << endl;
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Item not found." << endl;
        } else if (choice == 4) {
            viewMenu();
            int id; double newPrice;
            cout << "Enter Item ID: ";   cin >> id;
            cout << "Enter New Price: "; cin >> newPrice;
            bool found = false;
            for (auto& item : menu) {
                if (item.getId() == id) {
                    item.setPrice(newPrice);
                    cout << "Price updated for " << item.getName() << endl;
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Item not found." << endl;
        } else if (choice == 5) {
            if (orders.empty()) { cout << "No orders yet." << endl; continue; }
            for (const auto& order : orders) order.showBill();
        } else if (choice == 6) {
            cout << "\n  --- TABLE STATUS ---" << endl;
            for (const auto& t : tables) {
                cout << "  Table " << t.getNumber()
                     << " | Capacity: " << t.getCapacity() << " | ";
                if (t.isOccupied())
                    cout << "OCCUPIED (Order #" << t.getCurrentOrderId() << ")" << endl;
                else
                    cout << "FREE" << endl;
            }
        } else if (choice == 7) {
            double revenue = 0;
            int billedCount = 0;
            double cashRev = 0, upiRev = 0, otherRev = 0;
            for (const auto& order : orders) {
                if (order.isPaid()) {
                    double amt = order.getGrandTotal();
                    revenue += amt;
                    billedCount++;
                    if (order.getPaymentMode() == "Cash")      cashRev  += amt;
                    else if (order.getPaymentMode() == "UPI")  upiRev   += amt;
                    else                                        otherRev += amt;
                }
            }
            cout << "\n  -------- DAILY REVENUE REPORT --------" << endl;
            cout << "  Billed Orders  : " << billedCount << endl;
            cout << "  Pending Orders : " << (orders.size() - billedCount) << endl;
            cout << "  --------------------------------------" << endl;
            cout << "  Cash           : Rs." << fixed << setprecision(2) << cashRev  << endl;
            cout << "  UPI            : Rs." << upiRev  << endl;
            cout << "  Other          : Rs." << otherRev << endl;
            cout << "  --------------------------------------" << endl;
            cout << "  TOTAL REVENUE  : Rs." << revenue << endl;
            cout << "  --------------------------------------" << endl;
        } else if (choice == 8) {
            if (customers.empty()) { cout << "No customers registered yet." << endl; continue; }
            cout << "\n  --- REGISTERED CUSTOMERS ---" << endl;
            for (const auto& c : customers) {
                cout << "  [ID " << c.getId() << "] " << c.getName()
                     << " | Ph: " << c.getPhone()
                     << " | Visits: " << c.getTotalOrders()
                     << " | Spent: Rs." << fixed << setprecision(2) << c.getTotalSpent() << endl;
            }
        } else if (choice == 0) {
            break;
        } else {
            cout << "Invalid choice." << endl;
        }
    }
}

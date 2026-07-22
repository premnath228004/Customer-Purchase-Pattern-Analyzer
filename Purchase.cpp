#include "Purchase.h"
#include <iostream>
#include <sstream>
#include <vector>

Purchase::Purchase()
    : customerId(""), customerName(""), productName(""), category(""),
      quantity(0), price(0.0), date("") {}

Purchase::Purchase(string customerId, string customerName, string productName,
                    string category, int quantity, double price, string date)
    : customerId(customerId), customerName(customerName), productName(productName),
      category(category), quantity(quantity), price(price), date(date) {}

string Purchase::getCustomerId() const { return customerId; }
string Purchase::getCustomerName() const { return customerName; }
string Purchase::getProductName() const { return productName; }
string Purchase::getCategory() const { return category; }
int Purchase::getQuantity() const { return quantity; }
double Purchase::getPrice() const { return price; }
string Purchase::getDate() const { return date; }
double Purchase::getTotal() const { return quantity * price; }

void Purchase::setCustomerName(const string& name) { customerName = name; }
void Purchase::setProductName(const string& product) { productName = product; }
void Purchase::setCategory(const string& cat) { category = cat; }
void Purchase::setQuantity(int qty) { quantity = qty; }
void Purchase::setPrice(double pr) { price = pr; }
void Purchase::setDate(const string& dt) { date = dt; }

string Purchase::toCSV() const {
    ostringstream oss;
    oss << customerId << "," << customerName << "," << productName << ","
        << category << "," << quantity << "," << price << "," << date;
    return oss.str();
}

// Parses one CSV line into a Purchase. Sets `ok` to false on malformed input
// instead of throwing, so a single bad line can't crash the whole load.
Purchase Purchase::fromCSV(const string& line, bool& ok) {
    ok = false;
    vector<string> parts;
    string field;
    stringstream ss(line);

    while (getline(ss, field, ',')) {
        parts.push_back(field);
    }

    if (parts.size() < 7) return Purchase();

    try {
        Purchase p;
        p.customerId = parts[0];
        p.customerName = parts[1];
        p.productName = parts[2];
        p.category = parts[3];
        p.quantity = stoi(parts[4]);
        p.price = stod(parts[5]);
        p.date = parts[6];
        ok = true;
        return p;
    } catch (...) {
        return Purchase();
    }
}

void Purchase::print() const {
    cout << "Customer ID   : " << customerId << endl;
    cout << "Customer Name : " << customerName << endl;
    cout << "Product Name  : " << productName << endl;
    cout << "Category      : " << category << endl;
    cout << "Quantity      : " << quantity << endl;
    cout << "Price         : Rs. " << price << endl;
    cout << "Date          : " << date << endl;
    cout << "Line Total    : Rs. " << getTotal() << endl;
    cout << "------------------------------------------\n";
}

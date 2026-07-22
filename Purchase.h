#ifndef PURCHASE_H
#define PURCHASE_H

#include <string>
using namespace std;

// Represents a single purchase transaction record.
class Purchase {
private:
    string customerId;
    string customerName;
    string productName;
    string category;
    int quantity;
    double price;
    string date;

public:
    Purchase();
    Purchase(string customerId, string customerName, string productName,
              string category, int quantity, double price, string date);

    // Getters
    string getCustomerId() const;
    string getCustomerName() const;
    string getProductName() const;
    string getCategory() const;
    int getQuantity() const;
    double getPrice() const;
    string getDate() const;
    double getTotal() const;

    // Setters (used by the edit-record feature)
    void setCustomerName(const string& name);
    void setProductName(const string& product);
    void setCategory(const string& cat);
    void setQuantity(int qty);
    void setPrice(double pr);
    void setDate(const string& dt);

    // Serialization helpers
    string toCSV() const;
    static Purchase fromCSV(const string& line, bool& ok);

    void print() const;
};

#endif

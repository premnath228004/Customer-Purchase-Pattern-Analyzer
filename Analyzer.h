#ifndef ANALYZER_H
#define ANALYZER_H

#include <vector>
#include <string>
#include <map>
#include "Purchase.h"
using namespace std;

// All analytics logic lives here, separate from I/O and the menu/UI layer.
class Analyzer {
public:
    static double totalRevenue(const vector<Purchase>& records);

    // Returns {productName, totalQuantitySold}
    static pair<string, int> topSellingProduct(const vector<Purchase>& records);

    // Returns {customerName, customerId, totalSpent}
    static tuple<string, string, double> highestSpendingCustomer(const vector<Purchase>& records);

    // category -> total sales, sorted by value descending
    static vector<pair<string, double>> categorySales(const vector<Purchase>& records);

    static string recommendProduct(const vector<Purchase>& records, const string& product);

    static vector<Purchase> searchByCustomerId(const vector<Purchase>& records, const string& customerId);

    static double averageOrderValue(const vector<Purchase>& records);

    static int uniqueCustomerCount(const vector<Purchase>& records);

    // Builds a full plain-text business summary suitable for exporting/printing.
    static string generateSummaryReport(const vector<Purchase>& records);
};

#endif

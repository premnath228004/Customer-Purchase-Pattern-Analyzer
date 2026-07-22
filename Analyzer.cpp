#include "Analyzer.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <set>

double Analyzer::totalRevenue(const vector<Purchase>& records) {
    double revenue = 0.0;
    for (const auto& p : records) revenue += p.getTotal();
    return revenue;
}

pair<string, int> Analyzer::topSellingProduct(const vector<Purchase>& records) {
    map<string, int> qtyByProduct;
    for (const auto& p : records) {
        qtyByProduct[p.getProductName()] += p.getQuantity();
    }

    string topProduct = "";
    int maxQty = 0;
    for (const auto& entry : qtyByProduct) {
        if (entry.second > maxQty) {
            maxQty = entry.second;
            topProduct = entry.first;
        }
    }
    return {topProduct, maxQty};
}

tuple<string, string, double> Analyzer::highestSpendingCustomer(const vector<Purchase>& records) {
    map<string, double> spendByCustomer;
    map<string, string> nameByCustomer;

    for (const auto& p : records) {
        spendByCustomer[p.getCustomerId()] += p.getTotal();
        nameByCustomer[p.getCustomerId()] = p.getCustomerName();
    }

    string topId = "", topName = "";
    double maxSpent = 0.0;
    for (const auto& entry : spendByCustomer) {
        if (entry.second > maxSpent) {
            maxSpent = entry.second;
            topId = entry.first;
            topName = nameByCustomer[entry.first];
        }
    }
    return make_tuple(topName, topId, maxSpent);
}

vector<pair<string, double>> Analyzer::categorySales(const vector<Purchase>& records) {
    map<string, double> totals;
    for (const auto& p : records) {
        totals[p.getCategory()] += p.getTotal();
    }

    vector<pair<string, double>> result(totals.begin(), totals.end());
    sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    return result;
}

string Analyzer::recommendProduct(const vector<Purchase>& records, const string& product) {
    if (records.size() < 2) return "";

    set<string> buyers;
    for (const auto& p : records) {
        if (p.getProductName() == product) buyers.insert(p.getCustomerId());
    }
    if (buyers.empty()) return "";

    map<string, int> coOccurrence;
    for (const auto& p : records) {
        if (buyers.count(p.getCustomerId()) && p.getProductName() != product) {
            coOccurrence[p.getProductName()]++;
        }
    }

    string best = "";
    int maxCount = 0;
    for (const auto& entry : coOccurrence) {
        if (entry.second > maxCount) {
            maxCount = entry.second;
            best = entry.first;
        }
    }
    return best;
}

vector<Purchase> Analyzer::searchByCustomerId(const vector<Purchase>& records, const string& customerId) {
    vector<Purchase> matches;
    for (const auto& p : records) {
        if (p.getCustomerId() == customerId) matches.push_back(p);
    }
    return matches;
}

double Analyzer::averageOrderValue(const vector<Purchase>& records) {
    if (records.empty()) return 0.0;
    return totalRevenue(records) / records.size();
}

int Analyzer::uniqueCustomerCount(const vector<Purchase>& records) {
    set<string> ids;
    for (const auto& p : records) ids.insert(p.getCustomerId());
    return (int)ids.size();
}

string Analyzer::generateSummaryReport(const vector<Purchase>& records) {
    ostringstream out;
    out << fixed << setprecision(2);

    out << "=====================================================\n";
    out << "   SMART CUSTOMER PURCHASE PATTERN ANALYZER\n";
    out << "   Store: Prem Supermart\n";
    out << "   Business Summary Report\n";
    out << "=====================================================\n\n";

    out << "Total Records        : " << records.size() << "\n";
    out << "Unique Customers     : " << uniqueCustomerCount(records) << "\n";
    out << "Total Revenue        : Rs. " << totalRevenue(records) << "\n";
    out << "Average Order Value  : Rs. " << averageOrderValue(records) << "\n\n";

    auto top = topSellingProduct(records);
    out << "Top Selling Product  : " << (top.first.empty() ? "N/A" : top.first)
        << " (" << top.second << " units)\n";

    auto customer = highestSpendingCustomer(records);
    out << "Top Customer         : " << (get<0>(customer).empty() ? "N/A" : get<0>(customer))
        << " [" << get<1>(customer) << "] - Rs. " << get<2>(customer) << "\n\n";

    out << "----------------- Category Wise Sales --------------\n";
    for (const auto& entry : categorySales(records)) {
        out << "  " << left << setw(18) << entry.first << ": Rs. " << entry.second << "\n";
    }

    out << "\n=====================================================\n";
    return out.str();
}

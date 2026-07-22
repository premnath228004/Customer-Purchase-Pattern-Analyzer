#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "Purchase.h"
#include "Filehandler.h"
#include "Analyzer.h"
using namespace std;

const string DATA_FILE = "data.txt";
vector<Purchase> records;

// ---------- small input helpers (fix: no more crash/infinite loop on bad input) ----------

int readInt(const string& prompt, int minValue = numeric_limits<int>::min()) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a number";
        if (minValue != numeric_limits<int>::min()) cout << " >= " << minValue;
        cout << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readDouble(const string& prompt, double minValue = 0.0) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a number >= " << minValue << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt, bool allowEmpty = false) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (!value.empty() || allowEmpty) return value;
        cout << "This field cannot be empty.\n";
    }
}

// ---------------------------------- menu actions ----------------------------------

void addPurchase() {
    cout << "\n--- Add Purchase Record ---\n";
    string customerId = readLine("Enter Customer ID: ");
    string customerName = readLine("Enter Customer Name: ");
    string productName = readLine("Enter Product Name: ");
    string category = readLine("Enter Category: ");
    int quantity = readInt("Enter Quantity: ", 1);
    double price = readDouble("Enter Price: ", 0.0);
    string date = readLine("Enter Date (YYYY-MM-DD): ");

    records.push_back(Purchase(customerId, customerName, productName, category, quantity, price, date));
    cout << "\nPurchase record added successfully.\n";
}

void viewPurchases() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }

    cout << "\n========== All Purchase Records ==========\n";
    for (const auto& p : records) p.print();
}

void searchByCustomer() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }
    string id = readLine("\nEnter Customer ID to search: ");
    vector<Purchase> matches = Analyzer::searchByCustomerId(records, id);

    if (matches.empty()) {
        cout << "\nNo records found for Customer ID: " << id << "\n";
        return;
    }

    cout << "\n========== Records for " << id << " ==========\n";
    for (const auto& p : matches) p.print();
}

void editRecord() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }

    viewPurchases();
    int index = readInt("\nEnter the record number to edit (1-" + to_string(records.size()) + "): ", 1);
    if (index > (int)records.size()) {
        cout << "\nInvalid record number.\n";
        return;
    }

    Purchase& p = records[index - 1];
    cout << "\nEnter new values for this record:\n";

    p.setProductName(readLine("New Product Name: "));
    p.setCategory(readLine("New Category: "));
    p.setQuantity(readInt("New Quantity: ", 1));
    p.setPrice(readDouble("New Price: ", 0.0));
    p.setDate(readLine("New Date (YYYY-MM-DD): "));

    cout << "\nRecord updated successfully.\n";
}

void deleteRecord() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }

    viewPurchases();
    int index = readInt("\nEnter the record number to delete (1-" + to_string(records.size()) + "): ", 1);
    if (index > (int)records.size()) {
        cout << "\nInvalid record number.\n";
        return;
    }

    records.erase(records.begin() + (index - 1));
    cout << "\nRecord deleted successfully.\n";
}

void totalRevenue() {
    cout << "\nTotal Revenue: Rs. " << Analyzer::totalRevenue(records) << endl;
}

void topSellingProduct() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }
    auto result = Analyzer::topSellingProduct(records);
    cout << "\nTop Selling Product: " << result.first << endl;
    cout << "Total Quantity Sold: " << result.second << endl;
}

void highestSpendingCustomer() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }
    auto result = Analyzer::highestSpendingCustomer(records);
    cout << "\nHighest Spending Customer: " << get<0>(result) << endl;
    cout << "Customer ID: " << get<1>(result) << endl;
    cout << "Total Spending: Rs. " << get<2>(result) << endl;
}

void categorySalesMenu() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }
    cout << "\n====== Category Wise Sales ======\n";
    for (const auto& entry : Analyzer::categorySales(records)) {
        cout << entry.first << " : Rs. " << entry.second << endl;
    }
}

void recommendProductMenu() {
    if (records.size() < 2) {
        cout << "\nNot enough data for recommendation.\n";
        return;
    }
    string product = readLine("\nEnter product name: ");
    string recommended = Analyzer::recommendProduct(records, product);

    if (recommended.empty()) {
        cout << "\nNo recommendation found.\n";
    } else {
        cout << "\nCustomers who bought " << product << " also bought " << recommended << endl;
    }
}

void businessSummary() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }
    string report = Analyzer::generateSummaryReport(records);
    cout << "\n" << report << endl;

    string choice = readLine("Export this report to summary-report.txt? (y/n): ");
    if (!choice.empty() && (choice[0] == 'y' || choice[0] == 'Y')) {
        if (FileHandler::exportReport("summary-report.txt", report)) {
            cout << "Report exported to summary-report.txt\n";
        } else {
            cout << "Failed to export report.\n";
        }
    }
}

void saveRecords() {
    if (FileHandler::saveRecords(DATA_FILE, records)) {
        cout << "\nRecords saved to " << DATA_FILE << ".\n";
    } else {
        cout << "\nFailed to save records.\n";
    }
}

void printMenu() {
    cout << "\n=============================================\n";
    cout << "   Smart Customer Purchase Pattern Analyzer\n";
    cout << "         Store: Prem Supermart\n";
    cout << "=============================================\n";
    cout << " 1. Add Purchase Record\n";
    cout << " 2. View All Records\n";
    cout << " 3. Search Records by Customer ID\n";
    cout << " 4. Edit a Record\n";
    cout << " 5. Delete a Record\n";
    cout << " 6. Total Revenue\n";
    cout << " 7. Top Selling Product\n";
    cout << " 8. Highest Spending Customer\n";
    cout << " 9. Category Wise Sales\n";
    cout << "10. Product Recommendation\n";
    cout << "11. Business Summary Report (view + export)\n";
    cout << "12. Save Records\n";
    cout << "13. Exit\n";
}

int main() {
    records = FileHandler::loadRecords(DATA_FILE);
    cout << "Loaded " << records.size() << " record(s) from " << DATA_FILE << ".\n";

    int choice;
    do {
        printMenu();
        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:  addPurchase(); break;
            case 2:  viewPurchases(); break;
            case 3:  searchByCustomer(); break;
            case 4:  editRecord(); break;
            case 5:  deleteRecord(); break;
            case 6:  totalRevenue(); break;
            case 7:  topSellingProduct(); break;
            case 8:  highestSpendingCustomer(); break;
            case 9:  categorySalesMenu(); break;
            case 10: recommendProductMenu(); break;
            case 11: businessSummary(); break;
            case 12: saveRecords(); break;
            case 13:
                saveRecords();
                cout << "\nExiting... Thank you for using Prem Supermart's analyzer.\n";
                break;
            default:
                cout << "\nInvalid choice. Please select 1-13.\n";
        }
    } while (choice != 13);

    return 0;
}

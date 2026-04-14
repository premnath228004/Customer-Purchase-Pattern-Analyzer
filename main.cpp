#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct Purchase {
    string customerId;
    string customerName;
    string productName;
    string category;
    int quantity;
    float price;
    string date;
};

vector<Purchase> records;

void addPurchase() {
    Purchase p;

    cout << "\nEnter Customer ID: ";
    cin >> p.customerId;
    cin.ignore();

    cout << "Enter Customer Name: ";
    getline(cin, p.customerName);

    cout << "Enter Product Name: ";
    getline(cin, p.productName);

    cout << "Enter Category: ";
    getline(cin, p.category);

    cout << "Enter Quantity: ";
    cin >> p.quantity;

    cout << "Enter Price: ";
    cin >> p.price;
    cin.ignore();

    cout << "Enter Date: ";
    getline(cin, p.date);

    records.push_back(p);
    cout << "\nPurchase record added successfully.\n";
}

void viewPurchases() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }

    cout << "\n========== All Purchase Records ==========\n";
    for (const auto& p : records) {
        cout << "Customer ID   : " << p.customerId << endl;
        cout << "Customer Name : " << p.customerName << endl;
        cout << "Product Name  : " << p.productName << endl;
        cout << "Category      : " << p.category << endl;
        cout << "Quantity      : " << p.quantity << endl;
        cout << "Price         : " << p.price << endl;
        cout << "Date          : " << p.date << endl;
        cout << "------------------------------------------\n";
    }
}

void saveToFile() {
    ofstream file("data.txt");
    for (const auto& p : records) {
        file << p.customerId << ","
             << p.customerName << ","
             << p.productName << ","
             << p.category << ","
             << p.quantity << ","
             << p.price << ","
             << p.date << "\n";
    }
    file.close();
    cout << "\nRecords saved to file.\n";
}

void loadFromFile() {
    ifstream file("data.txt");
    if (!file) return;

    records.clear();
    string line;

    while (getline(file, line)) {
        Purchase p;
        string parts[7];
        int i = 0;
        size_t pos;

        while ((pos = line.find(',')) != string::npos && i < 6) {
            parts[i++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        parts[i] = line;

        p.customerId = parts[0];
        p.customerName = parts[1];
        p.productName = parts[2];
        p.category = parts[3];
        p.quantity = stoi(parts[4]);
        p.price = stof(parts[5]);
        p.date = parts[6];

        records.push_back(p);
    }

    file.close();
}

void totalRevenue() {
    float revenue = 0;

    for (const auto& p : records) {
        revenue += p.quantity * p.price;
    }

    cout << "\nTotal Revenue: Rs. " << revenue << endl;
}

void topSellingProduct() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }

    string topProduct;
    int maxQuantity = 0;

    for (int i = 0; i < records.size(); i++) {
        int totalQty = 0;

        for (int j = 0; j < records.size(); j++) {
            if (records[i].productName == records[j].productName) {
                totalQty += records[j].quantity;
            }
        }

        if (totalQty > maxQuantity) {
            maxQuantity = totalQty;
            topProduct = records[i].productName;
        }
    }

    cout << "\nTop Selling Product: " << topProduct << endl;
    cout << "Total Quantity Sold: " << maxQuantity << endl;
}

void highestSpendingCustomer() {
    if (records.empty()) {
        cout << "\nNo records found.\n";
        return;
    }

    string topCustomer;
    string topCustomerId;
    float maxSpent = 0;

    for (int i = 0; i < records.size(); i++) {
        float totalSpent = 0;

        for (int j = 0; j < records.size(); j++) {
            if (records[i].customerId == records[j].customerId) {
                totalSpent += records[j].quantity * records[j].price;
            }
        }

        if (totalSpent > maxSpent) {
            maxSpent = totalSpent;
            topCustomer = records[i].customerName;
            topCustomerId = records[i].customerId;
        }
    }

    cout << "\nHighest Spending Customer: " << topCustomer << endl;
    cout << "Customer ID: " << topCustomerId << endl;
    cout << "Total Spending: Rs. " << maxSpent << endl;
}
void categorySales() {

    string categories[] = {"Food","Dairy","Snacks","Beverage","PersonalCare","Stationery"};
    float sales[6] = {0};

    for (const auto &p : records) {

        for(int i=0;i<6;i++) {

            if(p.category == categories[i]) {

                sales[i] += p.quantity * p.price;

            }
        }
    }

    cout << "\n====== Category Wise Sales ======\n";

    for(int i=0;i<6;i++) {

        cout << categories[i] << " : Rs " << sales[i] << endl;

    }
}
void recommendProduct() {

    if(records.size() < 2) {
        cout << "\nNot enough data for recommendation.\n";
        return;
    }

    string product;
    cout << "\nEnter product name: ";
    cin.ignore();
    getline(cin, product);

    string recommended = "";
    int maxCount = 0;

    for(int i=0;i<records.size();i++) {

        if(records[i].productName == product) {

            for(int j=0;j<records.size();j++) {

                if(records[i].customerId == records[j].customerId &&
                   records[j].productName != product) {

                    int count = 0;

                    for(int k=0;k<records.size();k++) {

                        if(records[k].productName == records[j].productName)
                            count++;
                    }

                    if(count > maxCount) {
                        maxCount = count;
                        recommended = records[j].productName;
                    }
                }
            }
        }
    }

    if(recommended == "")
        cout << "\nNo recommendation found.\n";
    else
        cout << "\nCustomers who bought " << product
             << " also bought " << recommended << endl;
}
int main() {
    loadFromFile();

    int choice;
    do {
        cout << "=============================================\n";
        cout << "   Smart Customer Purchase Pattern Analyzer\n";
        cout << "         Store: Prem Supermart\n";
        cout << "=============================================\n";
        cout << "1. Add Purchase Record\n";
        cout << "2. View All Records\n";
        cout << "3. Total Revenue\n";
        cout << "4. Top Selling Product\n";
        cout << "5. Highest Spending Customer\n";
        cout << "6. Category Wise Sales \n";
         cout << "7. Product Recommendation \n";
        cout << "8. Save Records\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addPurchase();
                break;
            case 2:
                viewPurchases();
                break;
            case 3:
                totalRevenue();
                break;
            case 4:
                topSellingProduct();
                break;
            case 5:
                highestSpendingCustomer();
                break;
            case 6:
                categorySales();
                break;

            case 7:
                recommendProduct();
                break;


            case 8:
                saveToFile();
                break;
        
            
            case 9:
                saveToFile();
                break;

            case 10:
                saveToFile();
                cout<<"Exiting...\n";
                break;
            default:
                cout << "\nInvalid choice.\n";
        }
    } while (choice != 8);

    return 0;
}
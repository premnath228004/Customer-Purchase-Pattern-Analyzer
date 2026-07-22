#include "Filehandler.h"
#include <fstream>
#include <iostream>

vector<Purchase> FileHandler::loadRecords(const string& filename) {
    vector<Purchase> records;
    ifstream file(filename);
    if (!file.is_open()) return records;

    string line;
    int skipped = 0;
    while (getline(file, line)) {
        if (line.empty() || line == "\r") continue;

        bool ok = false;
        Purchase p = Purchase::fromCSV(line, ok);
        if (ok) {
            records.push_back(p);
        } else {
            skipped++;
        }
    }

    file.close();
    if (skipped > 0) {
        cout << "Note: " << skipped << " corrupted line(s) in " << filename << " were skipped.\n";
    }
    return records;
}

bool FileHandler::saveRecords(const string& filename, const vector<Purchase>& records) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    for (const auto& p : records) {
        file << p.toCSV() << "\n";
    }
    file.close();
    return true;
}

bool FileHandler::exportReport(const string& filename, const string& content) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << content;
    file.close();
    return true;
}

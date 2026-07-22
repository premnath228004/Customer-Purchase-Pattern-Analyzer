#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <vector>
#include <string>
#include "Purchase.h"
using namespace std;

// Handles all disk I/O: loading/saving purchase records and exporting
// generated text reports.
class FileHandler {
public:
    static vector<Purchase> loadRecords(const string& filename);
    static bool saveRecords(const string& filename, const vector<Purchase>& records);
    static bool exportReport(const string& filename, const string& content);
};

#endif

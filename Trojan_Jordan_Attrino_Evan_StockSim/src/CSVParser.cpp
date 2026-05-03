#include "CSVParser.h"
#include <string>
#include <fstream>
#include <vector>
using namespace std;

// Static utility class for loading Yahoo Finance historical CSV files.
// Reads the header row to detect column order (handles NVDA's non-standard layout).
// Only loads rows with dates in [2000-01-01, 2020-01-01].
// Returns a heap-allocated PriceHistory* (caller owns it).
// Returns nullptr if the file cannot be opened.
PriceHistory* CSVParser::loadHistory(const string& filename) {
    PriceHistory* history = new PriceHistory();
    ifstream file(filename);
    if (!file.is_open()) {
        delete history;
        return nullptr;
    }

    string line;
    getline(file, line);  // header row

    // detect column indices from header
    int idxDate = -1, idxOpen = -1, idxHigh = -1, idxLow = -1, idxClose = -1, idxVolume = -1;
    {
        int col = 0;
        size_t pos = 0;
        while (pos <= line.size()) {
            size_t comma = line.find(',', pos);
            string name = (comma == string::npos)
                ? line.substr(pos)
                : line.substr(pos, comma - pos);

            if      (name == "Date")   idxDate   = col;
            else if (name == "Open")   idxOpen   = col;
            else if (name == "High")   idxHigh   = col;
            else if (name == "Low")    idxLow    = col;
            else if (name == "Close")  idxClose  = col;
            else if (name == "Volume") idxVolume = col;

            ++col;
            if (comma == string::npos) break;
            pos = comma + 1;
        }
    }

    // fallback to standard Yahoo order if header parsing failed
    if (idxDate   < 0) idxDate   = 0;
    if (idxOpen   < 0) idxOpen   = 1;
    if (idxHigh   < 0) idxHigh   = 2;
    if (idxLow    < 0) idxLow    = 3;
    if (idxClose  < 0) idxClose  = 4;
    if (idxVolume < 0) idxVolume = 6;

    while (getline(file, line)) {
        try {
            // split line into fields
            vector<string> fields;
            size_t pos = 0;
            while (pos <= line.size()) {
                size_t comma = line.find(',', pos);
                if (comma == string::npos) {
                    fields.push_back(line.substr(pos));
                    break;
                }
                fields.push_back(line.substr(pos, comma - pos));
                pos = comma + 1;
            }

            string date = fields[idxDate];
            if (!dateInRange(date, "2000-01-01", "2020-01-01")) continue;

            double open   = stod(fields[idxOpen]);
            double high   = stod(fields[idxHigh]);
            double low    = stod(fields[idxLow]);
            double close  = stod(fields[idxClose]);
            long long volume = stoll(fields[idxVolume]);

            history->append(date, open, high, low, close, volume);
        } catch (...) {
            continue;
        }
    }
    return history;
}

// Returns true if 'date' falls within [startDate, endDate] (inclusive).
// Works on "YYYY-MM-DD" strings: lexicographic == chronological.
bool CSVParser::dateInRange(const string& date, const string& start, const string& end) {
    return date >= start && date <= end;
}

// Extracts the 4-digit year from "YYYY-MM-DD" and returns it as an int.
int CSVParser::extractYear(const string& date) {
    string SYear = date.substr(0, 4);
    return stoi(SYear);
}

// Extracts the 2-digit month from "YYYY-MM-DD" and returns it as an int.
int CSVParser::extractMonth(const string& date) {
    string SDate = date.substr(5, 2);
    return stoi(SDate);
}

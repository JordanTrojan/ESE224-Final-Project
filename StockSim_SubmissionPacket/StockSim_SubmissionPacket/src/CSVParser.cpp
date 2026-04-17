#include "CSVParser.h"
#include <string>
#include <fstream>
using namespace std;

// Static utility class for loading Yahoo Finance historical CSV files.
// Yahoo Finance CSV format (header row):
//   Date, Open, High, Low, Close, Adj Close, Volume
// This parser uses the "Close" column as the closing price.
// Dates are formatted "YYYY-MM-DD".
// Opens the CSV file at 'filename', skips the header row, and builds
// a PriceHistory linked list from each valid data row.
// Returns a heap-allocated PriceHistory* (caller owns it).
// Returns nullptr if the file cannot be opened.
PriceHistory* CSVParser::loadHistory(const string& filename) {
    PriceHistory* history = new PriceHistory();
    ifstream file(filename);
    if(!file.is_open()) {
        delete history;
        return nullptr;
    }
    string line;
    // Skip header
    getline(file, line);
    while(getline(file,line)) {
        try {
            string date;
            double open, high, low, close;
            long volume;
            size_t pos = 0;
            // Date
            pos = line.find(',');
            date = line.substr(0, pos);
            line.erase(0, pos + 1);
            // Open
            pos = line.find(',');
            open = stod(line.substr(0, pos));
            line.erase(0, pos + 1);
            // hgih
            pos = line.find(',');
            high = stod(line.substr(0,pos));
            line.erase(0, pos+1);
            // Low
            pos = line.find(',');
            low = stod(line.substr(0, pos));
            line.erase(0, pos+1);
            // Close
            pos = line.find(',');
            close = stod(line.substr(0, pos));
            line.erase(0, pos+1);
            // Adj Close — skip it
            pos = line.find(',');
            line.erase(0, pos + 1);
            //volume
            volume = stol(line);

            history->append(date, open, high, low, close, volume);
        } catch (...) {
            // Skip malformed lines
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

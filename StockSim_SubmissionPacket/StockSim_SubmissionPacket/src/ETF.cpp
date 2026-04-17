#include "ETF.h"
#include "CSVParser.h"
#include <iostream>
#include <cmath>

using namespace std;

ETF::ETF(const string& ticker, const string& name, const string& sector, double expenseRatio) :
Stock(ticker, name, sector), expenseRatio(expenseRatio) {
}

ETF::~ETF() {
}

double ETF::getExpenseRatio () const {
    return expenseRatio;
}

void ETF::setExpenseRatio (double ratio) {
    expenseRatio = ratio;
}

double ETF::calculate10YearCAGR() const {
    if (!history || history->getSize() == 0) {
        return 0.0;
    }

    // Get the most recent date's year
    double endPrice = history->getTail()->close;
    string endDate = history->getTail()->date;
    int endYear = CSVParser::extractYear(endDate);
    int startYear = endYear - 10;

    // walk forward until we find the target year
    double startPrice = 0.0;
    for (PriceHistory::ReverseIterator it = history->rbegin(); it != history->rend(); ++it) {
        if (CSVParser::extractYear((*it).date) == startYear) {
            startPrice = (*it).close;
            break;
        }
    }

    if (startPrice == 0.0) return 0.0;  // couldn't find 10 years back

    return (pow(endPrice / startPrice, 1.0 / 10.0) - 1.0) * 100.0;
}

void ETF::printSummary() const {
    Stock::printSummary();
    cout << "Expense Ratio: " << expenseRatio * 100.0 << "%" << endl;
    cout << "10-Year CAGR : " << calculate10YearCAGR() << "%" << endl;
}

string ETF::getType() const {
    return "ETF";
}

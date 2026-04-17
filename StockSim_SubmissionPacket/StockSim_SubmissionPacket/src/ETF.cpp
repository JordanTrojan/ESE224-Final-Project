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

}

void ETF::printSummary() const {
    Stock::printSummary();
    cout << "Expense Ratio: " << expenseRatio * 100.0 << "%" << endl;
    cout << "10-Year CAGR : " << calculate10YearCAGR() << "%" << endl;
}

string ETF::getType() const {
    return "ETF";
}

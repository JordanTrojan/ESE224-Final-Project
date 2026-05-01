/*
 * ESE 224 – Final Project: StockSim
 * Historical Market Analyzer & Trading Strategy Simulator
 *
 * Student Name : Jordan Trojan | Evan Attrino
 * Student ID   : 116147886 | 116141819
 *
 * Instructions:
 *   1. Implement all classes listed in the header files under include/.
 *   2. Create corresponding .cpp files in src/ for each header.
 *   3. Place your Yahoo Finance CSV files in data/ (spx.csv, AAPL.csv, TSLA.csv).
 *   4. Complete the menu handlers below — each case should call the relevant
 *      class methods you implemented.
 *   5. Do NOT use std::queue, std::stack, std::list, std::map, std::unordered_map,
 *      or any external library. std::vector, std::string, std::sort are allowed.
 *
 * Compile with C++11 or later:
 *   g++ -std=c++11 -Iinclude src/*.cpp main.cpp -o stocksim
 */

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <vector>

// Include all your headers here once implemented
#include "include/FinancialAsset.h"
#include "include/PriceNode.h"
#include "include/PriceHistory.h"
#include "include/CSVParser.h"
#include "include/Stock.h"
#include "include/ETF.h"
#include "include/CircularQueue.h"
#include "include/TradeStack.h"
#include "include/OrderQueue.h"
#include "include/StockBST.h"
#include "include/Portfolio.h"
#include "include/TradingStrategy.h"
#include "include/FixedSIPStrategy.h"
#include "include/DynamicSIPStrategy.h"
#include "include/GoldenCrossStrategy.h"
#include "include/MomentumStrategy.h"
#include "include/StockManager.h"

using namespace std;

// ---------------------------------------------------------------
// Forward declarations for menu handler functions
// ---------------------------------------------------------------
void menuLoadData(StockManager<ETF>& etfManager, StockManager<Stock>& stockManager);
void menuDisplayHistory(StockManager<Stock>& stockManager, StockManager<ETF>& etfManager);
void menuSearchByDate(StockManager<Stock>& stockManager, StockManager<ETF>& etfManager);
void menuBSTRangeSearch(StockBST& bst);
void menuInsertIntoBST(StockBST& bst, StockManager<Stock>& stockManager);
void menuDisplayBST(StockBST& bst);
void menuAddToPortfolio(Portfolio& portfolio);
void menuRemoveFromPortfolio(Portfolio& portfolio);
void menuQueueOrder(Portfolio& portfolio);
void menuExecuteOrder(Portfolio& portfolio);
void menuUndoTrade(Portfolio& portfolio);
void menuRunStrategy(StockManager<ETF>& etfManager, StockManager<Stock>& stockManager);
void menuCompareStrategies(StockManager<ETF>& etfManager);
void menuPortfolioSummary(Portfolio& portfolio);
void menuTradeHistory(Portfolio& portfolio);

// Bonus
void parameterSweep(ETF* spx, double monthlyCapital, int startYear, int endYear, StockBST& bst);

// ---------------------------------------------------------------
// Utility: print the main menu
// ---------------------------------------------------------------
void printMenu(const string& studentName, const string& studentID) {
    cout << "\n===== StockSim: Historical Market Analyzer =====\n";
    cout << "Student: " << studentName << "  |  ID: " << studentID << "\n";
    cout << "-------------------------------------------------\n";
    cout << " [1]  Load stock data from CSV\n";
    cout << " [2]  Display price history (linked list traversal)\n";
    cout << " [3]  Search by date range\n";
    cout << " [4]  Find stocks by return range (BST range query)\n";
    cout << " [5]  Insert stock into performance BST\n";
    cout << " [6]  Display BST (inorder / preorder / postorder)\n";
    cout << " [7]  Add stock to portfolio\n";
    cout << " [8]  Remove stock from portfolio\n";
    cout << " [9]  Queue a pending order\n";
    cout << "[10]  Execute next pending order\n";
    cout << "[11]  Undo last trade\n";
    cout << "[12]  Run strategy simulation\n";
    cout << "[13]  Compare all strategies head-to-head\n";
    cout << "[14]  Display portfolio summary\n";
    cout << "[15]  Display full trade history\n";
    cout << "[16]  Bonus: Parameter sweep\n";
    cout << " [0]  Exit\n";
    cout << "-------------------------------------------------\n";
    cout << "Enter choice: ";
}

// ---------------------------------------------------------------
// main
// ---------------------------------------------------------------
int main() {
    // --- Student login ---
    string studentName, studentID;
    cout << "========================================\n";
    cout << "  ESE 224 StockSim — Student Login\n";
    cout << "========================================\n";
    cout << "Enter your full name: ";
    getline(cin, studentName);
    cout << "Enter your student ID: ";
    getline(cin, studentID);
    cout << "\nWelcome, " << studentName << "!\n";

    // --- Initialize shared objects ---
    StockManager<ETF>   etfManager;
    StockManager<Stock> stockManager;
    StockBST            performanceBST;
    Portfolio           portfolio(studentName, 10000.0);  // start with $10,000 cash

    // --- Main menu loop ---
    int choice = -1;
    while (choice != 0) {
        printMenu(studentName, studentID);
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case  1: menuLoadData(etfManager, stockManager);                        break;
            case  2: menuDisplayHistory(stockManager, etfManager);                  break;
            case  3: menuSearchByDate(stockManager, etfManager);                    break;
            case  4: menuBSTRangeSearch(performanceBST);                            break;
            case  5: menuInsertIntoBST(performanceBST, stockManager);               break;
            case  6: menuDisplayBST(performanceBST);                                break;
            case  7: menuAddToPortfolio(portfolio);                                 break;
            case  8: menuRemoveFromPortfolio(portfolio);                            break;
            case  9: menuQueueOrder(portfolio);                                     break;
            case 10: menuExecuteOrder(portfolio);                                   break;
            case 11: menuUndoTrade(portfolio);                                      break;
            case 12: menuRunStrategy(etfManager, stockManager);                     break;
            case 13: menuCompareStrategies(etfManager);                             break;
            case 14: menuPortfolioSummary(portfolio);                               break;
            case 15: menuTradeHistory(portfolio);                                   break;
            case 16: {
                ETF* spx = etfManager.findByTicker("SPX");
                if (!spx) { cout << "SPX not loaded. Load it first." << endl; break; }
                double monthlyCapital; int startYear, endYear;
                cout << "Monthly capital: "; cin >> monthlyCapital;
                cout << "Start year: ";      cin >> startYear;
                cout << "End year: ";        cin >> endYear;
                StockBST sweepBST;
                parameterSweep(spx, monthlyCapital, startYear, endYear, sweepBST);
                break;
            }
            case  0: cout << "Goodbye, " << studentName << "!\n";                  break;
            default: cout << "Invalid choice. Please enter 0–15.\n";               break;
        }
    }

    return 0;
}

// ---------------------------------------------------------------
// Menu handler stubs — implement each one below
// ---------------------------------------------------------------

void menuLoadData(StockManager<ETF>& etfManager, StockManager<Stock>& stockManager) {
    string ticker;
    cout << "Enter ticker to load (SPX, AMZN, NVDA, or all): ";
    cin >> ticker;

    if (ticker == "SPX" || ticker == "all") {
        ETF* spx = new ETF("SPX", "S&P 500 Index", "Index", 0.0);
        if (spx->loadFromCSV("data/SPX.csv"))
            etfManager.addAsset(spx);
        else {
            cout << "Failed to load SPX.csv" << endl;
            delete spx;
        }
    }
    if (ticker == "AMZN" || ticker == "all") {
        Stock* amzn = new Stock("AMZN", "Amazon Inc.", "Technology");
        if (amzn->loadFromCSV("data/AMZN.csv"))
            stockManager.addAsset(amzn);
        else {
            cout << "Failed to load AMZN.csv" << endl;
            delete amzn;
        }
    }
    if (ticker == "NVDA" || ticker == "all") {
        Stock* nvda = new Stock("NVDA", "NVIDIA Corporation", "Technology");
        if (nvda->loadFromCSV("data/NVidia_stock_history.csv"))
            stockManager.addAsset(nvda);
        else {
            cout << "Failed to load NVidia_stock_history.csv" << endl;
            delete nvda;
        }
    }
    if (ticker != "SPX" && ticker != "AMZN" && ticker != "NVDA" && ticker != "all") {
        cout << "Unknown ticker: " << ticker << endl;
    }
}

void menuDisplayHistory(StockManager<Stock>& stockManager, StockManager<ETF>& etfManager) {
    string ticker;
    int limit;
    cout << "Enter ticker: "; cin >> ticker;
    cout << "How many records to display (0=all): "; cin >> limit;

    PriceHistory* history = nullptr;
    Stock* s = stockManager.findByTicker(ticker);
    if (s) history = s->getHistory();
    ETF*   e = etfManager.findByTicker(ticker);
    if (e) history = e->getHistory();

    if (!history) { cout << "Ticker not found." << endl; return; }

    int count = 0;
    for (PriceHistory::Iterator it = history->begin(); it != history->end(); ++it) {
        PriceNode& node = *it;
        cout << node.date  << " | "
             << "O: " << node.open  << " "
             << "H: " << node.high  << " "
             << "L: " << node.low   << " "
             << "C: " << node.close << " "
             << "V: " << node.volume << endl;
        if (++count == limit && limit != 0) break;
    }
    cout << count << " records displayed." << endl;
}

void menuSearchByDate(StockManager<Stock>& stockManager, StockManager<ETF>& etfManager) {
    string ticker, startDate, endDate;
    cout << "Enter ticker: ";    cin >> ticker;
    cout << "Start date (YYYY-MM-DD): "; cin >> startDate;
    cout << "End date   (YYYY-MM-DD): "; cin >> endDate;

    PriceHistory* history = nullptr;
    Stock* s = stockManager.findByTicker(ticker);
    if (s) history = s->getHistory();
    ETF*   e = etfManager.findByTicker(ticker);
    if (e) history = e->getHistory();

    if (!history) { cout << "Ticker not found." << endl; return; }
    history->printRange(startDate, endDate);
}

void menuBSTRangeSearch(StockBST& bst) {
    double low, high;
    cout << "Enter low return % : "; cin >> low;
    cout << "Enter high return %: "; cin >> high;

    vector<StockBST::BSTNode*> results;
    bst.rangeSearch(low, high, results);

    if (results.empty()) {
        cout << "No results found in range." << endl;
        return;
    }
    for (int i = 0; i < (int)results.size(); i++) {
        cout << results[i]->ticker << " | year=" << results[i]->year
             << " | return=" << results[i]->key << "%" << endl;
    }
}

void menuInsertIntoBST(StockBST& bst, StockManager<Stock>& stockManager) {
    string ticker; int year;
    cout << "Enter ticker: "; cin >> ticker;
    cout << "Enter year  : "; cin >> year;

    Stock* s = stockManager.findByTicker(ticker);
    if (!s) { cout << "Ticker not found." << endl; return; }

    double ret = s->calculateAnnualReturn(year);
    bst.insert(ticker, ret, year);
    cout << "Inserted " << ticker << " year=" << year
         << " return=" << ret << "%" << endl;
}

void menuDisplayBST(StockBST& bst) {
    int choice;
    cout << "Choose traversal:" << endl;
    cout << "  [1] Inorder" << endl;
    cout << "  [2] Preorder" << endl;
    cout << "  [3] Postorder" << endl;
    cout << "Choice: "; cin >> choice;

    if      (choice == 1) bst.inorder();
    else if (choice == 2) bst.preorder();
    else if (choice == 3) bst.postorder();
    else cout << "Invalid choice." << endl;
}

void menuAddToPortfolio(Portfolio& portfolio) {
    string ticker, date; int shares; double price;
    cout << "Ticker: ";          cin >> ticker;
    cout << "Shares: ";          cin >> shares;
    cout << "Price : ";          cin >> price;
    cout << "Date (YYYY-MM-DD): "; cin >> date;
    portfolio.buyShares(ticker, shares, price, date);
}

void menuRemoveFromPortfolio(Portfolio& portfolio) {
    string ticker, date; int shares; double price;
    cout << "Ticker: ";          cin >> ticker;
    cout << "Shares: ";          cin >> shares;
    cout << "Price : ";          cin >> price;
    cout << "Date (YYYY-MM-DD): "; cin >> date;
    portfolio.sellShares(ticker, shares, price, date);
}

void menuQueueOrder(Portfolio& portfolio) {
    Order o;
    cout << "Ticker: ";              cin >> o.ticker;
    cout << "Side (BUY/SELL): ";     cin >> o.side;
    cout << "Type (MARKET/LIMIT): "; cin >> o.type;
    cout << "Shares: ";              cin >> o.shares;
    if (o.type == "LIMIT") {
        cout << "Target price: ";    cin >> o.targetPrice;
    } else {
        o.targetPrice = 0.0;
    }
    cout << "Date (YYYY-MM-DD): ";   cin >> o.submittedDate;
    portfolio.queueOrder(o);
}

void menuExecuteOrder(Portfolio& portfolio) {
    double price; string date;
    cout << "Current market price: "; cin >> price;
    cout << "Date (YYYY-MM-DD): ";    cin >> date;
    portfolio.executeNextOrder(price, date);
}

void menuUndoTrade(Portfolio& portfolio) {
    portfolio.undoLastTrade();
}

void menuRunStrategy(StockManager<ETF>& etfManager, StockManager<Stock>& stockManager) {
    int choice;
    cout << "Choose strategy:" << endl;
    cout << "  [1] Fixed SIP" << endl;
    cout << "  [2] Dynamic SIP" << endl;
    cout << "  [3] Golden Cross" << endl;
    cout << "  [4] Momentum" << endl;
    cout << "Choice: "; cin >> choice;

    string ticker; double monthlyCapital; int startYear, endYear;
    cout << "Ticker: ";           cin >> ticker;
    cout << "Monthly capital: ";  cin >> monthlyCapital;
    cout << "Start year: ";       cin >> startYear;
    cout << "End year: ";         cin >> endYear;

    PriceHistory* history = nullptr;
    Stock* s = stockManager.findByTicker(ticker);
    if (s) history = s->getHistory();
    ETF*   e = etfManager.findByTicker(ticker);
    if (e) history = e->getHistory();
    if (!history) { cout << "Ticker not found." << endl; return; }

    TradingStrategy* strategy = nullptr;
    if (choice == 1) {
        strategy = new FixedSIPStrategy();
    } else if (choice == 2) {
        strategy = new DynamicSIPStrategy(50, 200, 5.0, 15.0, 2.0, 252);
    } else if (choice == 3) {
        strategy = new GoldenCrossStrategy(50, 200);
    } else if (choice == 4) {
        double threshold; int lookback;
        cout << "Momentum threshold %: "; cin >> threshold;
        cout << "Lookback days (126): ";  cin >> lookback;
        strategy = new MomentumStrategy(threshold, lookback);
    } else {
        cout << "Invalid choice." << endl; return;
    }

    SimResult r = strategy->backtest(history, monthlyCapital, startYear, endYear);
    strategy->printResult(r);
    delete strategy;
}

void menuCompareStrategies(StockManager<ETF>& etfManager) {
    ETF* spx = etfManager.findByTicker("SPX");
    if (!spx) { cout << "SPX not loaded. Load it first." << endl; return; }

    double monthlyCapital; int startYear, endYear;
    cout << "Monthly capital: "; cin >> monthlyCapital;
    cout << "Start year: ";      cin >> startYear;
    cout << "End year: ";        cin >> endYear;

    PriceHistory* history = spx->getHistory();

    TradingStrategy* strategies[4];
    strategies[0] = new FixedSIPStrategy();
    strategies[1] = new DynamicSIPStrategy(50, 200, 5.0, 15.0, 2.0, 252);
    strategies[2] = new GoldenCrossStrategy(50, 200);
    strategies[3] = new MomentumStrategy(5.0, 126);

    SimResult results[4];
    for (int i = 0; i < 4; i++)
        results[i] = strategies[i]->backtest(history, monthlyCapital, startYear, endYear);

    // comparison table
    cout << "\n" << string(90, '=') << endl;
    cout << left << setw(25) << "Strategy"
         << setw(14) << "Invested"
         << setw(14) << "Final Value"
         << setw(10) << "Return%"
         << setw(10) << "CAGR%"
         << setw(12) << "MaxDD%"
         << setw(8)  << "Trades" << endl;
    cout << string(90, '-') << endl;

    for (int i = 0; i < 4; i++) {
        cout << fixed << setprecision(2)
             << left << setw(25) << results[i].strategyName
             << setw(14) << results[i].totalInvested
             << setw(14) << results[i].finalValue
             << setw(10) << results[i].totalReturn
             << setw(10) << results[i].cagr
             << setw(12) << results[i].maxDrawdown
             << setw(8)  << results[i].totalTrades << endl;
    }
    cout << string(90, '=') << endl;

    for (int i = 0; i < 4; i++) delete strategies[i];
}

void menuPortfolioSummary(Portfolio& portfolio) {
    portfolio.printHoldings();
}

void menuTradeHistory(Portfolio& portfolio) {
    portfolio.printTradeHistory();
}

// ---------------------------------------------------------------
// BONUS: Parameter Sweep for DynamicSIPStrategy
// ---------------------------------------------------------------
void parameterSweep(ETF* spx, double monthlyCapital, int startYear, int endYear, StockBST& bst) {
    double mildDips[15] = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
    double severeDips[15]  = {10,11,12,13,14,15,17,19,21,23,25,27,29,31,35};
    double multipliers[15] = {1.0,1.25,1.5,1.75,2.0,2.25,2.5,2.75,3.0,3.25,3.5,3.75,4.0,4.25,4.5};

    int total = 0;
    int current = 0;

    //count valid combinitions first
 for (int i =0; i<15; i++) {
        for (int j = 0; j<15; j++) {
            for (int k =0;  k<15; k++) {
                if (mildDips[i] < severeDips[j]) {
                    total++;
                }
            }
        }
    }

    cout << "Running parameter sweep (" << total << "combinations)..." << endl;
    cout << "[";
    for (int i =0; i <50; i++) {
        cout << " ";
    }
    cout << "] 0%" << flush;



    double bestValue = -1.0;
    string bestParams = "";


  for (int i =0; i<15; i++) {
        for (int j = 0; j<15; j++) {
            for (int k =0;  k<15; k++) { 
                double mild = mildDips[i];
                double severe = severeDips[j];
                double mult = multipliers[k];

                if (mild >= severe) continue; //filter extraneous sweeps

                DynamicSIPStrategy strat(50, 200, mild, severe, mult, 252);
                SimResult r = strat.backtest(spx->getHistory(), monthlyCapital, startYear, endYear);

                string ticker = "m=" + to_string((int)mild) + "_s=" + to_string((int)severe) + "_x=" + to_string((int)(mult*100));
                bst.insert(ticker, r.finalValue,0);

                if (r.finalValue > bestValue) {
                    bestValue = r.finalValue;
                    bestParams = "mild=" + to_string((int)mild) + "% severe=" + to_string((int)severe) + "% mult=" + to_string(mult);
                }

                //update loading bar
                current++;
                int filled = (int)(50.0 * current / total);
                int pct    = (int)(100.0 * current / total);
                cout << "\r[";
                for (int b =0; b<50; b++) {
                    cout << (b<filled? "=" : " ");
                }
                cout << "]" << pct << "%  (" << current << "/" << total << ")" << flush;
            }
        }
    }
    cout << "\n\nRanked worst to best (BST inorder):" << endl;
    bst.inorder();
    cout << "\nOptimal parameters: " << bestParams << endl;
    cout << "Best final value  : $" << fixed << setprecision(2) << bestValue << endl;

}

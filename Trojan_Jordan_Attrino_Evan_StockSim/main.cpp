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
//g++ -O3 -std=c++17 -o stocksim.exe main.cpp src/*.cpp -Iinclude
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <vector>
// ===== REMOVE HERE (CSV sweep additions) =====
#include <algorithm>
#include <fstream>
#include <sstream>
// ===== END REMOVE HERE =====

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
// ===== REMOVE HERE (CSV sweep additions) =====
void parameterSweepCSV(ETF* spx, double monthlyCapital, int startYear, int endYear);
// ===== END REMOVE HERE =====

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
    cout << "  ESE 224 StockSim - Student Login\n";
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
            // ===== REMOVE HERE (CSV sweep additions) =====
            case 17: {
                ETF* spx = etfManager.findByTicker("SPX");
                if (!spx) { cout << "SPX not loaded. Load it first." << endl; break; }
                double monthlyCapital; int startYear, endYear;
                cout << "Monthly capital: "; cin >> monthlyCapital;
                cout << "Start year: ";      cin >> startYear;
                cout << "End year: ";        cin >> endYear;
                parameterSweepCSV(spx, monthlyCapital, startYear, endYear);
                break;
            }
            // ===== END REMOVE HERE =====
            case  0: cout << "Goodbye, " << studentName << "!\n";                  break;
            default: cout << "Invalid choice. Please enter 0–17.\n";               break;
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

    for (char& c : ticker) c = toupper(c);
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
        strategy = new DynamicSIPStrategy(25, 250, 3.5, 10.0, 0.5, 126);
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
    strategies[1] = new DynamicSIPStrategy(24, 234, 2.7, 18.75, 0.1, 63);
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

    // ================================================================
    // EDIT THESE ARRAYS to control what values are tested.
    // Add or remove values freely — the loop counts adjust automatically.
    // Rule: every mildDip must be < severeDip, shortWindow < longWindow.
    // ================================================================

    vector<int>    shortWindows     = { 22, 24, 26 };
    vector<int>    longWindows      = { 229, 234, 237 };
    vector<double> mildDips         = { 2.65, 2.75, 2.85 };
    vector<double> severeDips       = { 17.0, 18.0, 19.0 };
    vector<double> multipliers      = { 0.05, 0.1, 0.25 };
    vector<int>    lookbacks        = { 50, 63 };
    vector<double> rallyThresholds  = { 0.0, 20.0 };
    vector<double> rallyMultipliers = { 0.1, 0.5 };

    // ================================================================

    int nSW = (int)shortWindows.size(),     nLW = (int)longWindows.size();
    int nMD = (int)mildDips.size(),         nSD = (int)severeDips.size();
    int nMU = (int)multipliers.size(),      nLB = (int)lookbacks.size();
    int nRT = (int)rallyThresholds.size(),  nRM = (int)rallyMultipliers.size();

    long long total = 0;
    for (int a=0;a<nSW;a++) for (int b=0;b<nLW;b++) for (int i=0;i<nMD;i++)
        for (int j=0;j<nSD;j++) for (int k=0;k<nMU;k++) for (int L=0;L<nLB;L++)
            for (int ri=0;ri<nRT;ri++) for (int rmi=0;rmi<nRM;rmi++)
                if (shortWindows[a] < longWindows[b] && mildDips[i] < severeDips[j]) total++;

    cout << "Running parameter sweep (" << total << " combinations)..." << endl;
    cout << "["; for (int i=0;i<50;i++) cout<<" "; cout<<"] 0%" << flush;

    long long current   = 0;
    double bestValue = -1.0;
    string bestParams;

    for (int a=0;a<nSW;a++) {
        for (int b=0;b<nLW;b++) {
            if (shortWindows[a] >= longWindows[b]) continue;
            for (int i=0;i<nMD;i++) {
                for (int j=0;j<nSD;j++) {
                    if (mildDips[i] >= severeDips[j]) continue;
                    for (int k=0;k<nMU;k++) {
                        for (int L=0;L<nLB;L++) {
                            for (int ri=0;ri<nRT;ri++) {
                                for (int rmi=0;rmi<nRM;rmi++) {
                                    int    sw   = shortWindows[a];
                                    int    lw   = longWindows[b];
                                    double mild = mildDips[i];
                                    double severe = severeDips[j];
                                    double mult = multipliers[k];
                                    int    lb   = lookbacks[L];
                                    double rt   = rallyThresholds[ri];
                                    double rmt  = rallyMultipliers[rmi];

                                    DynamicSIPStrategy strat(sw, lw, mild, severe, mult, lb, rt, rmt);
                                    SimResult res = strat.backtest(spx->getHistory(), monthlyCapital, startYear, endYear);

                                    string ticker = "sw=" + to_string(sw) + "_lw=" + to_string(lw) +
                                                    "_m=" + to_string(mild) + "_s=" + to_string(severe) +
                                                    "_x=" + to_string(mult) + "_lb=" + to_string(lb) +
                                                    "_rt=" + to_string(rt) + "_rm=" + to_string(rmt);
                                    bst.insert(ticker, res.finalValue, 0);

                                    if (res.finalValue > bestValue) {
                                        bestValue  = res.finalValue;
                                        bestParams = "sw=" + to_string(sw) + " lw=" + to_string(lw) +
                                                     " mild=" + to_string(mild) + "%" +
                                                     " severe=" + to_string(severe) + "%" +
                                                     " mult=" + to_string(mult) +
                                                     " lookback=" + to_string(lb) +
                                                     " rallyThr=" + to_string(rt) +
                                                     " rallyMult=" + to_string(rmt);
                                    }

                                    current++;
                                    int filled = (int)(50.0 * current / total);
                                    int pct    = (int)(100.0 * current / total);
                                    cout << "\r[";
                                    for (int box=0;box<50;box++) cout << (box < filled ? "=" : " ");
                                    cout << "] " << pct << "%  (" << current << "/" << total << ")" << flush;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    cout << "\n\nRanked worst to best (BST inorder):" << endl;
    bst.inorder();
    StockBST::BSTNode* maxNode = bst.findMax();
    if (maxNode)
        cout << "\n\nBST max: " << maxNode->ticker << " ($" << fixed << setprecision(2) << maxNode->key << ")\n";
    cout << "\nOptimal parameters: " << bestParams << endl;
    cout << "Best final value  : $" << fixed << setprecision(2) << bestValue << endl;
}

// ===== REMOVE HERE (CSV sweep additions) =====
void parameterSweepCSV(ETF* spx, double monthlyCapital, int startYear, int endYear) {
    // Optimised: flat contiguous array (no linked-list pointer chasing), O(1) moving
    // averages via ring buffer with running sum, O(1) amortised sliding-window max/min
    // via manual monotone-queue arrays, optional OpenMP multi-threading, per-thread
    // local top-100 lists (zero lock contention in the hot path).
    // Compile with: g++ -O3 -std=c++17 -fopenmp ...  to enable multi-threading.

    // Previous fine-grained sweep (known-optimal neighbourhood — uncomment to re-run):
    /*
    vector<int>    shortWindows     = { 22, 23, 24, 25, 26};
    vector<int>    longWindows      = { 225, 229, 230, 231, 233, 234, 235, 236, 237};
    vector<double> mildDips         = { 2.6, 2.65, 2.7, 2.75, 2.8, 2.85, 2.9};
    vector<double> severeDips       = { 16, 16.5, 17, 17.25, 17.5, 17.75, 18, 18.25, 18.5, 18.75, 19 };
    vector<double> multipliers      = { 0.05, .075, 0.1, .125, 0.25 };
    vector<int>    lookbacks        = { 42, 50, 55, 57, 60, 63, 65, 70};
    vector<double> rallyThresholds  = { 0.0, 15.0, 20.0, 25.0 };
    vector<double> rallyMultipliers = { 0.1, 0.5 };
    */

    vector<int>    shortWindows     = { 5, 8, 10, 12, 15, 18, 20, 22, 24, 25, 28, 30, 35, 40, 50, 60, 75, 100 };
    vector<int>    longWindows      = { 110, 125, 150, 175, 200, 215, 225, 235, 250, 275, 300, 325, 350, 400, 450, 500, 600, 750 };
    vector<double> mildDips         = { 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 12.0 };
    vector<double> severeDips       = { 13.0, 15.0, 17.0, 18.0, 20.0, 22.0, 25.0, 28.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 70.0 };
    vector<double> multipliers      = { 0.05, 0.1, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0 };
    vector<int>    lookbacks        = { 21, 42, 63, 90, 126, 175, 252 };
    vector<double> rallyThresholds  = { 0.0, 10.0, 20.0, 30.0 };
    vector<double> rallyMultipliers = { 0.1, 0.5 };

    int nSW = (int)shortWindows.size(),    nLW = (int)longWindows.size();
    int nMD = (int)mildDips.size(),        nSD = (int)severeDips.size();
    int nMU = (int)multipliers.size(),     nLB = (int)lookbacks.size();
    int nRT = (int)rallyThresholds.size(), nRM = (int)rallyMultipliers.size();
    long long total = (long long)nSW * nLW * nMD * nSD * nMU * nLB * nRT * nRM;

    // Pre-flatten the linked list once into a contiguous array with pre-computed year/month.
    // This eliminates pointer chasing and string parsing inside the hot loop.
    struct FlatBar { int year, month; double close; };
    vector<FlatBar> bars;
    bars.reserve(spx->getHistory()->getSize());
    for (PriceHistory::Iterator it = spx->getHistory()->begin(); it != spx->getHistory()->end(); ++it) {
        PriceNode& nd = *it;
        bars.push_back({CSVParser::extractYear(nd.date), CSVParser::extractMonth(nd.date), nd.close});
    }

    cout << "Running CSV sweep (" << total << " combinations)..." << endl;
    cout << "["; for (int i = 0; i < 50; i++) cout << " "; cout << "] 0%" << flush;

    // Fast backtest lambda — pure function, no captures, thread-safe.
    // Uses O(1) ring-buffer MAs and O(1) amortised monotone-queue sliding window.
    // Stack-allocated buffers (no heap allocs in the hot path).
    // Max sw=100, max lw=750, max lb=252 — sizes hard-coded to match parameter arrays above.
    auto fastBacktest = [](
        const vector<FlatBar>& data,
        int sw, int lw, double mildDip, double severeDip,
        double mul, int lb, double rallyThr, double rallyMul,
        double monthlyCap, int sy, int ey) -> SimResult
    {
        SimResult r; r.strategyName=""; r.finalValue=0; r.totalInvested=0;
        r.totalReturn=0; r.cagr=0; r.maxDrawdown=0; r.totalTrades=0;
        if (data.empty()) return r;

        // Ring buffers with running sum — O(1) moving average, no heap allocation
        double sbuf[101], lbuf[751];   // sw<=100, lw<=750
        int sh=0, lh=0, sc=0, lc=0;
        double ss=0.0, ls=0.0;

        double ps=0.0, pl=0.0, cs=0.0, cl=0.0;
        bool prevOk=false, inMkt=false;
        double modDip = (mildDip + severeDip) / 2.0;
        double frac=0.0, cash=0.0;

        // Monotone-queue sliding window max/min — O(1) amortised, no <deque> needed
        struct MQE { double val; int idx; };
        MQE qHi[512], qLo[512];       // lb<=252, so queue depth < 512 always
        int qHiH=0, qHiT=0, qLoH=0, qLoT=0;

        int dayIdx=0, lm=-1, ly=-1;
        double lastC=0.0, peak=0.0;

        for (int i = 0; i < (int)data.size(); i++) {
            double c = data[i].close;
            int y = data[i].year, m = data[i].month;

            // Short MA — O(1)
            if (sc == sw) ss -= sbuf[sh];
            sbuf[sh] = c; ss += c; sh = (sh+1)%sw; if (sc<sw) sc++;

            // Long MA — O(1)
            if (lc == lw) ls -= lbuf[lh];
            lbuf[lh] = c; ls += c; lh = (lh+1)%lw; if (lc<lw) lc++;

            // Sliding high
            while (qHiH<qHiT && qHi[qHiH%512].idx < dayIdx-lb) qHiH++;
            while (qHiH<qHiT && qHi[(qHiT-1)%512].val <= c)     qHiT--;
            qHi[qHiT%512]={c,dayIdx}; qHiT++;

            // Sliding low
            while (qLoH<qLoT && qLo[qLoH%512].idx < dayIdx-lb) qLoH++;
            while (qLoH<qLoT && qLo[(qLoT-1)%512].val >= c)     qLoT--;
            qLo[qLoT%512]={c,dayIdx}; qLoT++;

            dayIdx++;
            if (y < sy) { lm=m; ly=y; continue; }
            if (y > ey) break;

            if (m!=lm || y!=ly) { cash+=monthlyCap; r.totalInvested+=monthlyCap; lm=m; ly=y; }

            if (sc==sw && lc==lw) {
                cs=ss/sw; cl=ls/lw;
                if (prevOk) {
                    if (ps<pl && cs>cl) {
                        inMkt=true;
                        if (cash>0.0) { frac+=cash/c; cash=0.0; r.totalTrades++; }
                    } else if (ps>pl && cs<cl) {
                        inMkt=false;
                        if (frac>0.0) { cash+=frac*c; frac=0.0; r.totalTrades++; }
                    }
                }
                if (cash>0.0) {
                    double hi   = qHi[qHiH%512].val, lo = qLo[qLoH%512].val;
                    double drop = (hi>0.0) ? (hi-c)/hi*100.0 : 0.0;
                    double rise = (lo>0.0) ? (c-lo)/lo*100.0  : 0.0;
                    double buy  = 0.0;
                    if      (drop>=severeDip)                    buy=cash;
                    else if (inMkt) {
                        if      (drop>=modDip)                   buy=mul*2.0*monthlyCap;
                        else if (drop>=mildDip)                  buy=mul*monthlyCap;
                        else if (rallyThr>0.0&&rise>=rallyThr)  buy=rallyMul*monthlyCap;
                        else                                      buy=monthlyCap;
                    }
                    if (buy>cash) buy=cash;
                    if (buy>0.0) { frac+=buy/c; cash-=buy; r.totalTrades++; }
                }
                ps=cs; pl=cl; prevOk=true;
            }
            lastC=c;
            double pv=frac*c+cash;
            if (pv>peak) peak=pv;
            else if (peak>0.0) { double dd=(peak-pv)/peak*100.0; if(dd>r.maxDrawdown) r.maxDrawdown=dd; }
        }

        r.finalValue  = frac*lastC+cash;
        r.totalReturn = (r.totalInvested>0.0)
            ? (r.finalValue-r.totalInvested)/r.totalInvested*100.0 : 0.0;
        int yrs = ey-sy;
        r.cagr = (yrs>0 && r.totalInvested>0.0)
            ? (pow(r.finalValue/r.totalInvested, 1.0/yrs)-1.0)*100.0 : 0.0;
        return r;
    };

    // Parallel sweep: each outer-a iteration is a chunk for one thread.
    // Each thread builds its own localTop — no locks in the hot path.
    // After each chunk the critical section merges results, updates the progress
    // bar, and flushes the current top-100 to CSV (crash-safe).
    vector<pair<double,string>> top100;
    top100.reserve(101);
    long long current = 0;
    double bestValue  = -1.0;
    string bestParams;

    #pragma omp parallel for schedule(dynamic, 1)
    for (int a = 0; a < nSW; a++) {
        vector<pair<double,string>> localTop;
        localTop.reserve(101);
        long long localCount = 0;
        double    localBest  = -1.0;
        string    localBestP;

        for (int b=0; b<nLW; b++) {
          for (int i=0; i<nMD; i++) {
            for (int j=0; j<nSD; j++) {
              for (int k=0; k<nMU; k++) {
                for (int L=0; L<nLB; L++) {
                  for (int ri=0; ri<nRT; ri++) {
                    for (int rmi=0; rmi<nRM; rmi++) {
                        int    sw  = shortWindows[a], lw  = longWindows[b];
                        double mild= mildDips[i],     sev = severeDips[j];
                        double mul = multipliers[k];
                        int    lb  = lookbacks[L];
                        double rt  = rallyThresholds[ri], rmt = rallyMultipliers[rmi];

                        SimResult res = fastBacktest(bars, sw, lw, mild, sev,
                                                     mul, lb, rt, rmt,
                                                     monthlyCapital, startYear, endYear);

                        if (res.finalValue > localBest) {
                            localBest = res.finalValue;
                            char buf[256];
                            snprintf(buf, sizeof(buf),
                                "sw=%d lw=%d mild=%.2f%% severe=%.2f%% mult=%.3f lookback=%d rallyThr=%.1f rallyMult=%.2f",
                                sw, lw, mild, sev, mul, lb, rt, rmt);
                            localBestP = buf;
                        }

                        char rowBuf[256];
                        snprintf(rowBuf, sizeof(rowBuf),
                            "%d,%d,%.4f,%.4f,%.4f,%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%d",
                            sw, lw, mild, sev, mul, lb, rt, rmt,
                            res.finalValue, res.totalReturn, res.cagr,
                            res.maxDrawdown, res.totalTrades);
                        string csvRow(rowBuf);

                        if ((int)localTop.size() < 100) {
                            localTop.push_back({res.finalValue, csvRow});
                        } else {
                            int wi = 0;
                            for (int x=1; x<100; x++)
                                if (localTop[x].first < localTop[wi].first) wi=x;
                            if (res.finalValue > localTop[wi].first)
                                localTop[wi] = {res.finalValue, csvRow};
                        }
                        localCount++;
                    }
                  }
                }
              }
            }
          }
          // Print progress after every (a,b) pair — 324 ticks total (no lock needed single-threaded)
          {
              long long done   = current + localCount;
              int filled = (int)(50.0 * done / total);
              int pct    = (int)(100.0 * done / total);
              cout << "\r[";
              for (int box=0; box<50; box++) cout << (box<filled ? '=' : ' ');
              cout << "] " << pct << "% (" << done << "/" << total << ")" << flush;
          }
        }

        #pragma omp critical
        {
            current += localCount;
            if (localBest > bestValue) { bestValue=localBest; bestParams=localBestP; }

            for (auto& e : localTop) {
                if ((int)top100.size() < 100) {
                    top100.push_back(e);
                } else {
                    int wi=0;
                    for (int x=1; x<100; x++)
                        if (top100[x].first < top100[wi].first) wi=x;
                    if (e.first > top100[wi].first) top100[wi]=e;
                }
            }

            int filled = (int)(50.0*current/total);
            int pct    = (int)(100.0*current/total);
            cout << "\r[";
            for (int box=0; box<50; box++) cout << (box<filled ? '=' : ' ');
            cout << "] " << pct << "% (" << current << "/" << total << ")" << flush;

            vector<pair<double,string>> snap = top100;
            sort(snap.begin(), snap.end(),
                 [](const pair<double,string>& x, const pair<double,string>& y){ return x.first > y.first; });
            ofstream csvOut("sweep_top100.csv");
            csvOut << "Rank,SW,LW,MildDip,SevereDip,Mult,Lookback,RallyThreshold,RallyMultiplier,FinalValue,TotalReturn,CAGR,MaxDrawdown,TotalTrades\n";
            for (int si=0; si<(int)snap.size(); si++)
                csvOut << (si+1) << "," << snap[si].second << "\n";
            csvOut.close();
        }
    }

    cout << "\n\nTop " << top100.size() << " results written to sweep_top100.csv\n";
    cout << "Optimal parameters: " << bestParams << "\n";
    cout << "Best final value  : $" << fixed << setprecision(2) << bestValue << "\n";
}
// ===== END REMOVE HERE =====

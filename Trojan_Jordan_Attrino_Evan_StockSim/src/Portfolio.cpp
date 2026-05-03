#include "Portfolio.h"
#include <iostream>
#include <algorithm>

using namespace std;

Portfolio::Portfolio(const string& ownerName, double initialCash){
    this->ownerName = ownerName;
    this->cashBalance = initialCash;
}


void Portfolio::buyShares(const string& ticker, int shares, double price, const string& date) {
    double cost = shares * price;

    if (cost > cashBalance) {
        cout << "Insufficient funds" << endl;
        return;
    }

    cashBalance -= cost;

    // check if position already exists
    bool found = false;
    for (int i = 0; i < holdings.size(); i++) {
        if (holdings[i].ticker == ticker) {
            // update average cost basis
            double totalCost = holdings[i].avgCostBasis * holdings[i].shares + cost;
            holdings[i].shares      += shares;
            holdings[i].avgCostBasis = totalCost / holdings[i].shares;
            holdings[i].currentPrice = price;
            found = true;
            break;
        }
    }

    // position doesn't exist yet — create it
    if (!found) {
        Position p;
        p.ticker       = ticker;
        p.shares       = shares;
        p.avgCostBasis = price;
        p.currentPrice = price;
        holdings.push_back(p);
    }

    // push to trade history
    TradeRecord record;
    record.ticker    = ticker;
    record.date      = date;
    record.price     = price;
    record.shares    = shares;
    record.action    = "BUY";
    record.totalCost = cost;
    tradeHistory.push(record);


}


void Portfolio::sellShares(const string& ticker, int shares, double price, const string& date) {
    double proceeds = shares * price;

    bool found = false;
    for (int i = 0; i < (int)holdings.size(); i++) {
        if (holdings[i].ticker == ticker) {
            if (holdings[i].shares < shares) {
                cout << "Not enough shares" << endl;
                return;
            }
            holdings[i].shares -= shares;
            holdings[i].currentPrice = price;
            if (holdings[i].shares == 0) {
                holdings.erase(holdings.begin() + i);
            }
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Stock not in portfolio" << endl;
        return;
    }

    cashBalance += proceeds;

    TradeRecord record;
    record.ticker    = ticker;
    record.date      = date;
    record.price     = price;
    record.shares    = shares;
    record.action    = "SELL";
    record.totalCost = proceeds;
    tradeHistory.push(record);


}


void Portfolio::undoLastTrade() {
    if (tradeHistory.isEmpty()) {
        cout << "Nothing to undo" << endl;
        return;
    }
    TradeRecord last = tradeHistory.pop();

    if (last.action == "BUY") {
        // reverse a buy — give cash back, remove shares
        cashBalance += last.totalCost;
        for (int i = 0; i < (int)holdings.size(); i++) {
            if (holdings[i].ticker == last.ticker) {
                holdings[i].shares -= last.shares;
                if (holdings[i].shares == 0) {
                    holdings.erase(holdings.begin() + i);
                }
                break;
            }
        }
    } else if (last.action == "SELL") {
        // reverse a sell — deduct cash, add shares back
        cashBalance -= last.totalCost;
        bool found = false;
        for (int i = 0; i < (int)holdings.size(); i++) {
            if (holdings[i].ticker == last.ticker) {
                holdings[i].shares += last.shares;
                found = true;
                break;
            }
        }
        if (!found) {
            Position p;
            p.ticker       = last.ticker;
            p.shares       = last.shares;
            p.avgCostBasis = last.price;
            p.currentPrice = last.price;
            holdings.push_back(p);
        }
    }
   
}

void Portfolio::queueOrder(const Order& order) {
    pendingOrders.enqueue(order);
}


void Portfolio::executeNextOrder(double currentPrice, const string& date) {
    if(pendingOrders.isEmpty()){
        return;
    }
    Order top = pendingOrders.peek();
    if(top.type == "LIMIT" && top.side == "SELL"){
        if(currentPrice >= top.targetPrice){
            sellShares(top.ticker, top.shares,currentPrice,date);
            pendingOrders.dequeue();
        }else{
            cout << "Order Skipped" << endl;
        }
    }else if(top.type == "LIMIT" && top.side == "BUY"){
        if(currentPrice <= top.targetPrice){
            buyShares(top.ticker, top.shares,currentPrice,date);
            pendingOrders.dequeue();
        }else{
            cout << "Order Skipped" << endl;
        }
    }else{
        if(top.side == "BUY"){
            buyShares(top.ticker, top.shares, currentPrice, date);
        }else if(top.side == "SELL"){
            sellShares(top.ticker, top.shares, currentPrice, date);
        }
        pendingOrders.dequeue();
    }
}


double Portfolio::getTotalMarketValue() const {
    double total = 0.0;
    for (int i = 0; i < (int)holdings.size(); i++) {
        total += holdings[i].shares * holdings[i].currentPrice;
    }
    return total;
}

double Portfolio::getTotalValue() const {
    return getTotalMarketValue() + cashBalance;
}


double Portfolio::getTotalUnrealizedReturn() const {
    double marketValue = 0.0;
    double costBasis   = 0.0;
    for (int i = 0; i < (int)holdings.size(); i++) {
        marketValue += holdings[i].shares * holdings[i].currentPrice;
        costBasis   += holdings[i].shares * holdings[i].avgCostBasis;
    }
    if (costBasis == 0.0) return 0.0;
    return (marketValue - costBasis) / costBasis * 100.0;
}

double Portfolio::getCashBalance() const {
    return cashBalance;
}

void Portfolio::updatePrice(const string& ticker, double newPrice) {
    for (int i =0; i < (int)holdings.size(); i++ ) {
        if (holdings[i].ticker == ticker) {
            holdings[i].currentPrice = newPrice;
        }
    }
}

void Portfolio::sortHoldingsByUnrealizedReturn() {
    sort(holdings.begin(), holdings.end(), [](const Position& a, const Position& b) {
        double retA = (a.avgCostBasis > 0) ? (a.currentPrice - a.avgCostBasis) / a.avgCostBasis : 0.0;
        double retB = (b.avgCostBasis > 0) ? (b.currentPrice - b.avgCostBasis) / b.avgCostBasis : 0.0;
        return retA > retB;  
    });
}


void Portfolio::sortHoldingsByTicker() {
    sort(holdings.begin(), holdings.end(), [](const Position& a, const Position& b) {
        return a.ticker < b.ticker;
    });
}    

void Portfolio::printHoldings() const {
    cout << "Owner    : " << ownerName << endl;
    cout << "Cash     : $" << cashBalance << endl;
    cout << "Total Val: $" << getTotalValue() << endl;
    cout << "Return   : " << getTotalUnrealizedReturn() << "%" << endl;
    if (holdings.empty()) {
        cout << "No positions" << endl;
        return;
    }

    for (int i = 0; i < (int)holdings.size(); i++) {
        double mktVal  = holdings[i].shares * holdings[i].currentPrice;
        double cost    = holdings[i].shares * holdings[i].avgCostBasis;
        double ret     = (cost > 0) ? (mktVal - cost) / cost * 100.0 : 0.0;
        cout << "Ticker : " << holdings[i].ticker << endl;
        cout << "Shares : " << holdings[i].shares << endl;
        cout << "Avg Cost: $" << holdings[i].avgCostBasis << endl;
        cout << "Curr Price: $" << holdings[i].currentPrice << endl;
        cout << "Return : " << ret << "%" << endl;
    }
}

void Portfolio::printTradeHistory() const {
    tradeHistory.printAll();
}

void Portfolio::printPendingOrders() const {
    pendingOrders.printAll();
}
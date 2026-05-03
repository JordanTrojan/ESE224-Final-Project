#ifndef STOCK_MANAGER_H
#define STOCK_MANAGER_H

#include "FinancialAsset.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

// Generic manager for any collection of FinancialAsset-derived objects.
// Must be fully defined in this header (template classes cannot be split across .cpp).
//
// Usage:
//   StockManager<Stock> stockManager;   // manages Stock objects
//   StockManager<ETF>   etfManager;     // manages ETF objects
//
// T must be a type that:
//   - Inherits from FinancialAsset
//   - Has getTicker(), calculateAnnualReturn(int), printSummary()
//
// Course module: Templates (generic programming)
template <typename T>
class StockManager {
private:
    vector<T*> assets;  // owns all T* pointers — must delete in destructor

public:
    StockManager() {}

    ~StockManager() {
    for (int i = 0; i < (int)assets.size(); i++) {
        delete assets[i];
    }
    assets.clear();
    }

    void addAsset(T* asset) {
        assets.push_back(asset);
    }

    void removeAsset(const string& ticker) {
        for (int i = 0; i < (int)assets.size(); i++) {
            if (assets[i]->getTicker() == ticker) {
                delete assets[i];
                assets.erase(assets.begin() + i);
                return;
            }
        }
    }

    T* findByTicker(const string& ticker) const {
        for (int i = 0; i < (int)assets.size(); i++) {
            if (assets[i]->getTicker() == ticker) {
                return assets[i];
            }
        }
        return nullptr;
    }

    void sortByAnnualReturn(int year) {
        sort(assets.begin(), assets.end(), [year](T* a, T* b) {
            return a->calculateAnnualReturn(year) > b->calculateAnnualReturn(year);
        });
    }

    void sortByTicker() {
        sort(assets.begin(), assets.end(), [](T* a, T* b) {
            return a->getTicker() < b->getTicker();
        });
    }

    void printAll() const {
        for (int i = 0; i < (int)assets.size(); i++) {
            assets[i]->printSummary();
        }
    }

    int getCount() const {
    return (int)assets.size();
    }
};

#endif // STOCK_MANAGER_H

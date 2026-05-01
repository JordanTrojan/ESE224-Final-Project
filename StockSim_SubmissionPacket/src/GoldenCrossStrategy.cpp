#include "GoldenCrossStrategy.h"
using namespace std;
#include <iostream>

GoldenCrossStrategy::GoldenCrossStrategy(int shortWindow, int longWindow){
    this->shortWindow = shortWindow;
    this->longWindow = longWindow;
}
SimResult GoldenCrossStrategy::backtest(PriceHistory* history, double monthlyCapital, int startYear, int endYear){
    SimResult result;
    CircularQueue ma50Queue(shortWindow);
    CircularQueue ma200Queue(longWindow);
    double prev_ma50 = 0.0;
    double prev_ma200 = 0.0;
    double curr_ma50 = 0.0;
    double curr_ma200 = 0.0;
    double cash = 0.0;
    double fractionalShares = 0.0;
    bool prevValues = false;

    result.strategyName  = getName();
    result.finalValue    = 0.0;
    result.totalInvested = 0.0;
    result.totalReturn   = 0.0;
    result.cagr          = 0.0;
    result.maxDrawdown   = 0.0;
    result.totalTrades   = 0;

    vector<double> portfolioValues;
    double lastClose = 0.0;
    int    lastMonth = -1, lastYear = -1;
    for(PriceHistory::Iterator it = history->begin(); it != history->end(); ++it){
        PriceNode& node = *it;
        int y = CSVParser::extractYear(node.date); 
        int m = CSVParser::extractMonth(node.date);
        ma50Queue.enqueue(node.close);
        ma200Queue.enqueue(node.close);

        if (y < startYear) { lastMonth = m; lastYear = y; continue; }
        if (y > endYear)   break;


        if (m != lastMonth || y != lastYear) {
            cash             += monthlyCapital;
            result.totalInvested += monthlyCapital;
            lastMonth = m;
            lastYear = y;
        }

        
        if((ma50Queue.isFull()) && (ma200Queue.isFull())){
            curr_ma50 = ma50Queue.getAverage();
            curr_ma200 = ma200Queue.getAverage();
            if(prevValues){
                if((prev_ma50 < prev_ma200) && (curr_ma50 > curr_ma200)){
                    fractionalShares += cash / node.close;
                    cash = 0;
                    result.totalTrades++;
                }else if((prev_ma50 > prev_ma200) && (curr_ma50 < curr_ma200)){
                    cash += fractionalShares * node.close;
                    fractionalShares = 0;
                    result.totalTrades++;
                }
            }
            prev_ma50  = curr_ma50;  
            prev_ma200 = curr_ma200;
            prevValues = true;       
        }

        lastClose = node.close;
        portfolioValues.push_back(fractionalShares * node.close + cash);
    }
    result.finalValue  = fractionalShares * lastClose + cash;
    result.totalReturn = (result.totalInvested > 0)
        ? (result.finalValue - result.totalInvested) / result.totalInvested * 100.0 : 0.0;
    result.cagr        = calculateCAGR(result.totalInvested, result.finalValue, endYear - startYear);
    result.maxDrawdown = calculateMaxDrawdown(portfolioValues);
    return result; 
}

string GoldenCrossStrategy::getName() const{
    return "Golden Cross Strategy";
}
                
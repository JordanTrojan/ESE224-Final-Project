
#include "FixedSIPStrategy.h"

// Fixed Systematic Investment Plan (SIP): invest a fixed dollar amount
// on the first available trading day of every calendar month, regardless
// of market conditions. This is the baseline strategy.
//
// Backtest logic:
//   For each month in [startYear, endYear]:
//     Find the first PriceNode whose month matches.
//     Buy (monthlyCapital / closePrice) fractional shares at that close price.
//     Record the portfolio value snapshot for drawdown calculation.
//
// Course module: OOP (concrete derived class, override)


SimResult FixedSIPStrategy::backtest(PriceHistory* history, double monthlyCapital, int startYear, int endYear)  {
    SimResult result;
    result.strategyName = getName();
    result.finalValue = 0;     // final portfolio value at end of simulation
    result.totalInvested = 0;   // total cash put into the market
    result.totalReturn = 0;     // (finalValue - totalInvested) / totalInvested * 100
    result.cagr = 0;            // Compound Annual Growth Rate (%)
    result.maxDrawdown = 0;     // worst peak-to-trough decline (%)
    result.totalTrades = 0;     // number of buy/sell executions

    if(!history || history->getSize() == 0) return result;

    double fractionalShares = 0.0;

    vector<double> portfolioValues;

    int lastMonth = -1;
    int lastYear = -1;
    double lastClose = 0.0;

    for (PriceHistory::Iterator it = history->begin(); it != history->end(); ++it) {
        PriceNode& node = *it;

        int y = CSVParser::extractYear(node.date);
        int m = CSVParser::extractMonth(node.date);

        if (y < startYear) continue;
        if (y > endYear) break;

        if (m != lastMonth || y != lastYear) {
            fractionalShares += monthlyCapital / node.close;
            result.totalInvested += monthlyCapital;
            result.totalTrades++;
            lastMonth = m;
            lastYear = y;
        }
        lastClose = node.close;
        portfolioValues.push_back(fractionalShares * node.close);
    }
    result.finalValue = fractionalShares * lastClose;
    result.totalReturn = (result.totalInvested > 0) ? (result.finalValue - result.totalInvested) / result.totalInvested * 100 : 0;
    result.cagr = calculateCAGR(result.totalInvested, result.finalValue, endYear-startYear);
    result.maxDrawdown = calculateMaxDrawdown(portfolioValues);
    return result;
}

string FixedSIPStrategy::getName() const  {
    return "Fixed SIP";
}

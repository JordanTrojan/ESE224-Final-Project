#include "MomentumStrategy.h"

// 6-Month Momentum Strategy: invest when recent price trend is positive,
// stay in cash when the trend turns negative.
//
// Backtest logic (evaluated at the start of each month):
//   1. Use the ReverseIterator on PriceHistory to scan backward ~126 trading days
//      (approx. 6 months) from the current date.
//   2. Compute 6-month trailing return: (current_price - price_6mo_ago) / price_6mo_ago * 100
//   3. If trailing return > momentumThreshold: buy / stay invested (invest monthlyCapital)
//   4. If trailing return < 0: sell all holdings, hold cash
//   5. Otherwise (0 to threshold): hold current position, no new investment
//
// The ReverseIterator integration is the key requirement here.
//
// Course modules: OOP (derived class), Iterators (ReverseIterator in practice)

MomentumStrategy::MomentumStrategy(double m, int d) {
    momentumThreshold = m;
    lookbackDays      = d;
}

SimResult MomentumStrategy::backtest(PriceHistory* history, double monthlyCapital, int startYear, int endYear) {
    SimResult result;
    result.strategyName  = getName();
    result.finalValue    = 0.0;
    result.totalInvested = 0.0;
    result.totalReturn   = 0.0;
    result.cagr          = 0.0;
    result.maxDrawdown   = 0.0;
    result.totalTrades   = 0;

    if (!history || history->getSize() == 0) return result;

    double fractionalShares = 0.0;
    double cash             = 0.0;
    vector<double> portfolioValues;

    int    lastMonth = -1, lastYear = -1;
    double lastClose = 0.0;

    for (PriceHistory::Iterator it = history->begin(); it != history->end(); ++it) {
        PriceNode& node = *it;

        int y = CSVParser::extractYear(node.date);
        int m = CSVParser::extractMonth(node.date);

        if (y < startYear) { lastMonth = m; lastYear = y; continue; }
        if (y > endYear)   break;

        // only act on first trading day of each month
        if (m != lastMonth || y != lastYear) {
            cash += monthlyCapital;
            result.totalInvested += monthlyCapital;

            // walk back lookbackDays using ReverseIterator
            PriceHistory::ReverseIterator rit(&(*it));
            int steps = 0;
            while (steps < lookbackDays && rit != history->rend()) {
                ++rit;
                ++steps;
            }

            if (steps == lookbackDays) {
                double pastPrice      = (*rit).close;
                double trailingReturn = (node.close - pastPrice) / pastPrice * 100.0;

                if (trailingReturn > momentumThreshold) {
                    // buy — invest monthly capital
                    fractionalShares += cash / node.close;
                    cash              = 0.0;
                    result.totalTrades++;
                } else if (trailingReturn < 0.0) {
                    // sell all
                    if (fractionalShares > 0.0) {
                        cash             += fractionalShares * node.close;
                        fractionalShares  = 0.0;
                        result.totalTrades++;
                    }
                }
                // between 0 and threshold — hold, no action
            }

            lastMonth = m;
            lastYear  = y;
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

string MomentumStrategy::getName() const {
    return "Momentum Strategy";
}


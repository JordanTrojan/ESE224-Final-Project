#include "DynamicSIPStrategy.h"
#include "CSVParser.h"
#include <vector>
using namespace std;

DynamicSIPStrategy::DynamicSIPStrategy(int shortWindow, int longWindow, double mildDipThreshold, double severeDipThreshold, double mildMultiplier, int lookbackDays, double rallyThreshold, double rallyMultiplier) {
    this->shortWindow        = shortWindow;
    this->longWindow         = longWindow;
    this->mildDipThreshold   = mildDipThreshold;
    this->severeDipThreshold = severeDipThreshold;
    this->mildMultiplier     = mildMultiplier;
    this->lookbackDays       = lookbackDays;
    this->rallyThreshold     = rallyThreshold;
    this->rallyMultiplier    = rallyMultiplier;
}

SimResult DynamicSIPStrategy::backtest(PriceHistory* history, double monthlyCapital,
                                        int startYear, int endYear) {
    SimResult result;
    result.strategyName  = getName();
    result.finalValue    = 0.0;
    result.totalInvested = 0.0;
    result.totalReturn   = 0.0;
    result.cagr          = 0.0;
    result.maxDrawdown   = 0.0;
    result.totalTrades   = 0;

    if (!history || history->getSize() == 0) return result;

    // Two circular queues act as moving average windows — enqueue each day's close
    CircularQueue shortMA(shortWindow);
    CircularQueue longMA(longWindow);

    // Track previous day's MAs to detect crossover direction
    double prev_short = 0.0, prev_long = 0.0;
    double curr_short = 0.0, curr_long = 0.0;
    bool   prevValues = false;  // false until both MAs have a full window of data
    bool   inMarket   = false;  // true between golden cross and death cross

    // Midpoint between mild and severe — triggers a 2x buy instead of 1x
    double moderateDipThreshold = (mildDipThreshold + severeDipThreshold) / 2.0;

    double fractionalShares = 0.0;  // shares held (fractional allowed)
    double cash             = 0.0;  // uninvested cash balance
    vector<double> portfolioValues; // daily portfolio value, used for max drawdown calc
    portfolioValues.reserve(history->getSize());

    int    lastMonth = -1, lastYear = -1;
    double lastClose = 0.0;  // saved so final portfolio value uses the last price seen

    for (PriceHistory::Iterator it = history->begin(); it != history->end(); ++it) {
        PriceNode& node = *it;

        int y = CSVParser::extractYear(node.date);
        int m = CSVParser::extractMonth(node.date);

        // Feed both MAs every day — warmup period before startYear still builds the windows
        shortMA.enqueue(node.close);
        longMA.enqueue(node.close);

        if (y < startYear) { lastMonth = m; lastYear = y; continue; }  // warmup — don't invest yet
        if (y > endYear)   break;

        // First trading day of a new month: add monthly contribution to cash
        if (m != lastMonth || y != lastYear) {
            cash += monthlyCapital;
            result.totalInvested += monthlyCapital;
            lastMonth = m;
            lastYear  = y;
        }

        // Only act on crossover signals once both MAs have filled their windows
        if (shortMA.isFull() && longMA.isFull()) {
            curr_short = shortMA.getAverage();
            curr_long  = longMA.getAverage();

            if (prevValues) {
                // Golden Cross: short MA crosses above long MA — bull signal, deploy all cash
                if (prev_short < prev_long && curr_short > curr_long) {
                    inMarket = true;
                    if (cash > 0.0) {
                        fractionalShares += cash / node.close;  // deploy all accumulated cash
                        cash = 0.0;
                        result.totalTrades++;
                    }
                }
                // Death Cross: short MA crosses below long MA — bear signal, sell everything
                else if (prev_short > prev_long && curr_short < curr_long) {
                    inMarket = false;
                    if (fractionalShares > 0.0) {
                        cash += fractionalShares * node.close;  // convert all shares to cash
                        fractionalShares = 0.0;
                        result.totalTrades++;
                    }
                }
            }

            // Dip detection runs every day — checks how far price has fallen from rolling high.
            // Skip entirely if no cash available (nothing to invest).
            if (cash > 0.0) {
                // Walk back lookbackDays using ReverseIterator to find rolling high and low
                PriceHistory::ReverseIterator rit(&(*it));
                double high12 = node.close, low12 = node.close;
                int steps = 0;
                while (steps < lookbackDays && rit != history->rend()) {
                    double p = (*rit).close;
                    if (p > high12) high12 = p;
                    if (p < low12)  low12  = p;
                    ++rit;
                    ++steps;
                }

                // % drop from the rolling high — used to classify dip severity
                double dropFromHigh = (high12 > 0.0)
                    ? (high12 - node.close) / high12 * 100.0 : 0.0;
                // % rise from the rolling low — used to detect rally (reduce buying on surges)
                double riseFromLow  = (low12  > 0.0)
                    ? (node.close - low12)  / low12  * 100.0 : 0.0;

                double toInvest = 0.0;
                if (dropFromHigh >= severeDipThreshold) {
                    // Severe crash (e.g. 2008, dot-com) — deploy ALL cash regardless of market status
                    toInvest = cash;
                } else if (inMarket) {
                    if (dropFromHigh >= moderateDipThreshold) {
                        // Moderate dip — invest 2x the mild multiplier amount
                        toInvest = mildMultiplier * 2.0 * monthlyCapital;
                    } else if (dropFromHigh >= mildDipThreshold) {
                        // Mild dip — invest multiplier * monthly capital
                        toInvest = mildMultiplier * monthlyCapital;
                    } else if (rallyThreshold > 0.0 && riseFromLow >= rallyThreshold) {
                        // Price has surged from its low — reduce investment to avoid buying at peak
                        toInvest = rallyMultiplier * monthlyCapital;
                    } else {
                        // Normal day in market — invest baseline monthly capital
                        toInvest = monthlyCapital;
                    }
                }
                // out of market + no severe dip: hold cash, accumulate for golden cross

                if (toInvest > cash) toInvest = cash;  // never invest more than available cash
                if (toInvest > 0.0) {
                    fractionalShares += toInvest / node.close;  // buy shares at today's price
                    cash             -= toInvest;
                    result.totalTrades++;
                }
            }

            // Save today's MAs so tomorrow can detect a crossover direction change
            prev_short = curr_short;
            prev_long  = curr_long;
            prevValues = true;
        }

        lastClose = node.close;
        portfolioValues.push_back(fractionalShares * node.close + cash);  // snapshot for drawdown calc
    }

    // Final portfolio value = shares at last price + remaining cash
    result.finalValue  = fractionalShares * lastClose + cash;
    result.totalReturn = (result.totalInvested > 0)
        ? (result.finalValue - result.totalInvested) / result.totalInvested * 100.0 : 0.0;
    result.cagr        = calculateCAGR(result.totalInvested, result.finalValue, endYear - startYear);
    result.maxDrawdown = calculateMaxDrawdown(portfolioValues);
    return result;
}

string DynamicSIPStrategy::getName() const {
    return "Dynamic SIP";
}

double DynamicSIPStrategy::getMildDipThreshold()   const { return mildDipThreshold; }
double DynamicSIPStrategy::getSevereDipThreshold() const { return severeDipThreshold; }
double DynamicSIPStrategy::getMildMultiplier()     const { return mildMultiplier; }

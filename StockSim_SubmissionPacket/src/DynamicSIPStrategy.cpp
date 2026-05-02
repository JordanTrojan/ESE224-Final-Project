#include "DynamicSIPStrategy.h"
#include "CSVParser.h"
#include <vector>
using namespace std;

DynamicSIPStrategy::DynamicSIPStrategy(int shortWindow, int longWindow, double mildDipThreshold, double severeDipThreshold, double mildMultiplier, int lookbackDays) {
    this->shortWindow        = shortWindow;
    this->longWindow         = longWindow;
    this->mildDipThreshold   = mildDipThreshold;
    this->severeDipThreshold = severeDipThreshold;
    this->mildMultiplier     = mildMultiplier;
    this->lookbackDays       = lookbackDays;
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

    CircularQueue shortMA(shortWindow);
    CircularQueue longMA(longWindow);

    double prev_short = 0.0, prev_long = 0.0;
    double curr_short = 0.0, curr_long = 0.0;
    bool   prevValues = false;
    bool   inMarket   = false;

    double moderateDipThreshold = (mildDipThreshold + severeDipThreshold) / 2.0;

    double fractionalShares = 0.0;
    double cash             = 0.0;
    vector<double> portfolioValues;

    int    lastMonth = -1, lastYear = -1;
    double lastClose = 0.0;

    for (PriceHistory::Iterator it = history->begin(); it != history->end(); ++it) {
        PriceNode& node = *it;

        int y = CSVParser::extractYear(node.date);
        int m = CSVParser::extractMonth(node.date);

        shortMA.enqueue(node.close);
        longMA.enqueue(node.close);

        if (y < startYear) { lastMonth = m; lastYear = y; continue; }
        if (y > endYear)   break;

        // add monthly capital to cash reserve
        if (m != lastMonth || y != lastYear) {
            cash += monthlyCapital;
            result.totalInvested += monthlyCapital;
            lastMonth = m;
            lastYear  = y;
        }

        if (shortMA.isFull() && longMA.isFull()) {
            curr_short = shortMA.getAverage();
            curr_long  = longMA.getAverage();

            if (prevValues) {
                // Golden Cross — enter market
                if (prev_short < prev_long && curr_short > curr_long) {
                    inMarket = true;
                    if (cash > 0.0) {
                        fractionalShares += cash / node.close;
                        cash = 0.0;
                        result.totalTrades++;
                    }
                }
                // Death Cross — exit market
                else if (prev_short > prev_long && curr_short < curr_long) {
                    inMarket = false;
                    if (fractionalShares > 0.0) {
                        cash += fractionalShares * node.close;
                        fractionalShares = 0.0;
                        result.totalTrades++;
                    }
                }
            }

            // Dip detection — runs every day regardless of market status
            if (cash > 0.0) {
                double high12 = node.close;
                PriceHistory::ReverseIterator rit(&(*it));
                int steps = 0;
                while (steps < lookbackDays && rit != history->rend()) {
                    ++rit; ++steps;
                    if (!(rit != history->rend())) break;
                    double c = (*rit).close;
                    if (c > high12) high12 = c;
                }

                double dropFromHigh = (high12 > 0.0)
                    ? (high12 - node.close) / high12 * 100.0 : 0.0;

                double toInvest = 0.0;
                if (dropFromHigh >= severeDipThreshold) {
                    // severe crash — deploy ALL cash (in OR out of market)
                    toInvest = cash;
                } else if (inMarket) {
                    if (dropFromHigh >= moderateDipThreshold) {
                        // moderate dip — 2x mild multiplier
                        toInvest = mildMultiplier * 2.0 * monthlyCapital;
                    } else if (dropFromHigh >= mildDipThreshold) {
                        // mild dip — amplified buy
                        toInvest = mildMultiplier * monthlyCapital;
                    } else {
                        // no dip — invest monthly as baseline
                        toInvest = monthlyCapital;
                    }
                }
                // out of market + no severe dip: hold cash, accumulate for golden cross

                if (toInvest > cash) toInvest = cash;
                if (toInvest > 0.0) {
                    fractionalShares += toInvest / node.close;
                    cash             -= toInvest;
                    result.totalTrades++;
                }
            }

            prev_short = curr_short;
            prev_long  = curr_long;
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

string DynamicSIPStrategy::getName() const {
    return "Dynamic SIP";
}

double DynamicSIPStrategy::getMildDipThreshold()   const { return mildDipThreshold; }
double DynamicSIPStrategy::getSevereDipThreshold() const { return severeDipThreshold; }
double DynamicSIPStrategy::getMildMultiplier()     const { return mildMultiplier; }
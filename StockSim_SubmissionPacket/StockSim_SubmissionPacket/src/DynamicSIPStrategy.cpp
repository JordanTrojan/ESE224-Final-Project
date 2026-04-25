#include "DynamicSIPStrategy.h"
//MUST CHOOSE BETWEEN THE TWO STRATEGIES, COMMENT OUT THE OTHER ONE


DynamicSIPStrategy::DynamicSIPStrategy(double dipThreshold, double rallyThreshold, double multiplier) {
    this->dipThreshold = dipThreshold;
    this->rallyThreshold = rallyThreshold;
    this->multiplier = multiplier;
}

SimResult DynamicSIPStrategy::backtest(PriceHistory* history, double monthlyCapital, int startYear, int endYear) {
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

    double yearlyBudget     = 12.0 * monthlyCapital;
    double deployedThisYear = 0.0;
    int    currentYear      = startYear;
    bool   newCashAvailable = false;
    const int lookbackDays  = 252;

    for (PriceHistory::Iterator it = history->begin(); it != history->end(); ++it) {
        PriceNode& node = *it;

        int y = CSVParser::extractYear(node.date);
        int m = CSVParser::extractMonth(node.date);

        if (y < startYear) { lastMonth = m; lastYear = y; continue; }
        if (y > endYear)   break;

        if (y != currentYear) {
            deployedThisYear = 0.0;
            currentYear      = y;
        }

        if (m != lastMonth || y != lastYear) {
            cash += monthlyCapital;
            result.totalInvested += monthlyCapital;
            newCashAvailable = true;
            lastMonth = m;
            lastYear  = y;
        }

        double high12 = node.close, low12 = node.close;
        PriceHistory::ReverseIterator rit(&(*it));
        int steps = 0;
        while (steps < lookbackDays && rit != history->rend()) {
            ++rit; ++steps;
            double c = (*rit).close;
            if (c > high12) high12 = c;
            if (c < low12)  low12  = c;
        }

        double dropFromHigh = (high12 > 0.0) ? (high12 - node.close) / high12 * 100.0 : 0.0;
        double riseFromLow  = (low12  > 0.0) ? (node.close - low12)  / low12  * 100.0 : 0.0;

        if (dropFromHigh >= dipThreshold) {
            // dip: buy aggressively
            double toInvest = multiplier * monthlyCapital;
            double remaining = yearlyBudget - deployedThisYear;
            if (toInvest > remaining) toInvest = remaining;
            if (toInvest > cash)      toInvest = cash;
            if (toInvest > 0.0) {
                fractionalShares += toInvest / node.close;
                cash             -= toInvest;
                deployedThisYear += toInvest;
                result.totalTrades++;
            }
            newCashAvailable = false;
        } else if (riseFromLow >= rallyThreshold) {
            // rally: sell all shares, move to cash
            if (fractionalShares > 0.0) {
                cash += fractionalShares * node.close;
                fractionalShares = 0.0;
                result.totalTrades++;
            }
        } else if (newCashAvailable) {
            // normal month: invest this month's deposit
            double toInvest = monthlyCapital;
            double remaining = yearlyBudget - deployedThisYear;
            if (toInvest > remaining) toInvest = remaining;
            if (toInvest > cash)      toInvest = cash;
            if (toInvest > 0.0) {
                fractionalShares += toInvest / node.close;
                cash             -= toInvest;
                deployedThisYear += toInvest;
                result.totalTrades++;
            }
            newCashAvailable = false;
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

// Accessors (used by the parameter sweep bonus)
double DynamicSIPStrategy::getDipThreshold()   const {
    return this->dipThreshold;
}
double DynamicSIPStrategy::getRallyThreshold() const {
    return this->rallyThreshold;
}
double DynamicSIPStrategy::getMultiplier()     const {
    return this->multiplier;    
}
















TieredDipSIPStrategy::TieredDipSIPStrategy(double mildDipThreshold, double severeDipThreshold, double rallyThreshold, double mildMultiplier, double severeMultiplier) {
    this->mildDipThreshold   = mildDipThreshold;
    this->severeDipThreshold = severeDipThreshold;
    this->rallyThreshold     = rallyThreshold;
    this->mildMultiplier     = mildMultiplier;
    this->severeMultiplier   = severeMultiplier;
}

SimResult TieredDipSIPStrategy::backtest(PriceHistory* history, double monthlyCapital, int startYear, int endYear) {
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
    bool   newCashAvailable = false;
    const int lookbackDays  = 252;

    for (PriceHistory::Iterator it = history->begin(); it != history->end(); ++it) {
        PriceNode& node = *it;

        int y = CSVParser::extractYear(node.date);
        int m = CSVParser::extractMonth(node.date);

        if (y < startYear) { lastMonth = m; lastYear = y; continue; }
        if (y > endYear)   break;

        if (m != lastMonth || y != lastYear) {
            cash += monthlyCapital;
            result.totalInvested += monthlyCapital;
            newCashAvailable = true;
            lastMonth = m;
            lastYear  = y;
        }

        double high12 = node.close, low12 = node.close;
        PriceHistory::ReverseIterator rit(&(*it));
        int steps = 0;
        while (steps < lookbackDays && rit != history->rend()) {
            ++rit; ++steps;
            double c = (*rit).close;
            if (c > high12) high12 = c;
            if (c < low12)  low12  = c;
        }

        double dropFromHigh = (high12 > 0.0) ? (high12 - node.close) / high12 * 100.0 : 0.0;
        double riseFromLow  = (low12  > 0.0) ? (node.close - low12)  / low12  * 100.0 : 0.0;

        if (dropFromHigh >= severeDipThreshold) {
            // severe dip: dump all available cash
            if (cash > 0.0) {
                fractionalShares += cash / node.close;
                cash = 0.0;
                result.totalTrades++;
            }
            newCashAvailable = false;
        } else if (dropFromHigh >= mildDipThreshold) {
            // mild dip: buy at mild multiplier
            double toInvest = mildMultiplier * monthlyCapital;
            if (toInvest > cash) toInvest = cash;
            if (toInvest > 0.0) {
                fractionalShares += toInvest / node.close;
                cash             -= toInvest;
                result.totalTrades++;
            }
            newCashAvailable = false;
        } else if (riseFromLow >= rallyThreshold) {
            // rally: sell all shares, move to cash
            if (fractionalShares > 0.0) {
                cash += fractionalShares * node.close;
                fractionalShares = 0.0;
                result.totalTrades++;
            }
        } else if (newCashAvailable) {
            // normal month: invest this month's deposit only
            double toInvest = monthlyCapital;
            if (toInvest > cash) toInvest = cash;
            if (toInvest > 0.0) {
                fractionalShares += toInvest / node.close;
                cash             -= toInvest;
                result.totalTrades++;
            }
            newCashAvailable = false;
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












































#ifndef ELITE_STRATEGY_H
#define ELITE_STRATEGY_H

#include "TradingStrategy.h"
#include "CircularQueue.h"

// Hybrid: MA crossover (sell on Death Cross, buy on Golden Cross) +
// tiered dip-buy during bull phases + monthly SIP base investment.
class EliteStrategy : public TradingStrategy {
private:
    int    shortWindow;          // short MA window (e.g. 50)
    int    longWindow;           // long MA window  (e.g. 200)
    double mildDipThreshold;     // % drop from 12-month high for mild dip  (e.g. 5.0)
    double severeDipThreshold;   // % drop from 12-month high for severe dip (e.g. 15.0)
    double mildMultiplier;       // investment factor on mild dip            (e.g. 2.0)
    int    lookbackDays;         // days for rolling high calculation        (e.g. 252)

public:
    EliteStrategy(int shortWindow, int longWindow,
                  double mildDipThreshold, double severeDipThreshold,
                  double mildMultiplier, int lookbackDays);

    SimResult backtest(PriceHistory* history,
                       double monthlyCapital,
                       int startYear,
                       int endYear) override;

    string getName() const override;
};

#endif // ELITE_STRATEGY_H














#include "EliteStrategy.h"

EliteStrategy::EliteStrategy(int shortWindow, int longWindow,
                             double mildDipThreshold, double severeDipThreshold,
                             double mildMultiplier, int lookbackDays) {
    this->shortWindow        = shortWindow;
    this->longWindow         = longWindow;
    this->mildDipThreshold   = mildDipThreshold;
    this->severeDipThreshold = severeDipThreshold;
    this->mildMultiplier     = mildMultiplier;
    this->lookbackDays       = lookbackDays;
}

SimResult EliteStrategy::backtest(PriceHistory* history, double monthlyCapital, int startYear, int endYear) {
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
    bool   prevValues       = false;
    bool   inMarket         = false;
    bool   newCashAvailable = false;

    double fractionalShares = 0.0;
    double cash             = 0.0;
    vector<double> portfolioValues;

    int    lastMonth = -1, lastYear = -1;
    double lastClose = 0.0;

    for (PriceHistory::Iterator it = history->begin(); it != history->end(); ++it) {
        PriceNode& node = *it;

        int y = CSVParser::extractYear(node.date);
        int m = CSVParser::extractMonth(node.date);

        // always feed MAs for warmup even before startYear
        shortMA.enqueue(node.close);
        longMA.enqueue(node.close);

        if (y < startYear) { lastMonth = m; lastYear = y; continue; }
        if (y > endYear)   break;

        // receive monthly capital deposit
        if (m != lastMonth || y != lastYear) {
            cash += monthlyCapital;
            result.totalInvested += monthlyCapital;
            newCashAvailable = true;
            lastMonth = m;
            lastYear  = y;
        }

        if (shortMA.isFull() && longMA.isFull()) {
            curr_short = shortMA.getAverage();
            curr_long  = longMA.getAverage();

            if (prevValues) {
                // Golden Cross: short crosses above long → buy everything
                if (prev_short < prev_long && curr_short > curr_long) {
                    inMarket = true;
                    if (cash > 0.0) {
                        fractionalShares += cash / node.close;
                        cash = 0.0;
                        result.totalTrades++;
                    }
                    newCashAvailable = false;
                }
                // Death Cross: short crosses below long → sell everything
                else if (prev_short > prev_long && curr_short < curr_long) {
                    inMarket = false;
                    if (fractionalShares > 0.0) {
                        cash += fractionalShares * node.close;
                        fractionalShares = 0.0;
                        result.totalTrades++;
                    }
                }
            }

            // while in market: daily dip-buy check
            if (inMarket && cash > 0.0) {
                double high12 = node.close;
                PriceHistory::ReverseIterator rit(&(*it));
                int steps = 0;
                while (steps < lookbackDays && rit != history->rend()) {
                    ++rit; ++steps;
                    double c = (*rit).close;
                    if (c > high12) high12 = c;
                }

                double dropFromHigh = (high12 > 0.0)
                    ? (high12 - node.close) / high12 * 100.0 : 0.0;

                double toInvest = 0.0;
                if (dropFromHigh >= severeDipThreshold) {
                    // severe dip: dump all cash into the market
                    toInvest = cash;
                    newCashAvailable = false;
                } else if (dropFromHigh >= mildDipThreshold) {
                    // mild dip: invest at multiplier rate
                    toInvest = mildMultiplier * monthlyCapital;
                    newCashAvailable = false;
                } else if (newCashAvailable) {
                    // normal month: invest this month's deposit
                    toInvest = monthlyCapital;
                    newCashAvailable = false;
                }

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

string EliteStrategy::getName() const {
    return "Elite Strategy";
}

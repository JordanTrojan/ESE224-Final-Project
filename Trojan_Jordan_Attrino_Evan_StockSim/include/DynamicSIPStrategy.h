#ifndef DYNAMIC_SIP_STRATEGY_H
#define DYNAMIC_SIP_STRATEGY_H

#include "TradingStrategy.h"
#include "CircularQueue.h"

// Dynamic SIP Strategy — Hybrid Elite Algorithm
//
// This strategy combines three layers of market logic:
//
// Layer 1 — MA Crossover (market direction filter):
//   Uses a short MA (e.g. 50-day) and long MA (e.g. 200-day).
//   Golden Cross (short crosses above long) → enter market, buy all cash.
//   Death Cross  (short crosses below long) → exit market, SELL ALL SHARES,
//                                             hold proceeds as cash until
//                                             next Golden Cross signal.
//   While out of the market, monthly capital accumulates in cash
//   but is never invested until a Golden Cross triggers.
//
// Layer 2 — Tiered Dip Buying (while in market):
//   Every day while invested, checks how far price has dropped from 12-month high.
//   Mild dip  (>= mildDipThreshold%)   → invest mildMultiplier * monthlyCapital
//   Severe dip(>= severeDipThreshold%) → dump ALL available cash into the market
//
// Layer 3 — Monthly SIP Base (while in market, normal months):
//   On the first trading day of each month, if no dip signal triggered,
//   invest this month's monthlyCapital as a baseline contribution.
//
// Result: aggressively buys dips during confirmed bull markets,
//         sits in cash during bear markets (after Death Cross),
//         and maintains a steady base investment in normal conditions.


class DynamicSIPStrategy : public TradingStrategy {
private:
    int    shortWindow;           // short MA period (e.g. 50 days)
    int    longWindow;            // long MA period  (e.g. 200 days)
    double mildDipThreshold;     // % drop from 12-month high for mild dip  (e.g. 5.0)
    double severeDipThreshold;   // % drop from 12-month high for severe dip (e.g. 15.0)
    double mildMultiplier;       // investment multiplier on mild dip (e.g. 2.0)
    int    lookbackDays;         // days for rolling high/low calculation (e.g. 252 = 1 year)
    double rallyThreshold;       // % rise from lookback low that triggers rally mode (e.g. 20.0)
    double rallyMultiplier;      // fraction of monthlyCapital to invest during a rally (e.g. 0.1)
public:
    DynamicSIPStrategy(int shortWindow, int longWindow,
                       double mildDipThreshold, double severeDipThreshold,
                       double mildMultiplier, int lookbackDays,
                       double rallyThreshold = 0.0, double rallyMultiplier = 1.0);

    SimResult backtest(PriceHistory* history,
                       double monthlyCapital,
                       int startYear,
                       int endYear) override;

    string getName() const override;

    double getMildDipThreshold()   const;
    double getSevereDipThreshold() const;
    double getMildMultiplier()     const;
};

#endif // DYNAMIC_SIP_STRATEGY_H
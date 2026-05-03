#include "TradingStrategy.h"

void TradingStrategy::printResult(const SimResult& result) const {
    cout << "Strategy name: " << result.strategyName << endl;
    cout << "Final Value: " << result.finalValue << endl;
    cout << "Total Invested: " << result.totalInvested << endl;
    cout << "Total return (percentage): " << result.totalReturn << endl;
    cout << "Compound Annual Growth Rate: " << result.cagr << endl;
    cout << "Worst peak-to-trough drop: " << result.maxDrawdown << endl;
    cout << "Total Trades: " << result.totalTrades << endl;
}

double TradingStrategy::calculateCAGR(double startVal, double endVal, int years) const {
    if((years <= 0) || (startVal <= 0.0)){
        return 0.0;
    }
    return (pow(endVal / startVal, 1.0 / years) - 1.0) * 100;
}

double TradingStrategy::calculateMaxDrawdown(const vector<double>& portfolioValues) const {
    if(portfolioValues.empty()){
        return 0.0;
    }
    double peak = portfolioValues.at(0);
    double maxDrawdown = 0.0;

    for(int i = 0; i < portfolioValues.size(); i++){
        if(portfolioValues.at(i) > peak){
            peak = portfolioValues.at(i);
        }else{
            double drawdown = (peak > 0.0) ? (peak - portfolioValues.at(i)) / peak * 100 : 0.0;
            if(drawdown > maxDrawdown){
                maxDrawdown = drawdown;
            }
        }
    }
    return maxDrawdown;
}
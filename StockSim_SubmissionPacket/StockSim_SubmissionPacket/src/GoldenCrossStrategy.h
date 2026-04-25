#include "GoldenCrossStrategy.h"

GoldenCrossStrategy::GoldenCrossStrategy(int shortWindow = 50, int longWindow = 200){
    this->shortWindow = shortWindow;
    this->longWindow = longWindow;
}
SimResult GoldenCrossStrategy::backtest(PriceHistory* history, double monthlyCapital, int startYear, int endYear) override {
    CircularQueue ma50Queue = new CircularQueue(50);
    CircularQueue ma200Queue = new CircularQueue(200);

    
    

}
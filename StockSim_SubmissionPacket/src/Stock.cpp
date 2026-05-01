#include "Stock.h"
#include "CSVParser.h"

Stock::Stock(const string& ticker, const string& name, const string& sector)
        : FinancialAsset(ticker, name, sector){
            history = nullptr;
}
Stock::~Stock(){
    delete history;
}

bool Stock::loadFromCSV(const string& filename){
    history = CSVParser::loadHistory(filename);
    if(history == nullptr){
        return false;
    }
    cout << "Loaded " << history->getSize() << " days" << endl;
    return true;
}

PriceHistory* Stock::getHistory() const{
    return history;
}
double Stock::getPriceOnDate(const string& date) const{
    if (history == nullptr) {
        return -1.0;
    }
    if(history->findByDate(date) != nullptr){
       return history->findByDate(date)->close;
    }else{
        return -1.0;
    }
}
double Stock::getYearStartPrice(int year) const{
    if (history == nullptr) {
        return -1.0;
    }
   
    for(PriceHistory::Iterator it = history->begin(); it != history->end(); ++it){
        PriceNode& node = *it;
        if(CSVParser::extractYear(node.date) == year){
            return node.close;    
        }
    }
    return -1.0;
}
double Stock::getYearEndPrice(int year) const{
     if (history == nullptr) {
        return -1.0;
    }
    
    for(PriceHistory::ReverseIterator it = history->rbegin(); it != history->rend(); ++it){
        PriceNode& node = *it;
        if(CSVParser::extractYear(node.date) == year){
            return node.close;    
        }
    }
    return -1.0;
}
double Stock::calculateAnnualReturn(int year) const{
    if(history == nullptr){
        return 0.0;
    }
    double start = getYearStartPrice(year);
    double end = getYearEndPrice(year);
    if(start == -1.0 || end == -1.0){
        return 0.0;
    }
    return ((end - start) / start) * 100;
}
void Stock::printSummary() const {
    cout << "Ticker  : " << getTicker() << endl;
    cout << "Name    : " << getName()   << endl;
    cout << "Sector  : " << getSector() << endl;

    if (history == nullptr) {
        cout << "No price data available" << endl;
        return;
    }

    PriceNode* head = history->getHead();
    PriceNode* tail = history->getTail();

    cout << "Range   : " << head->date << " - " << tail->date << endl;
    cout << "Days    : " << history->getSize() << " trading days" << endl;

    double totalReturn = 0.0;
    if (head->close != 0.0) {
        totalReturn = ((tail->close - head->close) / head->close) * 100.0;
    }
    cout << "Return  : " << totalReturn << "%" << endl;
}
    

string Stock::getType() const {
    return "Stock";
}





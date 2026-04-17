#include "FinancialAsset.h"


FinancialAsset::FinancialAsset(const string& ticker, const string& name, const string& sector){
    this->ticker = ticker;
    this->name = name;
    this->sector = sector;
}

FinancialAsset::~FinancialAsset(){}

string FinancialAsset::getTicker() const{
    return ticker;
}
string FinancialAsset::getName()   const{
    return name;
}
string FinancialAsset::getSector() const{
    return sector;
}
void FinancialAsset::setSector(const string& sector){
    this->sector = sector;
}








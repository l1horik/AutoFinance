#include "StockData.h"

StockData::StockData(std::string d, double o, double h, double l,
    double c, double adj_c, int v) :
    date(d), open(o), high(h), low(l),
    close(c), adjusted_close(adj_c), volume(v) {
}

std::string StockData::getDate() const { return date; }
double StockData::getOpen() const { return open; }
double StockData::getHigh() const { return high; }
double StockData::getLow() const { return low; }
double StockData::getClose() const { return close; }
double StockData::getAdjustedClose() const { return adjusted_close; }
int StockData::getVolume() const { return volume; }
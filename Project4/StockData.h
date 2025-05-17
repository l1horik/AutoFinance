#pragma once
#include <string>

class StockData {
private:
    std::string date;
    double open, high, low, close, adjusted_close;
    int volume;
public:
    StockData(std::string d = "", double o = 0.0, double h = 0.0, double l = 0.0,
        double c = 0.0, double adj_c = 0.0, int v = 0);

    std::string getDate() const;
    double getOpen() const;
    double getHigh() const;
    double getLow() const;
    double getClose() const;
    double getAdjustedClose() const;
    int getVolume() const;
};
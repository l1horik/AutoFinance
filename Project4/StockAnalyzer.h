#pragma once
#include <vector>
#include "StockData.h"
#include <string>

class StockAnalyzer {
private:
    bool isDisposed = false;
    std::vector<StockData> stockData;

    struct LinearRegression {
        double slope, intercept, r_squared;
    };

    LinearRegression calculateLinearRegression(const std::vector<double>& x,
        const std::vector<double>& y) const;

    
public:
    std::vector<std::string> getDates() const {
        std::vector<std::string> dates;
        for (const auto& data : stockData) {
            dates.push_back(data.getDate());
        }
        return dates;
    }
    std::vector<double> getClosePrices() const {
        std::vector<double> prices;
        for (const auto& data : stockData) {
            prices.push_back(data.getClose());
        }
        return prices;
    }

    size_t getDataSize() const {
        return stockData.size();
    }
    bool loadCSV(const std::string& filename);
    std::vector<double> calculateSMA(int window) const;
    std::string predictPrice(int days_to_predict, int lookback_window = 30) const;
    std::string getAnalysisResults(int window) const;
    ~StockAnalyzer() {
        if (!isDisposed) {
            isDisposed = true;
            // Доп. очистка если есть
        }
    }
};

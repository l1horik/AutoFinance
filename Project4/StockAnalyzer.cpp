#include "StockAnalyzer.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <algorithm>

bool StockAnalyzer::loadCSV(const std::string& filename) {
    stockData.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip header line

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 7) {
            continue; // Skip invalid lines
        }

        try {
            StockData entry(
                tokens[0], // date
                std::stod(tokens[2]), // open
                std::stod(tokens[4]), // high
                std::stod(tokens[1]), // low
                std::stod(tokens[5]), // close
                std::stod(tokens[6]), // adjusted close
                std::stoi(tokens[3])  // volume
            );
            stockData.push_back(entry);
        }
        catch (const std::invalid_argument&) {
            continue; // Skip lines with conversion errors
        }
        catch (const std::out_of_range&) {
            continue; // Skip lines with out of range values
        }
    }

    file.close();
    return !stockData.empty();
}

std::vector<double> StockAnalyzer::calculateSMA(int window) const {
    std::vector<double> sma;
    if (stockData.size() < window) return sma;

    for (size_t i = window - 1; i < stockData.size(); ++i) {
        double sum = 0.0;
        for (int j = 0; j < window; ++j) {
            sum += stockData[i - j].getClose(); // Суммируем окно из 30 дней
        }
        sma.push_back(sum / window);
    }
    return sma;
}

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
    std::getline(file, line); // пропустить первую линию файла

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 7) {
            continue; // Пропустить недействующие строки
        }

        try {
            StockData entry(
                tokens[0], // дата
                std::stod(tokens[2]), // открытый
                std::stod(tokens[4]), // высокий
                std::stod(tokens[1]), // низкий
                std::stod(tokens[5]), // закрытый
                std::stod(tokens[6]), // скорректированное закрытие
                std::stoi(tokens[3])  // величина
            );
            stockData.push_back(entry);
        }
        catch (const std::invalid_argument&) {
            continue; // Пропустить строки с неправильными данными
        }
        catch (const std::out_of_range&) {
            continue; // Пропустить строки за границами значений
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

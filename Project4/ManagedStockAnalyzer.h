#pragma once
#include "StockAnalyzer.h"

public ref class ManagedStockAnalyzer {
private:
    StockAnalyzer* nativeAnalyzer;

public:
    ManagedStockAnalyzer();
    ~ManagedStockAnalyzer();
    !ManagedStockAnalyzer();

    bool LoadCSV(System::String^ filename);
    System::String^ GetAnalysisResults(int window);
    System::String^ PredictPrice(int days_to_predict);
};
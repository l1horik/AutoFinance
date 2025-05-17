#include "ManagedStockAnalyzer.h"
#include <msclr/marshal_cppstd.h>

ManagedStockAnalyzer::ManagedStockAnalyzer() {
    nativeAnalyzer = new StockAnalyzer();
}

ManagedStockAnalyzer::~ManagedStockAnalyzer() {
    this->!ManagedStockAnalyzer();
}

ManagedStockAnalyzer::!ManagedStockAnalyzer() {
    delete nativeAnalyzer;
}

bool ManagedStockAnalyzer::LoadCSV(System::String^ filename) {
    std::string stdFilename = msclr::interop::marshal_as<std::string>(filename);
    return nativeAnalyzer->loadCSV(stdFilename);
}

System::String^ ManagedStockAnalyzer::GetAnalysisResults(int window) {
    std::string result = nativeAnalyzer->getAnalysisResults(window);
    return gcnew System::String(result.c_str());
}

System::String^ ManagedStockAnalyzer::PredictPrice(int days_to_predict) {
    std::string result = nativeAnalyzer->predictPrice(days_to_predict);
    return gcnew System::String(result.c_str());
}
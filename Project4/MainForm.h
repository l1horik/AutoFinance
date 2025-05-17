#pragma once
#include "StockAnalyzer.h"

namespace StockAnalyzerGUI {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Windows::Forms::DataVisualization::Charting;

    public ref class MainForm : public Form {
    private:
        StockAnalyzer* analyzer;
        Button^ btnLoad;
        Button^ btnAnalyze;
        Button^ btnPlot;
        TextBox^ tbOutput;
        StatusStrip^ statusBar;
        Chart^ chart;
        Label^ lblTrend;     
        Label^ lblRecommendation; 
        Panel^ infoPanel;     
        
        String^ AnalyzeTrend();

        void InitializeComponent();
        void btnLoad_Click(Object^ sender, EventArgs^ e);
        void btnAnalyze_Click(Object^ sender, EventArgs^ e);
        void btnPlot_Click(Object^ sender, EventArgs^ e);
        bool ValidateData();

    public:

        MainForm();
        ~MainForm();
    };
}

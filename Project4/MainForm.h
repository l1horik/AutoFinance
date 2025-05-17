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
        Label^ lblTrend;      // Надпись с трендом
        Label^ lblRecommendation; // Рекомендация
        Panel^ infoPanel;     // Панель для информации

        // Метод для анализа тренда
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

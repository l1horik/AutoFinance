#include "MainForm.h"
#include <msclr/marshal_cppstd.h>

using namespace StockAnalyzerGUI;
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::DataVisualization::Charting;

MainForm::MainForm() {
    analyzer = new StockAnalyzer();
    InitializeComponent();
}

MainForm::~MainForm() {
    if (analyzer) {
        delete analyzer;
        analyzer = nullptr;
    }
}

bool MainForm::ValidateData() {
    if (analyzer == nullptr) return false;
    if (analyzer->getDataSize() < 30) {
        MessageBox::Show("��� ���������� ������� ����� ������� 30 ���� ������!",
            "������������ ������",
            MessageBoxButtons::OK,
            MessageBoxIcon::Warning);
        return false;
    }
    return true;
}

String^ MainForm::AnalyzeTrend() {
    if (analyzer == nullptr || analyzer->getDataSize() < 30)
        return "������������ ������ ��� �������";

    auto prices = analyzer->getClosePrices();
    auto sma = analyzer->calculateSMA(30);

    // ������ ��������� ���
    double currentPrice = prices.back();
    double smaValue = sma.back();
    double deviation = (currentPrice - smaValue) / smaValue * 100;

    // ����������� ������
    if (deviation > 5.0) {
        return "������� ���������� ����� (+" + deviation.ToString("F2") + "%)";
    }
    else if (deviation > 0) {
        return "������ ���������� ����� (+" + deviation.ToString("F2") + "%)";
    }
    else if (deviation < -5.0) {
        return "������� ���������� ����� (" + deviation.ToString("F2") + "%)";
    }
    else {
        return "������� ����� (" + deviation.ToString("F2") + "%)";
    }
}

void MainForm::InitializeComponent() {
    this->SuspendLayout();

    // ������ ��������
    this->btnLoad = gcnew Button();
    this->btnLoad->Location = Point(10, 10);
    this->btnLoad->Name = L"btnLoad";
    this->btnLoad->Size = Drawing::Size(120, 30);
    this->btnLoad->Text = L"��������� ����";
    this->btnLoad->Click += gcnew EventHandler(this, &MainForm::btnLoad_Click);

    // ������ �������
    this->btnPlot = gcnew Button();
    this->btnPlot->Location = Point(135, 10);
    this->btnPlot->Name = L"btnPlot";
    this->btnPlot->Size = Drawing::Size(120, 30);
    this->btnPlot->Text = L"��������� ������";
    this->btnPlot->Click += gcnew EventHandler(this, &MainForm::btnPlot_Click);

    //��������� ���� ������
    this->tbOutput = gcnew TextBox();
    this->tbOutput->Location = Point(10, 50);
    this->tbOutput->Multiline = true;
    this->tbOutput->Name = L"tbOutput";
    this->tbOutput->ScrollBars = ScrollBars::Vertical;
    this->tbOutput->Size = Drawing::Size(760, 40);
    this->tbOutput->Font = gcnew System::Drawing::Font("Courier New", 10);
    
    // ��������� �������
    this->chart = gcnew Chart();
    ChartArea^ chartArea = gcnew ChartArea();
    this->chart->ChartAreas->Add(chartArea);

    Series^ seriesClose = gcnew Series("���� ��������");
    seriesClose->ChartType = SeriesChartType::Line;
    seriesClose->Color = Color::Blue;
    this->chart->Series->Add(seriesClose);

    Series^ seriesSMA = gcnew Series("SMA(30)");
    seriesSMA->ChartType = SeriesChartType::Line;
    seriesSMA->Color = Color::Red;
    this->chart->Series->Add(seriesSMA);

    this->chart->Location = Point(10, 95);
    this->chart->Name = L"chart";
    this->chart->Size = Drawing::Size(760, 352);
    this->chart->Visible = false;

    chart->Series["���� ��������"]->Color = Color::Blue;
    chart->Series["���� ��������"]->BorderWidth = 2;
    chart->Series["SMA(30)"]->Color = Color::Red;
    chart->Series["SMA(30)"]->BorderWidth = 2;
    chart->Series["SMA(30)"]->BorderDashStyle = ChartDashStyle::Dash;

    //������ ���
    this->statusBar = gcnew StatusStrip();
    this->statusBar->Items->Add(gcnew ToolStripStatusLabel(L"�����"));
    this->statusBar->Location = Point(0, 520);
    this->statusBar->Name = L"statusBar";
    this->statusBar->Size = Drawing::Size(800, 22);
    
    //��������� ������� �����
    this->ClientSize = Drawing::Size(800, 542);
    this->Controls->Add(this->btnLoad);
    this->Controls->Add(this->btnAnalyze);
    this->Controls->Add(this->btnPlot);
    this->Controls->Add(this->tbOutput);
    this->Controls->Add(this->chart);
    this->Controls->Add(this->statusBar);
    this->Name = L"MainForm";
    this->Text = L"���������� �����";
    this->ResumeLayout(false);
    this->PerformLayout();

    // ������ ��� ����������
    infoPanel = gcnew Panel();
    infoPanel->Location = Point(10, 452);
    infoPanel->Size = Drawing::Size(760, 60);
    infoPanel->BackColor = Color::LightGray;

    // ������� � �������
    lblTrend = gcnew Label();
    lblTrend->Location = Point(10, 10);
    lblTrend->Size = Drawing::Size(740, 20);
    lblTrend->Font = gcnew Drawing::Font("Arial", 10, FontStyle::Bold);
    infoPanel->Controls->Add(lblTrend);

    // ������� � �������������
    lblRecommendation = gcnew Label();
    lblRecommendation->Location = Point(10, 35);
    lblRecommendation->Size = Drawing::Size(740, 20);
    lblRecommendation->Font = gcnew Drawing::Font("Arial", 10, FontStyle::Italic);
    infoPanel->Controls->Add(lblRecommendation);

    this->Controls->Add(infoPanel);
}

void MainForm::btnLoad_Click(Object^ sender, EventArgs^ e) {
    OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
    openFileDialog->Filter = "CSV Files|*.csv|All Files|*.*";

    if (openFileDialog->ShowDialog() == Windows::Forms::DialogResult::OK) {
        try {
            std::string filename = msclr::interop::marshal_as<std::string>(openFileDialog->FileName);

            if (analyzer->loadCSV(filename)) {
                tbOutput->Text = "���� ������� ��������.";
                statusBar->Items[0]->Text = "���� ��������: " + openFileDialog->FileName;
                chart->Visible = false; // �������� ���������� ������
            }
            else {
                tbOutput->Text = "������ �������� �����. ��������� ������.";
                statusBar->Items[0]->Text = "������ ��������";
            }
        }
        catch (Exception^ ex) {
            tbOutput->Text = "������: " + ex->Message;
            statusBar->Items[0]->Text = "����������� ������";
        }
    }
}

void MainForm::btnAnalyze_Click(Object^ sender, EventArgs^ e) {
    try {
        if (analyzer->getDataSize() == 0) {
            tbOutput->Text = "��� ������ ��� �������! ������� ��������� ����.";
            return;
        }

        statusBar->Items[0]->Text = "������ ��������";
    }
    catch (const std::exception& ex) {
        tbOutput->Text = "������ �������: " + gcnew String(ex.what());
        statusBar->Items[0]->Text = "������ �������";
    }
    catch (Exception^ ex) {
        tbOutput->Text = "������: " + ex->Message;
        statusBar->Items[0]->Text = "����������� ������";
    }
}

void MainForm::btnPlot_Click(Object^ sender, EventArgs^ e) {
    if (analyzer == nullptr || analyzer->getDataSize() < 30) {
        MessageBox::Show("��� ���������� ������� ��������� ������� 30 ���� ������",
            "������������ ������",
            MessageBoxButtons::OK,
            MessageBoxIcon::Warning);
        return;
    }

    try {
        // ������� ���������� ������
        chart->Series["���� ��������"]->Points->Clear();
        chart->Series["SMA(30)"]->Points->Clear();

        // �������� ������
        auto dates = analyzer->getDates();
        auto prices = analyzer->getClosePrices();
        auto sma = analyzer->calculateSMA(30);

        // ����������� ���
        chart->ChartAreas[0]->AxisX->Title = "��� ������";
        chart->ChartAreas[0]->AxisY->Title = "���� ($)";
        chart->ChartAreas[0]->AxisX->Interval = 5;
        chart->ChartAreas[0]->AxisX->MajorGrid->LineColor = Color::LightGray;
        chart->ChartAreas[0]->AxisY->MajorGrid->LineColor = Color::LightGray;

        // ��������� ����������� ���� (����� �����)
        for (size_t i = 0; i < prices.size(); ++i) {
            int pointIndex = chart->Series["���� ��������"]->Points->AddXY(i, prices[i]);
            chart->Series["���� ��������"]->Points[pointIndex]->ToolTip =
                String::Format("���� {0}\n{1}\n����: {2:F2}",
                    i + 1, gcnew String(dates[i].c_str()), prices[i]);
        }

        // ��������� SMA (������� ���������� �����)
        for (size_t i = 0; i < sma.size(); ++i) {
            int dayIndex = i + 29; // ���������� �������� ��� SMA(30)
            int pointIndex = chart->Series["SMA(30)"]->Points->AddXY(dayIndex, sma[i]);
            chart->Series["SMA(30)"]->Points[pointIndex]->ToolTip =
                String::Format("���� {0}\n{1}\nSMA(30): {2:F2}",
                    dayIndex + 1, gcnew String(dates[dayIndex].c_str()), sma[i]);
        }

        // ����������� ������� ���
        chart->Series["���� ��������"]->Color = Color::Blue;
        chart->Series["���� ��������"]->BorderWidth = 2;
        chart->Series["SMA(30)"]->Color = Color::Red;
        chart->Series["SMA(30)"]->BorderWidth = 2;
        chart->Series["SMA(30)"]->BorderDashStyle = ChartDashStyle::Dash;

        // �������� ���������������
        chart->ChartAreas[0]->CursorX->IsUserEnabled = true;
        chart->ChartAreas[0]->CursorX->IsUserSelectionEnabled = true;
        chart->ChartAreas[0]->CursorY->IsUserEnabled = true;
        chart->ChartAreas[0]->CursorY->IsUserSelectionEnabled = true;

        chart->Visible = true;

        // ������ � ����� ����������
        String^ trend = AnalyzeTrend();
        lblTrend->Text = "������� �����: " + trend;

        // ������������ ������������
        if (trend->Contains("������� ����������")) {
            lblRecommendation->Text = "������������: �������� (���� ���� SMA �� 5%+)";
            lblRecommendation->ForeColor = Color::Green;
        }
        else if (trend->Contains("������ ����������")) {
            lblRecommendation->Text = "������������: ������� (���� ���� SMA)";
            lblRecommendation->ForeColor = Color::DarkGreen;
        }
        else if (trend->Contains("������� ����������")) {
            lblRecommendation->Text = "������������: ��������� (���� ���� SMA �� 5%+)";
            lblRecommendation->ForeColor = Color::Red;
        }
        else {
            lblRecommendation->Text = "������������: �� ��������� (����� � �����)";
            lblRecommendation->ForeColor = Color::Black;
        }

        chart->Visible = true;
        infoPanel->Visible = true;
    }

    catch (Exception^ ex) {
        MessageBox::Show("������ ���������� �������:\n" + ex->Message,
            "������",
            MessageBoxButtons::OK,
            MessageBoxIcon::Error);
    }
}
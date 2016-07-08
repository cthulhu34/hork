#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>


#include "digitkeyboard.h"
#include "plotoptions.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->centralWidget->setLayout(ui->vlCentral);

//    QFile styleTab;
//    styleTab.setFileName(":/styles/style");
//    styleTab.open(QFile::ReadOnly);
//    QString qssStr = styleTab.readAll();
//    qApp->setStyleSheet(qssStr);

    plotStarted = false;
    //ui->chartView->setTimeRange(600);
    //ui->chartView->setTimeStep(1000);
    //ui->chartView->setAutoYScale(true);

    dataminer = new DataMiner(this);
    timer = new QTimer(this);
    connect (timer, &QTimer::timeout, this, &MainWindow::addDataToPlot);
    connect(dataminer, &DataMiner::errorDetected, this, &MainWindow::errDetect);
    timer->start(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbTemperatureUstavka_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(-70.00, 100.00);
    keyboard->setMessage("Введите значение температуры");

    if(keyboard->exec() == QDialog::Accepted)
    {
        this->setTemperature(keyboard->getValue());
    }
}

void MainWindow::on_pbHumiUstavka_clicked()
{
    DigitKeyboard *keyboard = new DigitKeyboard(this);
    keyboard->setRange(0, 100);
    keyboard->setMessage("Введите значение влажности");

    if(keyboard->exec() == QDialog::Accepted)
    {
        this->setHumidity(keyboard->getValue());
    }

}

int MainWindow::setTemperature(float t)
{
    temperature_u = t;
    ui->pbTemperatureUstavka->setText(QString::number(temperature_u, 'f', 2) + " C" + QChar(176));

    return 0;
}

int MainWindow::setHumidity(float humi)
{
    humidity_u = humi;
    ui->pbHumiUstavka->setText(QString::number(humidity_u, 'f', 2) + " %");

    return 0;
}


void MainWindow::on_pbPlotSettings_clicked()
{
    PlotOptions *options = new PlotOptions(this);

    options->setAutoY(ui->chartView->getAutoY());
    options->setMinY(ui->chartView->getMinY());
    options->setMaxY(ui->chartView->getMaxY());
    options->setGraphicsVisible(ui->chartView->getTVisible(), ui->chartView->getHVisible());
    options->setTimeRange(ui->chartView->getTimeRange());

    if(options->exec() == QDialog::Accepted)
    {
        ui->chartView->setAutoYScale(options->autoY);
        if(!options->autoY)
            ui->chartView->setValuesRange(options->minY, options->maxY);
        ui->chartView->setTimeRange(options->timeRange);
        ui->chartView->setGraphicsVisible(options->showT, options->showH);
    }
}

void MainWindow::on_pbStart_clicked()
{

    ui->chartView->initNewPlot();
    plotStarted = true;

    //ui->chartView->setValuesRange(0, 40);



}

void MainWindow::addDataToPlot()
{
    if(plotStarted)
    {
      ui->chartView->addDataT(dataminer->getCurrentTemperature());
      ui->chartView->addDataH(dataminer->getCurrentHumidity());
    }

    ui->lblTemperatureReal->setText(QString::number(dataminer->getCurrentTemperature(), 'f', 2) + " C" + QChar(176));
    ui->lblHumiReal->setText(QString::number(dataminer->getCurrentHumidity(), 'f', 2) + " %");
}

void MainWindow::errDetect(QString msg)
{
    ui->statusBar->showMessage(msg);
}



void MainWindow::on_pbForwardForward_clicked()
{
    ui->chartView->toEnd();
}

void MainWindow::on_pbForward_clicked()
{
    ui->chartView->screenForward();
}

void MainWindow::on_pbBack_clicked()
{
    ui->chartView->screenBack();
}

void MainWindow::on_pbBackBack_clicked()
{
    ui->chartView->toStart();

}

void MainWindow::on_pbCurrent_clicked()
{
    ui->chartView->returnToOnline();
}

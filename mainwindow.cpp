#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _serialPort(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->btnStart, &QPushButton::clicked,this, &MainWindow::on_btnStart_clicked);
    connect(ui->btnStop, &QPushButton::clicked,this, &MainWindow::on_btnStop_clicked);
    connect(ui->PortInfo, &QPushButton::clicked,this, &MainWindow::on_PortInfo_clicked);

    //TODO: CHANGE THESE VALUES TO CORRECT
    //sets minimum and maximum for the amount of frames per second
    ui->framesPerSecond->setMaximum(1000000000);
    ui->framesPerSecond->setMinimum(0);
    //sets minimum and maximum for the amount time to run
    ui->captureLengthSeconds->setMaximum(1000000000);
    ui->captureLengthSeconds->setMinimum(0);

    loadPorts();

}

MainWindow::~MainWindow()
{
    delete ui;
    if (_serialPort != nullptr) {
        _serialPort->close();
        delete _serialPort;
    }
}



//initialize counters
int framesPerSecond, seconds;

//function happens when START button in the GUI is clicked
void MainWindow::on_btnStart_clicked(){
    //gets values from the GUI
    framesPerSecond = ui->framesPerSecond->text().toInt();
    seconds = ui->captureLengthSeconds->text().toInt();
    //does not continue if invalid arhuments are given
    if (seconds == 0 || framesPerSecond == 0) {
        return;
    }
    //connects to progress bar
    QProgressBar* progressBar = ui->progressBar;
    //sets progress bar range
    progressBar->setRange(0, seconds);
    //initialize at 0%
    progressBar->setValue(0);
    //loop that will send the signals and increment bar
    for (int i = 0; i <= seconds; ++i) {
        // Do some work here
        progressBar->setValue(i); //update the progress bar
        QApplication::processEvents(); //allows process to update
    }
}

//function happens when the stop button is pressed to halt the capture
void MainWindow::on_btnStop_clicked() {
    //TODO: IMPLEMENT

}


void MainWindow::on_PortInfo_clicked() {
    foreach (auto &port, QSerialPortInfo::availablePorts()) {
        qDebug() << port.portName();
    }
}

void MainWindow::loadPorts() {
    foreach (auto &port, QSerialPortInfo::availablePorts()) {
        ui->comboPorts->addItem(port.portName());
    }
}


void MainWindow::on_btnOpenPort_clicked()
{
    if (_serialPort != nullptr) {
        _serialPort->close();
        delete _serialPort;
    }
    _serialPort = new QSerialPort(this);
    _serialPort->setPortName(ui->comboPorts->currentText());
    _serialPort->setBaudRate(QSerialPort::Baud9600);
    _serialPort->setDataBits(QSerialPort::Data8);
    _serialPort->setParity(QSerialPort::NoParity);
    _serialPort->setStopBits(QSerialPort::OneStop);
    _serialPort->setFlowControl(QSerialPort::NoFlowControl);
    if (_serialPort->open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "Result", "Port opened successfully");
    }
    else {
        QMessageBox::critical(this, "Port Error", "Unable to open specified Port...");
    }
}




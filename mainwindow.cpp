#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _serialPort(nullptr)
    , ui(new Ui::MainWindow)
{
    //sets up GUI
    ui->setupUi(this);

    //TODO: CHANGE THESE VALUES TO CORRECT

    //sets minimum and maximum for the amount of frames per second
    ui->framesPerSecond->setMaximum(1000000000);
    ui->framesPerSecond->setMinimum(0);

    //sets minimum and maximum for the amount time to run
    ui->captureLengthSeconds->setMaximum(1000000000);
    ui->captureLengthSeconds->setMinimum(0);

    //calls function to load the ports into the drop down menu
    on_btnRefreshPorts_clicked();
}

//deletes the window and disconnects the serial port
MainWindow::~MainWindow() {
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

    //does not continue if invalid arguments are given
    if (seconds == 0 || framesPerSecond == 0) {
        return;
    }

    //connects to progress bar
    QProgressBar* progressBar = ui->progressBar;

    //sets progress bar range and initialize it at 0%
    progressBar->setRange(0, seconds);
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

//opens up serial port
void MainWindow::on_btnOpenPort_clicked() {
    //makes sure values are not left over from the last port connection
    resetValues();

    //closes any serial port that is already connected
    if (_serialPort != nullptr) {
        _serialPort->close();
        delete _serialPort;
    }

    //sets serial port parameters
    _serialPort = new QSerialPort(this);
    _serialPort->setPortName(ui->comboPorts->currentText());
    _serialPort->setBaudRate(QSerialPort::Baud9600);
    _serialPort->setDataBits(QSerialPort::Data8);
    _serialPort->setParity(QSerialPort::NoParity);
    _serialPort->setStopBits(QSerialPort::OneStop);
    _serialPort->setFlowControl(QSerialPort::NoFlowControl);

    //opens up serial port and connects it to the reader function
    if (_serialPort->open(QIODevice::ReadOnly)) {
        //sends message to user that it has connected successfully
        //   TODO: Replace with a red "not connected" that switches to a green "connected" text
        QMessageBox::information(this, "Result", "Port opened successfully");
        connect(_serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
    }
    //send error message to user
    else {
        QMessageBox::critical(this, "Port Error", "Unable to open specified Port...");
    }
}

//read data from the microcontroller and display it as output
void MainWindow::readData() {
    //returns error if port is disconnected
    if (!_serialPort->isOpen()) {
        QMessageBox::critical(this, "Port Error", "Port is not open");
        return;
    }

    //appends all data sent from the microcontroller to the buffer
    static QByteArray buffer;
    buffer.append(_serialPort->readAll());

    //creates the average based on sampleAverage set in mainwindow.h
    if (samples < sampleAverage) {
        if (buffer.contains("\n")) {
            //convert & trim
            QString data = QString::fromUtf8(buffer).trimmed();
            items = data.split(",");

            //ensure there are at least 3 values
            if (items.size() >= 3) {
                botLeftAvg += items[0].toInt();
                topLeftAvg += items[1].toInt();
                topRightAvg += items[2].toInt();
            }

            //clear the buffer for the next message
            buffer.clear();
            samples++;
        }
    }

    //set the UI to display the average and reset them back to the start
    else {
        ui->botLeftNum->display(botLeftAvg/sampleAverage-zeroBotLeft);
        ui->topLeftNum->display(topLeftAvg/sampleAverage-zeroTopLeft);
        ui->topRightNum->display(topRightAvg/sampleAverage-zeroTopRight);
        botLeftAvg = 0;
        topLeftAvg = 0;
        topRightAvg = 0;
        samples = 0;
    }
}

//sets the zeros
void MainWindow::on_btnZero_clicked() {
    zeroBotLeft += ui->botLeftNum->intValue();
    zeroTopLeft += ui->topLeftNum->intValue();
    zeroTopRight += ui->topRightNum->intValue();
}

//loads the ports into the drop down menu
void MainWindow::on_btnRefreshPorts_clicked() {
    ui->comboPorts->clear();
    foreach (auto &port, QSerialPortInfo::availablePorts()) {
        ui->comboPorts->addItem(port.portName());
    }
}


void MainWindow::on_btnClosPort_clicked() {
    if (_serialPort != nullptr) {
        _serialPort->close();
        delete _serialPort;
        _serialPort = nullptr;
        resetValues();
        QMessageBox::information(this, "Result", "Port closed successfully");
    }
}

//sets values to 0 to prevent any data carrying over to the next usage
void MainWindow::resetValues() {
    zeroTopLeft=0;
    zeroTopRight=0;
    zeroBotLeft=0;

    topLeftAvg = 0;
    topRightAvg = 0;
    botLeftAvg = 0;

    samples = 0;

    ui->botLeftNum->display(0);
    ui->topLeftNum->display(0);
    ui->topRightNum->display(0);
}


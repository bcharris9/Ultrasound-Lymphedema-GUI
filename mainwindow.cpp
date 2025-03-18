#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QThread>
#include <QKeyEvent>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _serialPort(nullptr)
    , ui(new Ui::MainWindow)
{
    //sets up GUI
    ui->setupUi(this);

    ui->framesPerSecond->installEventFilter(this);
    ui->captureLengthSeconds->installEventFilter(this);

    //TODO: CHANGE THESE VALUES TO CORRECT

    //sets minimum and maximum for the amount of frames per second
    ui->framesPerSecond->setRange(0, 1000000000);

    //sets minimum and maximum for the amount time to run
    ui->captureLengthSeconds->setRange(0, 1000000000);

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
    if (seconds <= 0 || framesPerSecond <= 0) {
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

void MainWindow::freezeCapture() {
    //TODO: IMPLEMENT
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
void MainWindow::readData() {
    // Check if the serial port is open
    if (!_serialPort->isOpen()) {
        QMessageBox::critical(this, "Port Error", "Port is not open");
        return;
    }

    // Append incoming data to buffer
    static QByteArray buffer;
    buffer.append(_serialPort->readAll());
    qDebug() << "Received Raw Data: " << buffer;

    // Process the data once we have enough samples
    //if (samples < sampleAverage) {
        // Convert QByteArray to QString and trim any unwanted spaces
        QString data = QString::fromUtf8(buffer).trimmed();

        // Split data by commas to separate the values
        QStringList items = data.split(",");
        qDebug() << "Parsed Data: " << data;
        qDebug() << "Number of items: " << items.size();

        // Ensure there are at least 3 values (one for each sensor)
        if (items.size() == 3) {
            bool ok0, ok1, ok2;
            // Convert the values to integers, check if conversion is successful
            ui->botLeftNum->display(items[0].toInt(&ok0) - zeroBotLeft);
            ui->topLeftNum->display(items[1].toInt(&ok0) - zeroTopLeft);
            ui->topRightNum->display(items[2].toInt(&ok0) - zeroTopRight);

        }
        else {
            qDebug() << "Insufficient data. Expected 3 values, received: " << items.size();
        }

        // Clear the buffer after processing the data
        buffer.clear();
    //}

    // Once we have enough samples, update the UI with averages

}

//read data from the microcontroller and display it as output
/*void MainWindow::readData() {
    //returns error if port is disconnected
    if (!_serialPort->isOpen()) {
        QMessageBox::critical(this, "Port Error", "Port is not open");
        return;
    }

    //appends all data sent from the microcontroller to the buffer
    static QByteArray buffer;
    buffer.append(_serialPort->readAll());
    qDebug() << "Received Raw Data: " << buffer;

    //creates the average based on sampleAverage set in mainwindow.h
    if (samples < sampleAverage) {
        //if (buffer.contains("")) {
            //convert & trim
            QString data = QString::fromUtf8(buffer).trimmed();
            //int endIndex = buffer.indexOf("\r\n");
            //QString data = QString::fromUtf8(buffer.left(endIndex)).trimmed(); // Extract a full line
            //buffer.remove(0, endIndex + 2); // Remove processed data from buffer
            qDebug() << "Parsed Data: " << data; // Debug parsed values

            items = data.split(",");
            qDebug() << items[0];
            qDebug() << items[1];
            qDebug() << items[2];

            qDebug() << items.size();
            //ensure there are at least 3 values
            if (items.size() >= 3) {
                botLeftAvg += items[0].toInt();
                topLeftAvg += items[1].toInt();
                topRightAvg += items[2].toInt();
            }

            //clear the buffer for the next message
            buffer.clear();
            samples++;
        //}
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
}*/

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

void MainWindow::keyPressEvent(QKeyEvent *event)
{

    if (event->isAutoRepeat()) {
        return;
    }

    else if (!event->isAutoRepeat()) {
        keys.clear();
        keys.append(event->key());
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        return;
    }

    keys.append(event->key());
    if (keys.size() == 3) {
        if (keys[0] == 16777250 && keys[1] == 16777250) {
            if (keys[2] == 56) {
                on_btnStart_clicked();
            }

            else if (keys[2] == 57) {
                freezeCapture();
            }

            else if (keys[2] == 48) {
                on_btnStop_clicked();
            }

        }
        keys.clear();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if ((obj == ui->framesPerSecond || obj == ui->captureLengthSeconds) && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        // Debug output to see what actually arrives:
        //qDebug() << "KeyPress:" << keyEvent->key() << "modifiers:" << keyEvent->modifiers();

        // Check if it's Ctrl+8
        if ((keyEvent->key() == Qt::Key_8 | keyEvent->key() == Qt::Key_9 | keyEvent->key() == Qt::Key_0) &&
            (keyEvent->modifiers() & (Qt::ControlModifier | Qt::MetaModifier))) {
            // Ignore pedal input
            return true;
        }
    }
    // Otherwise, let the default behavior happen
    return QMainWindow::eventFilter(obj, event);
}




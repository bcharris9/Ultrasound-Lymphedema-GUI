#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>  // For QThread::msleep()

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QMessageBox>

#include <QKeyEvent>

#include <QVector>

class CsvWriter : public QThread {
    Q_OBJECT

public:
    explicit CsvWriter(QObject *parent = nullptr);
    ~CsvWriter();

    void writeData(int botLeftAvg, int topLeftAvg, int topRightAvg);
    void stop();
    void setParameters(double fps, int duration);

    int topLeftAvg, topRightAvg, botLeftAvg;

protected:
    void run() override;

private:
    QFile file;
    QTextStream stream;
    bool running;
    double framesPerSecond;
    int captureDuration;



signals:
    void requestData(); // Signal to request data from the UI


};

#endif // CSVWRITER_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "csvwriter.h"

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


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void sendSensorData();


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void freezeCapture();

    void on_btnOpenPort_clicked();

    void on_btnZero_clicked();

    void on_btnRefreshPorts_clicked();

    void readData();

    void on_btnClosPort_clicked();

    //void keyPressEvent(QKeyEvent *event);
    //void keyReleaseEvent(QKeyEvent *event);

    //void on_framesPerSecond_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    QSerialPort *_serialPort;

    QStringList items;

    int zeroTopLeft=0, zeroTopRight=0, zeroBotLeft=0;

    int topLeftAvg = 0, topRightAvg = 0, botLeftAvg = 0;

    //sampling rate
    const int sampleAverage = 500;

    int samples = 0;

    void resetValues();

    QVector<int> keys;

    int Buffer;

    CsvWriter *csvWriter;

};



#endif // MAINWINDOW_H

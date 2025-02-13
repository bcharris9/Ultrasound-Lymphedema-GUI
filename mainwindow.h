#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>  // For QThread::msleep()

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QMessageBox>



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
private slots:

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnOpenPort_clicked();

    void on_btnZero_clicked();

    void on_btnRefreshPorts_clicked();

    void readData();

    void on_btnClosPort_clicked();

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
};


#endif // MAINWINDOW_H

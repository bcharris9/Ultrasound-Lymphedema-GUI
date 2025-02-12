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

    void on_PortInfo_clicked();

    void on_btnOpenPort_clicked();

private:
    Ui::MainWindow *ui;

    void loadPorts();

    QSerialPort *_serialPort;

};


#endif // MAINWINDOW_H

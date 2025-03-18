#include "csvwriter.h"
#include <QDateTime>
#include <QDebug>

CsvWriter::CsvWriter(QObject *parent)
    : QThread(parent), framesPerSecond(0), captureDuration(0), running(true), file(QString("/Users/blake/Desktop/sensor_data_%1.csv")
               .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss")))
 {

    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        stream.setDevice(&file);
    } else {
        qDebug() << "Failed to open file for appending.";
    }
}

CsvWriter::~CsvWriter() {
    stop();
    if (file.isOpen()) {
        file.close();
    }
}

void CsvWriter::setParameters(double fps, int duration) {
    framesPerSecond = fps;
    captureDuration = duration/framesPerSecond;
}

void CsvWriter::run() {
    if (!file.isOpen()) {
        qDebug() << "File not open, unable to write.";
        return;
    }

    qDebug() << "Starting CSV writing...";

    running = true;
    stream << "TIME, TOP LEFT, TOP RIGHT, BOTTOM RIGHT\n";
    stream.flush();

    for (int i = 0; i < captureDuration; ++i) {
        if (!running) break;

        emit requestData();

        stream << i*framesPerSecond << "," << topLeftAvg << "," << topRightAvg << "," << botLeftAvg << "\n";
        stream.flush();

        QThread::msleep(framesPerSecond*1000);  // Simulate per-second data capture
    }

    qDebug() << "CSV writing completed.";
}

void CsvWriter::writeData(int botLeftAvg, int topLeftAvg, int topRightAvg) {
    if (file.isOpen()) {
        qDebug() << "Writing data to CSV...";
        stream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << ","
               << botLeftAvg << "," << topLeftAvg << "," << topRightAvg << "\n";
        stream.flush();
    }

    else {
        qDebug() << "Error: CSV file not open!";
    }
}

void CsvWriter::stop() {
    running = false;
    wait();  // Ensures the thread stops before destruction
}

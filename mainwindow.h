#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCore>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QByteArray>

#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);
    void about();
    void readData();
    void writeData();
    void timePC();

private:
    void initActionConnection();

private:
    Ui::MainWindow *ui;
    //
    SettingsDialog *settings;
    QSerialPort *serial;
    QTimer *timer;
};

#endif // MAINWINDOW_H

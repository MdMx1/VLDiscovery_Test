#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new SettingsDialog;
    serial   = new QSerialPort(this);
    timer    = new QTimer(this);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);
    ui->synchroButton->setEnabled(false);

    initActionConnection();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    //connect(ui->synchroButton, SIGNAL(clicked()), this, SLOT(wtiteData()));
    connect(timer, SIGNAL(timeout()),this, SLOT(timePC()));
    timer->start(500);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

// =====================================================================================================================

void MainWindow::initActionConnection()
{
    // ====
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    // ====
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    // ====
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

// ---------------------------------------------------------------------------------------------------------------------

void MainWindow::about()
{
    QMessageBox::about(this, tr("О программе VLDiscovery..."),
                       tr("<b>STM32VLDiscovery</b>"));
}

// ---------------------------------------------------------------------------------------------------------------------

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();

    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    if (serial->open(QIODevice::ReadWrite))
    {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        ui->synchroButton->setEnabled(true);
        //???
        ui->statusBar->showMessage(tr("Соединение с %1 : %2, %3, %4, %5, %6")
                                   .arg(p.name).arg(p.stringBaudRate)
                                   .arg(p.stringDataBits).arg(p.stringParity)
                                   .arg(p.stringStopBits).arg(p.stringFlowControl));
        //Write!!!!!!!!!!
        connect(timer, SIGNAL(timeout()),this, SLOT(writeData()));//TEST!!!!!!!!!!!! на себя
    }
    else
    {
        QMessageBox::critical(this, tr("Ошибка"), serial->errorString());
        ui->statusBar->showMessage(tr("Ошибка соединения..."));
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
    {
        serial->close();
    }
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->synchroButton->setEnabled(false);
}

// ---------------------------------------------------------------------------------------------------------------------

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        QMessageBox::critical(this, tr("Критическая ошибка"), serial->errorString());
        closeSerialPort();
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void MainWindow::readData()
{
    QByteArray data;/* = serial->readAll();
    while (serial->waitForReadyRead(10))
    {*/
        data.append(serial->readAll());
    //}

    ui->test->insertPlainText(QString(data.toHex()));

    qDebug() << data.size() << data.toHex();

    data.clear();
}

// ---------------------------------------------------------------------------------------------------------------------

void MainWindow::writeData()
{
    QDateTime dataTime = QDateTime::currentDateTime();
    QString str;
    QByteArray data;

    str = dataTime.toString("dd");
    data += str.toInt();
    str = dataTime.toString("MM");
    data += str.toInt();
    str = dataTime.toString("yyyy");
    data += str.toInt()/100;
    data += str.toInt()%100;
    str = dataTime.toString("hh");
    data += str.toInt();
    str = dataTime.toString("mm");
    data += str.toInt();
    str = dataTime.toString("ss");
    data += str.toInt();

    serial -> write(data);
    data.clear();
}

// ---------------------------------------------------------------------------------------------------------------------

void MainWindow::timePC()
{
    QDateTime dataTime = QDateTime::currentDateTime();
    ui->timeDataPcLabel->setText(dataTime.toString("hh:mm:ss - dd.MM.yyyy"));
}

// ---------------------------------------------------------------------------------------------------------------------

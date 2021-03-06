#include "settingsdialog.h"
#include "ui_settingsdialog.h"

QT_USE_NAMESPACE

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "Неизвестно");

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    intValidator = new QIntValidator(0, 4000000, this);
    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->portBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));
    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkCustomBaudRatePolicy(int)));
    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkCustomDevicePathPolicy(int)));

    fillPortsInfo();
    fillPortsParameters();
    updateSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

SettingsDialog::Settings SettingsDialog::settings() const
{
    return currentSettings;
}

// =====================================================================================================================

void SettingsDialog::fillPortsInfo()
{
    ui->portBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QStringList list;
        description  = info.description();
        manufacturer = info.manufacturer();
        serialNumber  = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString)   ;

        ui->portBox->addItem(list.first(), list);
    }
    ui->portBox->addItem(tr("Custom"));
}

void SettingsDialog::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("Нет"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Нечет"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Чет"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Маркер"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Пробел"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
  #ifdef O_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
  #endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("Нет"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("Xon/Xoff"), QSerialPort::SoftwareControl);
}

void SettingsDialog::updateSettings()
{
    currentSettings.name = ui->portBox->currentText();
    //====
    if(ui->baudRateBox->currentIndex() == 4)
    {
        currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    }
    else
    {
        currentSettings.baudRate = static_cast <QSerialPort::BaudRate> (
                ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);
    //====
    currentSettings.dataBits = static_cast <QSerialPort::DataBits> (
                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();
    //====
    currentSettings.parity = static_cast <QSerialPort::Parity> (
                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityBox->currentText();
    //====
    currentSettings.stopBits = static_cast <QSerialPort::StopBits> (
                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();
    //====
    currentSettings.flowControl = static_cast <QSerialPort::FlowControl> (
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();
}

//----------------------------------------------------------------------------------------------------------------------

void SettingsDialog::showPortInfo(int idx)
{
    if (idx == -1)
    {
        return;
    }

    QStringList list = ui->portBox->itemData(idx).toStringList();

    ui->descriptionLabel->setText(tr("Модель: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->manufacturerLabel->setText(tr("Изготовитель: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->serialNumberLabel->setText(tr("Номер модели: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    ui->locationLabel->setText(tr("Размещение: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    ui->vidLabel->setText(tr("VID: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    ui->pidLabel->setText(tr("PID: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SettingsDialog::apply()
{
    updateSettings();
    hide();
}

void SettingsDialog::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate)
    {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void SettingsDialog::checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !ui->portBox->itemData(idx).isValid();
    ui->portBox->setEditable(isCustomPath);
    if (isCustomPath)
    {
        ui->portBox->clearEditText();
    }
}

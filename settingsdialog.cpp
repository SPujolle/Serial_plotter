#include "settingsdialog.h"

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");


settingsDialog::settingsDialog(QWidget * parent) :
    QDialog(parent),
    m_serialInfo(new QList<QSerialPortInfo>(QSerialPortInfo::availablePorts())),
    m_serialPortComboBox(new(QComboBox)),
    m_refresh(new QPushButton  ("Refresh port list")),
    manufLabel(new QLabel),
    serialNumLabel(new QLabel),
    vendorLabel(new QLabel),
    productLabel(new QLabel),
    baudrateComboBox(new(QComboBox)),
    dataBitsComboBox(new(QComboBox)),
    parityComboBox(new(QComboBox)),
    stopBitsComboBox(new(QComboBox)),
    flowControlComboBox(new(QComboBox)),
    m_intValidator(new QIntValidator(0, 4000000, this))


{
    this->setWindowTitle(tr("Port selection"));

    QGridLayout *grid = new(QGridLayout);  this->setLayout(grid);

    QGroupBox *selectPortGroup = new QGroupBox("Select port");
    QGroupBox *selectParmaGroup = new QGroupBox("Select parameters");
    QPushButton *applyButton = new QPushButton("Connect");
    QPushButton *disconnectButton = new QPushButton("Disconnect");
    QPushButton *closeButton = new QPushButton("Close");

    grid->addWidget(selectPortGroup,0,0);
    grid->addWidget(selectParmaGroup,0,1);

    QGridLayout *topLeftLayout = new QGridLayout;
    selectPortGroup->setLayout(topLeftLayout);
    topLeftLayout->addWidget(m_refresh,0,0);
    topLeftLayout->addWidget(m_serialPortComboBox,1,0);
    topLeftLayout->addWidget(manufLabel,2,0);
    topLeftLayout->addWidget(serialNumLabel,3,0);
    topLeftLayout->addWidget(vendorLabel,4,0);
    topLeftLayout->addWidget(productLabel,5,0);

    QGridLayout *topRightLayout = new QGridLayout;
    selectParmaGroup->setLayout(topRightLayout);
    QLabel *lBaud = new QLabel("Baud rate : ");
    topRightLayout->addWidget(lBaud, 0,0);  topRightLayout->addWidget(baudrateComboBox,0,1);
    baudrateComboBox->setCurrentIndex(-1);
    baudrateComboBox->setInsertPolicy(QComboBox::InsertAtBottom);
    QLabel *lData = new QLabel("Data bits : ");
    topRightLayout->addWidget(lData, 1,0); topRightLayout->addWidget(dataBitsComboBox,1,1);
    QLabel *lParity = new QLabel("Parity : ");
    topRightLayout->addWidget(lParity, 2,0); topRightLayout->addWidget(parityComboBox,2,1);
    QLabel *lStop = new QLabel("Stop bits : ");
    topRightLayout->addWidget(lStop, 3,0); topRightLayout->addWidget(stopBitsComboBox,3,1);
    QLabel *lFlow = new QLabel("Flow control : ");
    topRightLayout->addWidget(lFlow, 4,0); topRightLayout->addWidget(flowControlComboBox,4,1);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    grid->addLayout(bottomLayout,1,0,3,Qt::AlignJustify);
    bottomLayout->addWidget(applyButton);
    bottomLayout->addWidget(disconnectButton);
    bottomLayout->addWidget(closeButton);

    fillPortsParameters();

    collectPortInfos(1);

    connect(m_refresh, SIGNAL(clicked(bool))  , this, SLOT(collectPortInfos(bool)));
    connect(m_serialPortComboBox , QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &settingsDialog::displayPortsInfos );

    connect(applyButton, SIGNAL(clicked(bool))  , this, SLOT(sendSettings(bool)));
    connect(disconnectButton, SIGNAL(clicked(bool))  , this, SLOT(disconnectCommand(bool)));
    connect(closeButton  ,SIGNAL(clicked(bool))  ,this  ,SLOT(close())  );


    connect(baudrateComboBox,  QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &settingsDialog::checkCustomBaudRatePolicy);

}
settingsDialog::~settingsDialog()
{

}
// ================================================================//
void settingsDialog::collectPortInfos(bool )
{
    m_serialPortComboBox ->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos =  QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        m_serialPortComboBox->addItem( list.first()+" | "+description, list );
    }
}
// ================================================================//
void settingsDialog::displayPortsInfos(int idx){
    const QStringList list = m_serialPortComboBox->itemData(idx).toStringList();
    manufLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    serialNumLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    vendorLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    productLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

// ================================================================//

void settingsDialog::fillPortsParameters()
{
    baudrateComboBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    baudrateComboBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    baudrateComboBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    baudrateComboBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    baudrateComboBox->addItem(tr("Custom"));

    dataBitsComboBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    dataBitsComboBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    dataBitsComboBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    dataBitsComboBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    dataBitsComboBox->setCurrentIndex(3);

   parityComboBox->addItem(tr("None"), QSerialPort::NoParity);
   parityComboBox->addItem(tr("Even"), QSerialPort::EvenParity);
   parityComboBox->addItem(tr("Odd"), QSerialPort::OddParity);
   parityComboBox->addItem(tr("Mark"), QSerialPort::MarkParity);
   parityComboBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    stopBitsComboBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    stopBitsComboBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    stopBitsComboBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    flowControlComboBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    flowControlComboBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    flowControlComboBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

}

void settingsDialog::checkCustomBaudRatePolicy(int idx)
{
    const bool isCustomBaudRate = !baudrateComboBox->itemData(idx).isValid();
    baudrateComboBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        baudrateComboBox->clearEditText();
        QLineEdit *edit = baudrateComboBox->lineEdit();
        edit->setValidator(m_intValidator);
    }
}
// ================================================================//
void settingsDialog::sendSettings(bool){

    QStringList list;
    QVariant data  = m_serialPortComboBox->currentData();
    list = data.toStringList();
    currentSettings.name = list.at(0);

    if (baudrateComboBox->currentIndex() == 4) {
        currentSettings.baudRate = baudrateComboBox->currentText().toInt();
    } else {

    currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                baudrateComboBox->itemData(baudrateComboBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    auto da = dataBitsComboBox->itemData(dataBitsComboBox->currentIndex()).toInt();
    currentSettings.dataBits = static_cast<QSerialPort::DataBits> (da);
    currentSettings.stringDataBits = dataBitsComboBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                parityComboBox->itemData(parityComboBox->currentIndex()).toInt());

    currentSettings.stringParity = parityComboBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
               stopBitsComboBox->itemData(stopBitsComboBox->currentIndex()).toInt());
    currentSettings.stringStopBits = stopBitsComboBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                flowControlComboBox->itemData(flowControlComboBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = flowControlComboBox->currentText();


    emit settingsApplied(1);
}
void settingsDialog:: disconnectCommand(bool){
    emit disconnectSignal (1);
}

settingsDialog::Settings    settingsDialog::settings()  {
    return currentSettings;
}

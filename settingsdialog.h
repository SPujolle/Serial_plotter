#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>

QT_BEGIN_NAMESPACE

namespace Ui {
class settingsDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class settingsDialog : public QDialog
{
    Q_OBJECT
public:
    settingsDialog(QWidget *parent = nullptr);

    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
         QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };
 Settings settings()  ;


private :
    ~settingsDialog();
    QList<QSerialPortInfo>* m_serialInfo = nullptr;
    QComboBox *m_serialPortComboBox = nullptr;
    QPushButton *m_refresh  = nullptr ;
    QLabel *manufLabel;
    QLabel *serialNumLabel;
    QLabel *vendorLabel;
    QLabel *productLabel;
    QComboBox *baudrateComboBox = nullptr;
    QComboBox *dataBitsComboBox = nullptr;
    QComboBox *parityComboBox = nullptr;
    QComboBox *stopBitsComboBox = nullptr;
    QComboBox *flowControlComboBox = nullptr;
    QIntValidator *  m_intValidator;

    Settings currentSettings ;
    void fillPortsParameters();




signals :
    void settingsApplied(bool);
    void disconnectSignal(bool);

public slots:

private slots :
    void  collectPortInfos(bool);
    void displayPortsInfos(int);
    void sendSettings(bool);
    void checkCustomBaudRatePolicy(int);
    void disconnectCommand(bool);

};

#endif // SETTINGSDIALOG_H

#ifndef CONSOLE_H
#define CONSOLE_H
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QDebug>

class Console : public QPlainTextEdit
{
    Q_OBJECT
signals:
    void getData(const QByteArray &data);

public:
    explicit Console(QWidget *parent = nullptr);

    void putData(const QByteArray &data);
    void setLocalEchoEnabled(bool set);

private:
    bool m_localEchoEnabled = false;
};
// ============================================== //
//        CLASSE FULLCONSOLE                      //
// ============================================== //


class fullConsole : public QWidget
{
    Q_OBJECT

public :
    fullConsole(QWidget * parent = nullptr);

private :
    Console * visuConsole = nullptr ;
    QLineEdit * sendDatasLine ;
    QPushButton * sendButton ;
    QComboBox * newlineChooser ;
    QCheckBox * deleteAfter ;



public slots :
    void clear(bool){
        visuConsole->clear();
    };
    void putData(QByteArray & D){
        visuConsole->putData(D);
    };
    void getDataFromLine();
signals :
    void getData(QByteArray &);


};



#endif // CONSOLE_H

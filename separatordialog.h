#ifndef SEPARATORDIALOG_H
#define SEPARATORDIALOG_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QCheckBox>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDockWidget>
#include <QLineEdit>
#include <QDebug>




namespace SEPARATORS {

const int codes[] = {1,2,4,8,12};
const char sepTable[5]={',' , ';' , static_cast<char>(32) , static_cast<char>(9) , static_cast<char>(127)};
const QString buttonName[5] = {"comma","semicolon","SPACE","TAB","Any space char"};
// 32 = SPACE , 9 = TAB , 127 = DEL

}




class separatorDialog : public QDialog
{
    Q_OBJECT

public:
    separatorDialog(QWidget * parent);
    int separatorCode = 0 ;
    char firstSeparator = static_cast<char>(0);
    bool test= false ;
    int testCode = 0 ;
    char testSeparator = static_cast<char>(0);


private :

    QCheckBox buttons[5] ;
    QLineEdit * customCharBox ;
    QByteArray  customChar ;
    QPushButton *Test ;
    QGridLayout *Layout = nullptr;
     QHBoxLayout * H1box = nullptr ;
    QLabel * fieldsN = nullptr ;
    QList<QCheckBox*> fieldCheckList ;

    int channelCode = 0;
    void disableCheckBox();

public slots :
    void channelSelected(int C){
        channelCode = C ;
        disableCheckBox();
        qDebug()<< "DISABLE ->Channel code = " + QString::number(C);
    }

private slots :
    int computeSepCode(int);
    void testOnOff();



signals :
    void channelChanged(int);


};

#endif // SEPARATORDIALOG_H

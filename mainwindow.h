//===============================================================//
// MULTI TOOL SERIAL CONSOLE/PLOTTER/LOGGER                      //
// VERSION 0.1 BETA                                              //
//===============================================================//



#ifndef MAINWINDOW_H
#define MAINWINDOW_H





#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QStackedWidget>
#include <QSplitter>
#include <QtDebug>

#include "console.h"
#include "settingsdialog.h"
#include "fileconfirmdialog.h"
#include "separatordialog.h"
#include "plotterwidget.h"
#include "chansettingwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class rollingBuffer{

public :
    rollingBuffer(int);
    ~rollingBuffer();

    int currentIndex = 0;
    int size = 0 ;

    int incIndex();
    int decIndex();
    int incIndex(int);
    int decIndex(int);
    int setIndex(int I);
    char * curValue() ;
    char * data =nullptr;


private :
    int indMax ;

};





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    settingsDialog * settWindow ;
    QLabel *status = nullptr;
    QSerialPort *serial = nullptr;

    Console *console = nullptr;
    separatorDialog  *sepDialog = nullptr;

    scopeWindow *plot ;
    QDockWidget *dock ;

    QStackedWidget *centralStack = nullptr ;
    QSplitter * centralSplit = nullptr ;
    QAction *record = nullptr ;
    QAction *stop = nullptr ;
    QAction *closeFile= nullptr;

    QAction *consoleWiew = nullptr;
    QAction *clearConsole = nullptr;
    QAction *graphWiew = nullptr;
    QAction *clearGraph = nullptr;

    QByteArray data;
    QByteArray trame ;
    QList<QByteArray> listDatas ;
    QFile file ;
    QString curFile ="";
    int writteMode = 0;
    QString writteModeString[3] = {"N/A","overwritte","append"};
    QString statPart1 = "No port selected";
    QString statPart2 ="No output file";
    bool outFileready = false ;
    bool recordOnOff = false ;
    int separatorCode = 0 ;

    rollingBuffer * RB ;


private :
    void accTrame();
    int splitDatas(QByteArray &);
    void showStatusMessage(const QString &part1, bool);
    void computeStatusMessage();

private slots:

    //bool switchConsoleGraph(bool);

public slots:

    void  moveEvent(QMoveEvent *e) {
        plot->parentMoved(true);
    }
    void acceptSettings(bool);
    void openSerialPort();
    void closeSerialPort(bool);
    void readData();
    bool selectOutFile();
    bool saveFile(bool);
    bool switchRecordOnOff(bool);
    bool CloseFile(bool);
    void chooseSeparator(bool);
    void putConsole(QByteArray &);

signals :
    void fieldsNumber(int);
    void newDatas(QList<QByteArray> &);
    void newTrame(QByteArray &);

};





#endif // MAINWINDOW_H

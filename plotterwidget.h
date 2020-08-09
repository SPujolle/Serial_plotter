#ifndef PLOTTERWIDGET_H
#define PLOTTERWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLine>
#include <QPainter>
#include <QList>
#include <QDebug>
#include <QRect>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDockWidget>
#include <QCheckBox>
#include <QLabel>
#include <QDebug>
#include "chansettingwindow.h"

#define COLORNUMBER 7



//==================================================================//
//                CLASS CHANNELSETTINGS                             //
//==================================================================//

class chanSettings
{
public :
    float gain = 1 ;
    float offset = 0 ;

};

//==================================================================//
//                CLASS PANELSET                                    //
//==================================================================//

class panelSet : public QFrame
{
    Q_OBJECT
public :
    //~panelSet();
    panelSet(int i , QColor * colorList, QWidget * parent);
    QCheckBox * selectBox = nullptr ;
    QPushButton * setBut = nullptr ;
    chanSettingWindow * cSetWin = nullptr ;
    chanSettings cSetVals ;

    void showEvent(QShowEvent * );
    void moveEvent(QMoveEvent * );
private :
    QVBoxLayout * vBox = nullptr ;
     QPoint oldP ;

public slots:
    void repos(bool);
private :
    int chanNum ;

private slots :
    void butClicked(bool);
    void setGain(double G);
    void setOffset(double G);

signals :
    //int channelNum(int);
    int cValsChanged(int);
};

//==================================================================//
//                CLASS SELECTOR WINDOW                             //
//==================================================================//

class selectorWindow : public QWidget
{
      Q_OBJECT
public :
    selectorWindow(QWidget *);
    void refillChekbox();
    void showEvent(QShowEvent * e);

public slots :
    void repos();
    void channelSelector(int, QColor *);

   // int computeChannelGain(int);

private slots :
    int computeChannelSelect(int);
    void setGain(int N);
signals :
    void channelChanged(int);
    void channelNumOpen(int);
    void newSettings(QList<panelSet*> &);

private:
    QHBoxLayout * Hlayout = nullptr;
    QList<panelSet *> panelList ;
    int channelCode =0 ;
};


//=========================================================//
class trace{

public :

    trace(int N){
        chanNum = N ;

    }

    int chanNum  = 0 ;
    QByteArray vals[1000];
    float maxRange = 100 ;
    QColor color ;

};

//==================================================================//
//                CLASS PLOTTERWIDGET                               //
//==================================================================//


class plotterWidget : public QWidget
{
    Q_OBJECT
public:
    plotterWidget(QWidget * parent = 0 );
    void paintEvent(QPaintEvent *event);
private :
    QPainter * painter;

    QList<float> validChannel ;
    QList<trace *> traces ;
    QList<panelSet *>  SET ;

    QSpinBox * maxSpin = nullptr ;
    QSpinBox * minSpin = nullptr ;

    int index = 0 ;
    int max = 100;
    int min = -100;
    int range = max - min ;
    int leftMargin = 25 ;
    int rightMargin = 10 ;
    int topMargin = 10 ;
    int bottomMargin = 10 ;
    int bottomP = 0;
    int topP = 0 ;

    int curIndex = 0 ;
    int maxIndex = 1000;

   int channelCode = 0;
   int stepMap[20]={1,2,4,5,
                    10,15,20, 25,
                    40,50,100,150,
                    200,250,400,500,
                    750,1000,1500, 2000};
   int bestStep =100;
   int mapTocadre(float);
   int findBestStep(int);
   void updateGrid();
   void showEvent(QShowEvent * ){
    updateGrid();
   }

public slots :
    void updateGraph(QList<QByteArray> &);
    int allocMemory(int, QColor *);
    void memorizedatas(QList<QByteArray> &);
    void updateSettings(QList<panelSet *> & LIST){
        SET = LIST ;
    }
    void setMax(int M){
        max = M ;
        updateGrid();
    }
    void setMin(int M){
        min = M ;
        updateGrid();
    }

signals :

};







//==================================================================//
//                CLASS SCOPE WINDOW                               //
//==================================================================//

class scopeWindow : public QWidget
{
      Q_OBJECT
public :
    scopeWindow(QWidget * parent):
    QWidget(parent),
    commandWindow(new  selectorWindow(this))
    {
        plotWindow = new plotterWidget(this);

        QVBoxLayout * layout = new QVBoxLayout ;
        this->setLayout(layout);
        layout->addWidget(commandWindow);
        layout->addWidget(plotWindow);

        connect ( commandWindow  ,SIGNAL(channelChanged(int))   ,
                  this, SLOT(channelChanged(int))   );

        connect( commandWindow ,SIGNAL(newSettings(QList<panelSet*> &)  )  ,
                 plotWindow,SLOT( updateSettings   (QList<panelSet*> &)) );

//        connect( commandWindow , SIGNAL(channelNumOpen(int)) ,
//                 this  , SLOT(  channelNumOpen(int)  ) );
    }
    selectorWindow * commandWindow = nullptr ;
    plotterWidget * plotWindow = nullptr ;

public slots :
    void parentMoved(bool){
        commandWindow->repos();

    }
    void channelSelector(int N){
        commandWindow->channelSelector(N,colorList );
    }
    void channelChanged(int C){
        plotWindow->allocMemory(C,colorList);
        emit channelSelected(C);
    }

//    void channelNumOpen(int N){
//        //qDebug() << "GAIN PANEL TOP = " + QString::number(N);
//    }
signals :
    void channelSelected(int C);

private :
    QColor colorList[COLORNUMBER]={QColor(Qt::red) ,
                         QColor(Qt::green),
                         QColor(Qt::blue),
                         QColor(Qt::cyan),
                         QColor(Qt::magenta) ,
                         QColor(Qt::yellow),
                         QColor(Qt::darkBlue)
                        };
    chanSettings cSettings ;
};




//                     SCOPEWINDOW


//   SELECTORWINDOW                      PLOTTERWIDGET



//   PANELSET[0]...PANELSET[n]         TRACE[0]...TRACE[i]


//



#endif // PLOTTERWIDGET_H

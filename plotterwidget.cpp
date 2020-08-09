#include "plotterwidget.h"

//==================================================================//
//                CLASS PANELSET                                    //
//==================================================================//
panelSet:: panelSet(int i , QColor * colorList, QWidget * parent):
    QFrame(parent)
{
    chanNum = i;
    int Cn = i % COLORNUMBER ;

    QString T = tr("Channel N ") + QString::number(i+1);
    cSetWin = new chanSettingWindow(this, T, colorList[Cn]) ;
    this->setSizePolicy(QSizePolicy::Preferred , QSizePolicy::Fixed);

    QString CC = "border: 2px solid " + colorList[Cn].name()  + ";" ;
    this -> setFrameStyle(QFrame::Plain | QFrame::Box);
    this->setObjectName("truc");
    this->setStyleSheet("#truc { " +  CC  + " }");

    vBox = new QVBoxLayout(this);
    this->setLayout(vBox);

    selectBox = new QCheckBox(this) ;
    selectBox->setText(QString::number(i+1));

    setBut = new QPushButton(tr("Parameters"),this) ;
    setBut->setToolTip(tr("Set channel gain, offset and parameters"));
    setBut->show();

    connect( setBut , SIGNAL(clicked(bool)) ,this  ,SLOT( butClicked(bool) )  );

    connect( this->cSetWin->gain ,SIGNAL(valueChanged(double))  ,
             this  ,                  SLOT(setGain(double))  );

    connect( this->cSetWin->offset ,SIGNAL(valueChanged(double))  ,
             this  ,                  SLOT(setOffset(double))  );

    vBox->addWidget(selectBox);
    vBox->addWidget(setBut);
}


void panelSet:: butClicked(bool){
    QPoint O = mapToGlobal(QPoint(0,0));
    oldP = O ;
    cSetWin->move(O);
    cSetWin->show();

}
void panelSet:: setGain(double G){
    cSetVals.gain = G ;
    emit cValsChanged(chanNum);
}
void panelSet:: setOffset(double G){
    cSetVals.offset = G ;
    emit cValsChanged(chanNum);
}

void panelSet:: repos(bool){
    QPoint O = mapToGlobal(QPoint(0,0));
    QPoint P = cSetWin->pos();
    cSetWin->move(P + O - oldP);
   oldP  = O ;
  //qDebug() << "REPOSIT ";
}

void panelSet:: showEvent(QShowEvent * ){
    this->cSetWin->gain->setValue(cSetVals.gain);
    this->cSetWin->offset->setValue(cSetVals.offset);

}
void panelSet:: moveEvent(QMoveEvent * ) {
    repos(true);
}

//==================================================================//
//                CLASS PLOTTERWIDGET                               //
//==================================================================//
plotterWidget::plotterWidget(QWidget * parent) :
    QWidget(parent),
    maxSpin(new QSpinBox(this)),
    minSpin(new QSpinBox(this))

{
        painter = new QPainter(this);
        this->setMinimumSize(QSize(100,100));

        this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Ignored);
        maxSpin->setRange(min + 10,1000);
        minSpin->setRange(-10000,max - 10);

        maxSpin->setValue(max);
        minSpin->setValue(min);

        connect(maxSpin  ,SIGNAL(valueChanged(int))  , this  ,  SLOT( setMax(int)));
        connect(minSpin  ,SIGNAL(valueChanged(int))  , this  ,  SLOT( setMin(int)));

}

void plotterWidget::paintEvent(QPaintEvent *)
{
    QPen pen;  // creates a default pen
    QRect plotCadre =  rect().adjusted(leftMargin, topMargin,-rightMargin,-bottomMargin);

    maxSpin->move(0,10 - maxSpin->height()/2);
    minSpin->move(0,this->height() - 10 - minSpin->height()/2);


    bottomP = plotCadre.bottom();
    topP = plotCadre.top();
    int leftP = plotCadre.left();
    int rightP = plotCadre.right() ;
    int nP = rightP - leftP ;
    if (nP >=maxIndex){nP=maxIndex;}

    painter->begin(this);
    painter->drawRect(plotCadre);

    // V GRID

    int start = (min/bestStep)*bestStep ;

    int Y0 = 0 ;
    while(start < max) {
        Y0 = mapTocadre( start);
        QLine L = QLine(leftP -5,Y0 , rightP + 5 , Y0);


        painter->setPen(Qt::black);
        painter->setFont(QFont("Arial", 8));
        painter->drawText(0, Y0+3, QString::number(start));

        pen.setWidth(1);
        if(start == 0){
            pen.setBrush(Qt::black);
        }else{
            pen.setBrush(Qt::darkGray);

        }
        painter->setPen(pen);

        painter->drawLine(L);
        start += bestStep ;
    };





    if((min < -5 && max > 5 )|| true){

        int Y0 = mapTocadre(0);
        QLine L = QLine(leftP -5,Y0 , rightP + 5 , Y0);
        pen.setWidth(1);
        pen.setBrush(Qt::black);
        painter->setPen(pen);

        painter->drawLine(L);
    }


    bool ok = false ;
    for (int i = 0 ; i < traces.size() ; i++){
        int N = traces[i]->chanNum;
        float G = SET[N]->cSetVals.gain ;
        float OFS = SET[N]->cSetVals.offset ;
            for (int j = 1 ; j <= nP-2 ; j++){

                int iD = (curIndex - j >= 0 ? curIndex-j : curIndex-j+maxIndex);
               int iL = (iD -1 >= 0 ? iD-1 : iD+maxIndex -1 );

                float Vd = traces[i]->vals[iD].toFloat(&ok);
                float Vl = traces[i]->vals[iL].toFloat(&ok);

                    Vd = Vd * G + OFS ;
                    Vl = Vl * G + OFS ;

                    int Yl = mapTocadre(Vl);
                    int Yd = mapTocadre(Vd);

                QLine L = QLine(rightP - j,Yd ,
                                rightP - j -1 , Yl);

                pen.setWidth(2);
                pen.setBrush(traces[i]->color);
                painter->setPen(pen);
                painter->drawLine(L);
            }

    }
    painter->end();
}

int plotterWidget::mapTocadre(float Val){
    int Y = (int) ((topP-bottomP)*((Val-min)/(range)) + bottomP) ;
    return Y ;
}

void plotterWidget::updateGrid(){
    range = max - min ;
    if (range > 10000){
        leftMargin = 35 ;
    }else if (range >1000){
        leftMargin = 30 ;
    }else{
        leftMargin =25 ;
    }
    maxSpin->setRange(min + 10,30000);
    minSpin->setRange(-30000,max - 10);
    bestStep =findBestStep(range);
    update();
}

int plotterWidget::findBestStep(int R){
    int size =  sizeof stepMap / sizeof stepMap[0] ;
     int step = R/10 ;
     int D = 0 ;
     int oldD = stepMap[size - 1] ;
    for (int i = 0 ; i < size  ; i++){

        D = abs(stepMap[i] - step)  ;
        if(D > oldD ){
            return stepMap[i-1];
        }
        oldD = D ;
    }
    return stepMap[size - 1] ;
}

int plotterWidget::allocMemory(int channelCode, QColor * colorList){

    int I = sizeof (int);
    int C = channelCode ;

    for (int i = 0 ; i < traces.size() ; i ++){
        traces[i]->~trace();
    }

    traces.clear();
    int Cn = 0 ;
    for (int i = 0 ; i < 4*I ; i++){
        if (C & 1){
            traces.append(new trace(i));
            traces[traces.size() -1]->color = colorList[Cn];
        }
        Cn++;  Cn = (Cn < COLORNUMBER ? Cn : 0);
        C = C>>1;
    }
    //qDebug() << "Create mem for : " + QString::number( traces.size());
    return 1;
}

void plotterWidget::memorizedatas(QList<QByteArray> & list){

    for(int i = 0 ; i < traces.size() ; i++){
        traces[i]->vals[curIndex] = list[traces[i]->chanNum]   ;

    }

}

void plotterWidget::updateGraph(QList<QByteArray> &list ){

    memorizedatas(list);
    validChannel.clear();
    for(int i = 0 ; i <traces.size(); i++){
        bool ok = false;
        int val =  traces[i]->vals[curIndex].toFloat(&ok);
        if (!ok){
            qDebug() << "Conversion failed";
        }else{
            validChannel.append(val);
        }
    }
        curIndex++;
        curIndex = (curIndex < maxIndex ? curIndex : 0    );
    update();

}



//==================================================================//
//                CLASS SELECTORWINDOW                              //
//==================================================================//
selectorWindow::selectorWindow(QWidget * parent):
    QWidget(parent),
    Hlayout(new QHBoxLayout)
{
    this->setLayout(Hlayout);
    Hlayout->setSizeConstraint(QLayout::SetMinimumSize);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

}

void selectorWindow::channelSelector(int N, QColor * colorList)
{

    for(int i = 0 ; i < panelList.size()  ; i++){
        Hlayout->removeWidget(panelList[i]);
        //panelList[i]->~panelSet();
        delete(panelList[i]);
    }

    panelList.clear();
    for(int i = 0 ; i < N ; i++){
        panelList.append( new panelSet(i, colorList, this));


        Hlayout->addWidget(panelList.last());

        connect( panelList[i]->selectBox ,SIGNAL(stateChanged(int))  ,
                 this  ,                  SLOT(computeChannelSelect(int))  );

        connect( panelList[i] ,SIGNAL(cValsChanged(int))  ,
                 this  ,                  SLOT(setGain(int))  );

    }
}

void selectorWindow::setGain(int)
{
    emit newSettings(panelList);
}

void selectorWindow::repos(){
    for(int i = 0 ; i < panelList.size()  ; i++){
        panelList[i]->repos(true);
    }

}

void selectorWindow::refillChekbox(){
    int C = 1;
    for (int i = 0 ; i < panelList.size() ; i++){
        if(channelCode && C){
            panelList[i]->selectBox->setChecked(true);
        }
        C = C << 1 ;
    }
}


int selectorWindow::computeChannelSelect(int){
    channelCode  = 0  ;
    int C = 1 ;

    for (int i = 0 ; i < panelList.size() ; i++){
        if(panelList[i]->selectBox->isChecked()){
            channelCode |= C;
        }
        C = C << 1 ;
    }
    emit channelChanged(channelCode);
    return channelCode ;
}

void selectorWindow::showEvent(QShowEvent * ){
    refillChekbox();
  emit newSettings(panelList);
}



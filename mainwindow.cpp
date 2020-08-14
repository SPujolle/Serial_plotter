#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QCheckBox>
#include <QList>
#include "settingsdialog.h"


// Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settWindow(new settingsDialog(this)),
    status(new QLabel() ),
    serial(new QSerialPort(this)),
   // console(new Console(this)),
    console(new fullConsole(this)),

    sepDialog(new separatorDialog(this)),
    plot(new scopeWindow(this)),
    centralStack(new QStackedWidget(this)),
    centralSplit(new QSplitter(this)),
    RB(new rollingBuffer(100))
{
    ui->setupUi(this);

    this->setWindowTitle("Serial Ploter V 0.1 BETA");   this->resize(600,750);
    ui->statusbar->addPermanentWidget(status);


     setCentralWidget(centralSplit);
     centralSplit->addWidget(console);
     centralSplit->addWidget(plot);

     computeStatusMessage();
// ======================= MENU CREATION ====================//
    QMenu *file = new QMenu(this);
        QAction *quit = new QAction("&Quit", this);
        record = new QAction("&Record", this);
            record->setStatusTip(tr("Record datas in selected file"));
            record->setEnabled(outFileready);
        stop = new QAction("&Stop record", this);
            stop->setEnabled(recordOnOff);
        closeFile = new QAction("Close output file",this);
            closeFile->setEnabled(false);
        QAction *selectOut = new QAction("&Select output file", this);
        selectOut->setStatusTip(tr("Select file for recording"));

    file = menuBar()->addMenu(tr("&File"));
    {
        file->addAction(record);
        file->addAction(stop);
        file->addAction(selectOut);
        file->addAction(closeFile);
        file->addSeparator();
        file->addAction(quit);
     }
    QMenu *tools = new QMenu(this);
        QAction *ports = new QAction("&Port", this);
        QAction *separator = new QAction("&Separators", this);
        QAction *Connect = new QAction("&Connect", this);
        QAction *disConnect = new QAction("&Disconnect", this);

    tools = menuBar()->addMenu(tr("&Settings"));
    tools->addAction(ports);
    tools->addAction(separator);
    tools->addAction(Connect);
    tools->addAction(disConnect);

    QMenu *wiew = new QMenu(this);

    clearConsole = new QAction("&Clear console", this);

    wiew = menuBar()->addMenu(tr("&Wiew"));
    wiew->addAction(clearConsole);


    QMenu *about = new QMenu(this);
        about = menuBar()->addMenu(tr("?"));
        QAction * aboutsV = new QAction("About SerialView", this);
        about->addAction(aboutsV);

// =====================CONNECTION========================//

    connect(selectOut, &QAction::triggered, this, &MainWindow::selectOutFile);
    connect(record, &QAction::triggered, this, &MainWindow::switchRecordOnOff);
    connect(stop, &QAction::triggered, this, &MainWindow::switchRecordOnOff);
    connect(closeFile, &QAction::triggered, this, &MainWindow::CloseFile);
    connect(quit, &QAction::triggered, qApp, QApplication::quit);

    connect(ports, &QAction::triggered,settWindow ,&settingsDialog::show );
    connect(Connect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(disConnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(clearConsole, &QAction::triggered, console, &fullConsole::clear);
    connect(separator, &QAction::triggered, this, &MainWindow::chooseSeparator);

   connect(aboutsV, &QAction::triggered, this, &MainWindow::about);

//    connect(graphWiew  ,&QAction::triggered  ,this  ,&MainWindow::switchConsoleGraph );
//    connect(consoleWiew  ,&QAction::triggered  ,this  ,&MainWindow::switchConsoleGraph );

    connect(settWindow, SIGNAL(settingsApplied(bool) ),
            this      , SLOT  (acceptSettings (bool)  ));
    connect(settWindow, SIGNAL(disconnectSignal(bool) ),
            this      , SLOT  (closeSerialPort (bool)  ));

    //connect(  ,  ,  ,  );

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    connect(console, &fullConsole::getData, this, &MainWindow::writeData);

    connect( this , SIGNAL(newDatas(QList<QByteArray>&)) ,
             plot->plotWindow , SLOT(updateGraph(QList<QByteArray>&)) );

    connect( plot , SIGNAL(channelSelected(int)),
                sepDialog        , SLOT(channelSelected(int))                   );

    connect( this , SIGNAL(newTrame(QByteArray &)) ,
             this , SLOT(putConsole(QByteArray &)) );

    connect( this    ,SIGNAL(fieldsNumber(int))  , plot , SLOT(channelSelector(int))  );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About SerialView"),
                       tr("The <b>SerialView V01 BETA</b> is an crude version "
                          "for a multi tool for serial port, ploter, logger and console<br> "
                          "writen with Qt, GPL intended for multiplatform.<br>"
                          "If you find bug, please mail to <b>serialview@free.fr</b><br>"
                          "To acces sources : <b>https://github.com/SPujolle/Serial_plotter</b><br>"
                          "Help welcome to compile a linux and mac version "));
}




void MainWindow::acceptSettings(bool){
    QString title;
    title = "TEST  V1 -- " ;

    this->setWindowTitle(settWindow->settings().name + " - "+ settWindow->settings().stringBaudRate);
    openSerialPort();
}
void MainWindow::openSerialPort()
{
    const settingsDialog::Settings p =  settWindow->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
//        m_console->setEnabled(true);
//        m_ui->actionConnect->setEnabled(false);
//        m_ui->actionDisconnect->setEnabled(true);
//        m_ui->actionConfigure->setEnabled(false);

    computeStatusMessage();


    } else {
        QMessageBox::critical(this, tr(" ELSE Error"), serial->errorString());
        showStatusMessage(tr("ERROR connecting to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl),
                          true
                          );
    }
}

void MainWindow::closeSerialPort(bool)
{
    if (serial->isOpen())
        serial->close();
//    m_console->setEnabled(false);
//    m_ui->actionConnect->setEnabled(true);
//    m_ui->actionDisconnect->setEnabled(false);
//    m_ui->actionConfigure->setEnabled(true);
    computeStatusMessage();
}

void MainWindow::putConsole(QByteArray & D){
    if (!sepDialog->test){
         //console->putData(data);
    }else{
        console->putData(D);

    }
}

void MainWindow::readData()
{
    data = serial->readAll();
    if(recordOnOff)  saveFile(true);

    if (!sepDialog->test){
         console->putData(data);
    }

    accTrame();
     //splitDatas(data);



}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::accTrame(){
    char CR = static_cast<char>(13);
    char LF = static_cast<char>(10);
    static int state = 0 ;
    static int startTrame  = 0 ;
    static int stopTrame = 0 ;
    bool trameReady  = 0 ;
        //qDebug()  << "SIZE = " << data.size();
        for(int i = 0 ; i < data.size(); i++ ){
            char A = data.at(i) ;
            bool eolTrue = (A==CR || A == LF) ;

            char * B = RB->curValue() ;
            *B = A;


            if( (state == 0) && eolTrue ){
                state = 1 ;
            }
            if( state == 3 ){
                state = 1 ;
            }
            if(state == 1 && !eolTrue ){
                state = 2;    startTrame = RB->currentIndex ;

            }
            if (state == 2 && eolTrue){
                state = 3; stopTrame = RB->currentIndex ; trameReady = true ;
            }
              RB->incIndex() ;
        }
        if(trameReady ){
            trame.clear();
            int oIndex = RB->currentIndex ;
            RB->setIndex(startTrame);
            while( RB->currentIndex  != stopTrame ){
                trame.append( *RB->curValue() ) ;
                RB->incIndex();

            }
            //trame.append('>');
            trame.append(CR);
            //qDebug() <<"Start =  " << startTrame << "  Stop =  "   << stopTrame ;
            RB->setIndex(oIndex);
            splitDatas(trame);
            emit newTrame(trame);
            emit newDatas(listDatas);
        }



}
int MainWindow::splitDatas(QByteArray & dat){
    static int oN = 1 ;
    char SEP = 0  ;
    int A = sepDialog->testCode ;

    // merge all separators to one same char
    for (int i = 0 ; i < 5 ; i++){
        if( A & 1 ){
            SEP = SEPARATORS::sepTable[i];
            dat.replace(SEP, sepDialog->testSeparator);
        }
        A = A >> 1 ;
    }
    // Explode datas in field by separtors
    try {
        listDatas = dat.split(sepDialog->testSeparator);
        dat.clear();
        for(int i = 0 ; i < listDatas.size() - 1 ; i++){
            dat.append(listDatas[i]);
            dat.append(" | ");
        }
        dat.append(listDatas.last());

        if (oN != listDatas.size()){
            emit fieldsNumber(listDatas.size());
        }
        oN = listDatas.size() ;
    } catch (...) {

    }
    return 1 ;
}


void MainWindow::showStatusMessage(const QString &part1 , bool ){
        statPart1 = part1;
        status->setText(statPart1 + " | "+ statPart2);
}
void MainWindow::computeStatusMessage(){
    if (serial->isOpen()){
        statPart1 = "Connected to ";
    }else{
        statPart1 = "Disconnected from ";
    }
    if (serial->portName() == ""){
        statPart1 = "No port selected";
    }else{
        statPart1 += serial->portName() +", "+ settWindow->settings().stringBaudRate   ;
    }
    //=========================================================//
    if (curFile == ""){
        statPart2 = "No output file selected";
    }else{
        statPart2 = curFile ;
    }
    statPart2 += " - " + writteModeString[writteMode] ;

        status->setText(statPart1 + " | "+ statPart2);
}

bool MainWindow::selectOutFile()
{
    int A=0;
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setOption(QFileDialog::DontConfirmOverwrite);
    if (dialog.exec() != QDialog::Accepted) return false;

    curFile = dialog.selectedFiles().first();

    file.setFileName(curFile);
    if(QFile::exists(curFile)){
        fileConfirmDialog *box = new fileConfirmDialog(this, curFile);
        box->setWindowModality(Qt::WindowModal);
        A = box->exec();
        if (A != QDialog::Accepted && A != box->overwritte && A!= box->append)
        {
            curFile = ""; writteMode = 0;
            return false;
        }
    }
    QIODevice::OpenMode M ;
    if (A == 1 || A == 0)
         M = QFile::WriteOnly | QFile::Text ;
    if (A == 2)
         M = QFile::WriteOnly | QFile::Text | QFile::Append ;
    if (!file.open(M)) {
        QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(curFile),
                                      file.errorString()));
        curFile = ""; writteMode = 0;
        return false;
    }
    writteMode = (A!=0 ? A : 1);
    outFileready = true ;
    record->setEnabled(outFileready);
    closeFile->setEnabled(true);
    computeStatusMessage();
    return true;
}

void MainWindow::chooseSeparator(bool){
    //dock->setVisible(true);
    sepDialog->show();
}

bool MainWindow::CloseFile(bool){
    outFileready = false ;

    if(recordOnOff){
        switchRecordOnOff(true);
    }
    file.close();
    closeFile->setEnabled(false);
      curFile = ""; writteMode = 0;
    computeStatusMessage();

    return true;
}
bool MainWindow::saveFile(bool)
{
    if (!file.isOpen()) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file (NOT OPEN) %1 :\n%2.")
                             .arg(QDir::toNativeSeparators(curFile),
                                  file.errorString()));
        return false;
    }
    try {
     QTextStream out(&file);

#ifndef QT_NO_CURSOR
     QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
     out << data ;
#ifndef QT_NO_CURSOR
     QApplication::restoreOverrideCursor();
#endif
    } catch (...) {
        QMessageBox::warning(this, tr("Application"),
                             tr("UNKNOW ERROR %1 :\n%2.")
                             .arg(QDir::toNativeSeparators(curFile),
                                  file.errorString()));
        return false;
}
    statusBar()->showMessage(tr("File saved  :  " )+ curFile, 2000);
    return true;
}

bool MainWindow:: switchRecordOnOff(bool){
    recordOnOff = !recordOnOff ;

    record->setEnabled(!recordOnOff && outFileready);
    stop->setEnabled(recordOnOff);
    if (recordOnOff){
        this->setWindowTitle("RECORDING");
    } else {
        this->setWindowTitle("TEST  V1");
    }
    return true;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
rollingBuffer::rollingBuffer(int N){

    data = (char*) malloc(N*sizeof(char));
    size = N ;
    indMax = size -1 ;

}


rollingBuffer::~rollingBuffer(){
    free (data) ;
}

int rollingBuffer::incIndex(){
        if (currentIndex == indMax){
            currentIndex = 0 ;
            return currentIndex ;

        }
        currentIndex ++ ;
        return currentIndex ;
}

int rollingBuffer::incIndex(int S){
    currentIndex += S ;
        if (currentIndex > indMax){
            currentIndex = currentIndex - size ;
            return currentIndex ;

        }
        if (currentIndex < 0){
            currentIndex = currentIndex + size ;
            return currentIndex ;

        }


        return currentIndex ;
}



int rollingBuffer::decIndex(){
            if (currentIndex == 0 ){
                currentIndex =  indMax ;
                return currentIndex ;
            }
            currentIndex -- ;
            return currentIndex ;
}

char * rollingBuffer::curValue(){

    return &data[currentIndex];

}

int rollingBuffer::setIndex(int I){

    currentIndex = I ;
    return I ;
}



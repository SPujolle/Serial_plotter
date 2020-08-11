#include "console.h"

Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
{

    document()->setMaximumBlockCount(100);
    this->setMinimumSize(QSize(200,100));
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::yellow);
    p.setColor(QPalette::Text, Qt::black);
    setPalette(p);



}
void Console::putData(const QByteArray &data)
{
    insertPlainText(data);

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}
// ============================================== //


fullConsole::fullConsole(QWidget * parent):
    QWidget(parent),
    visuConsole(new Console(this)),
    sendDatasLine(new QLineEdit(this)),
    sendButton(new QPushButton(this)),
    newlineChooser(new QComboBox(this)),
    deleteAfter(new QCheckBox)
{
    QVBoxLayout * Layout = new QVBoxLayout;
    QHBoxLayout * l1Layout = new QHBoxLayout ;
    QHBoxLayout * l2Layout = new QHBoxLayout ;

    this->setLayout(Layout);
    Layout->addWidget(visuConsole);
    Layout->addLayout(l1Layout);
    Layout->addLayout(l2Layout);

    l1Layout->addWidget(sendDatasLine);
    l1Layout->addWidget(sendButton);
    l2Layout->addWidget(newlineChooser);
    l2Layout->addWidget(deleteAfter);

    sendButton->setText(tr("Send"));
    QString LF ="\n" ;
    QString CR ="\r" ;
    QString BOTH ="\n\r";
    newlineChooser->addItem("Newline (NL)",  LF);
    newlineChooser->addItem("No line return", static_cast<QString>("") );
    newlineChooser->addItem("Carriage return (CR)", CR);
    newlineChooser->addItem("Both CR and NL", BOTH);

    deleteAfter->setText(tr("Clear after send"));

    connect( sendButton ,SIGNAL(clicked(bool))  ,
             this ,  SLOT(getDataFromLine()));

    connect( sendDatasLine ,SIGNAL(returnPressed())  ,
             this ,  SLOT(getDataFromLine()));

}

void fullConsole::getDataFromLine(){
    QString text = sendDatasLine->text();
    text.append(  newlineChooser->currentData().toString()) ;
    QByteArray Datas = text.toUtf8();
    if(deleteAfter->isChecked()){
        sendDatasLine->setText("");
    }
    emit getData(Datas);

}

//void fullConsole::keyPressEvent(QKeyEvent *e)
//{
//    switch (e->key()) {
//    case Qt::Key_Backspace:
//    case Qt::Key_Left:
//    case Qt::Key_Right:
//    case Qt::Key_Up:
//    case Qt::Key_Down:
//        break;
//    default:
//        if (m_localEchoEnabled)
//            QPlainTextEdit::keyPressEvent(e);
//        emit getData(e->text().toLocal8Bit());
//    }
//}









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

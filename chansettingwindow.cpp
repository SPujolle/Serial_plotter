#include "chansettingwindow.h"

chanSettingWindow::chanSettingWindow(QWidget * parent  , QString name, QColor color) :
    QFrame(parent),
    gain(new QDoubleSpinBox),
    offset(new QDoubleSpinBox)
{
    Qt::WindowFlags flags = 0;
    flags = Qt::Tool;
    setWindowFlags(flags);

    QString CC = "border: 2px solid " + color.name()  + ";" ;
    this -> setFrameStyle(QFrame::Plain | QFrame::Box);
    this->setObjectName("truc");
    this->setStyleSheet("#truc { " +  CC  + " }");

    this->setWindowModality(Qt::NonModal);
    this->setWindowTitle(name);
    this->setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed);
    this->setWindowFlags(windowFlags()  | Qt::Tool);
    QGridLayout * layout = new QGridLayout ;

    this->setLayout(layout);
    QLabel * gainText = new QLabel(tr("Gain"));
    layout->addWidget(gain,0,0); layout->addWidget(gainText,0,1);
    QLabel * offsetText = new QLabel(tr("Offset"));
    layout->addWidget(offset,1,0); layout->addWidget(offsetText,1,1);

    gain->  setRange(-10000, 10000);
    offset->  setRange(-10000, 10000);;

}

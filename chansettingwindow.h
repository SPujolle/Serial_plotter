#ifndef CHANSETTINGWINDOW_H
#define CHANSETTINGWINDOW_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>

class chanSettingWindow : public QFrame
{
    Q_OBJECT
public:
    chanSettingWindow(QWidget * parent =nullptr , QString name="", QColor color = Qt::red);
    QDoubleSpinBox * gain = nullptr;
    QDoubleSpinBox * offset = nullptr ;

private :



};

#endif // CHANSETTINGWINDOW_H

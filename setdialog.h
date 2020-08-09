#ifndef SETDIALOG_H
#define SETDIALOG_H
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QBoxLayout>

QT_BEGIN_NAMESPACE

namespace Ui {
class SetDialog;
}

class QIntValidator;

QT_END_NAMESPACE



class SetDialog : public QDialog
{
    Q_OBJECT
public:
    SetDialog(QWidget *parent = 0);

     QLabel *label;

private :
       Ui::SetDialog *s_ui = nullptr;
       ~SetDialog();

public slots :
};

#endif // SETDIALOG_H

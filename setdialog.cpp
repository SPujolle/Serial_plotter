#include "setdialog.h"


SetDialog::SetDialog(QWidget * parent) :
    QDialog(parent)

{
    label = new QLabel(tr("Find &what:"));

     QHBoxLayout *topLeftLayout = new QHBoxLayout;

    this->setLayout(topLeftLayout);
     topLeftLayout->addWidget(label);


}SetDialog::~SetDialog()
{
}

#include "fileconfirmdialog.h"

fileConfirmDialog::fileConfirmDialog(QWidget * parent, QString file):
    QDialog(parent)
{
    QLabel *message = new QLabel(file + tr(" \n already exist"));

    QGridLayout *Layout = new QGridLayout;
    this->setLayout(Layout);
    Layout->addWidget(message,0,0,1,0, Qt::AlignJustify);


    QPushButton *Cancel = new QPushButton(tr("Cancel"));
    QPushButton *Append = new QPushButton(tr("Append"));
    QPushButton *Overwritte = new QPushButton(tr("Over writte"));
    Layout->addWidget(Cancel,1,0);
    Layout->addWidget(Append,1,1);
    Layout->addWidget(Overwritte,1,2);

 connect(Cancel, SIGNAL(clicked(bool)), this , SLOT (CancelSlot(bool)) );
 connect(Append, SIGNAL(clicked(bool)), this , SLOT (AppendSlot(bool)) );
 connect(Overwritte, SIGNAL(clicked(bool)), this , SLOT (OverWSlot(bool)) );

}

int fileConfirmDialog::OPen(){
    this->setVisible(true);
    return 1;
}
void fileConfirmDialog::CancelSlot(bool){
    emit done(0);
}

void fileConfirmDialog::AppendSlot(bool){
    emit done(append);

}
void fileConfirmDialog::OverWSlot(bool){
    emit done(overwritte);

}

#include "separatordialog.h"

separatorDialog::separatorDialog(QWidget * parent) :
    QDialog(parent)
    // customCharBox(new QLineEdit(this))
{
    this->setWindowModality(Qt::NonModal);
    this->setWindowTitle(tr("Select separator character"));
    this->setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed);
    Layout = new QGridLayout;
    H1box = new QHBoxLayout ;
    this->setLayout(Layout);
    int i ;
    for (i = 0 ; i < 5 ; i++){
        buttons[i].setText(SEPARATORS::buttonName[i]);
        Layout->addWidget(&buttons[i],0,i);
        connect( &buttons[i] ,SIGNAL(stateChanged(int))  ,this  , SLOT(computeSepCode(int))  );

    }
//    H1box->addWidget(customCharBox);
//    customCharBox->setFixedWidth(100);
//    QLabel * customChar = new QLabel(tr("Custom char"));
//    H1box->addWidget(customChar);
//    Layout->addLayout(H1box,1,0,1,0,Qt::AlignRight);


    QPushButton *close = new QPushButton(tr("Close"));
    Test = new QPushButton(tr("Fields Visu"));


    Layout->addWidget(Test,2,0);
    //Layout->setColumnStretch(1,1);
    Layout->addWidget(close,2,i);


    connect(close   , SIGNAL(clicked(bool))      ,  this        , SLOT(close())          );
    connect(  Test  , SIGNAL(released())         ,  this        , SLOT(testOnOff())          );
}

void separatorDialog::disableCheckBox(){
    int size = sizeof (SEPARATORS:: sepTable) / sizeof(SEPARATORS::sepTable[0]);
        if(channelCode != 0){
            for (int i = 0 ; i < size ; i++){
                buttons[i].setDisabled(true);
            }
    }else{
            for (int i = 0 ; i < size ; i++){
                buttons[i].setEnabled(true);
            }
    }
}

int separatorDialog::computeSepCode(int){
    testCode  = 0  ;
    testSeparator = static_cast<char>(0);
    for (int i = 4 ; i >= 0 ; i--){
        if(buttons[i].isChecked()){
            testCode |= SEPARATORS::codes[i];
            testSeparator = SEPARATORS::sepTable[i];
        }
    }
    return 1;
}

void separatorDialog::testOnOff(){
    test = !test ;
    computeSepCode(1);

    if (test) Test->setText("Raw datas");
    if (!test) Test->setText("Show fields");

}








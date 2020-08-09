#ifndef FILECONFIRMDIALOG_H
#define FILECONFIRMDIALOG_H
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QPushButton>

class fileConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    fileConfirmDialog(QWidget * parent = nullptr, QString file ="");
    int OPen();
    const int overwritte = 1;
    const int append = 2 ;

private slots :
    void CancelSlot(bool);
    void AppendSlot(bool);
    void OverWSlot(bool);

signals :
    void writteMode(int);

};

#endif // FILECONFIRMDIALOG_H

#ifndef CONSOLE_H
#define CONSOLE_H
#include <QPlainTextEdit>
#include <QScrollBar>


class Console : public QPlainTextEdit
{
    Q_OBJECT




signals:
    void getData(const QByteArray &data);

public:
    explicit Console(QWidget *parent = nullptr);

    void putData(const QByteArray &data);
    void setLocalEchoEnabled(bool set);

private:
    bool m_localEchoEnabled = false;


};

#endif // CONSOLE_H

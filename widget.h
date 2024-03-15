#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleValidator>
#include "server.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QLineEdit *portLineEdit;
    QPushButton *startButton;
    QTextEdit *logTextEdit;
    QLineEdit *messageLineEdit;
    QPushButton *sendButton;

    QHBoxLayout *topLayout;
    QHBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout;

private slots:
    void ClickedSlot();
    void ToggleSlot(bool checked);
    void returnButton();

signals:
    void startS(int);
    void stopS();
    void sendToAll(QString);

public slots:
    void textToTE(QString str);
};

#endif // WIDGET_H

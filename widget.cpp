#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    portLineEdit = new QLineEdit();
    startButton = new QPushButton("Start");
    logTextEdit = new QTextEdit;
    messageLineEdit = new QLineEdit();
    sendButton = new QPushButton("Send");

    portLineEdit->setPlaceholderText("port");
    portLineEdit->setValidator(new QDoubleValidator(portLineEdit));
    messageLineEdit->setPlaceholderText("Enter a message:");
    startButton->setCheckable(true);
    logTextEdit->setReadOnly(true);

    topLayout = new QHBoxLayout();
    topLayout->addWidget(portLineEdit);
    topLayout->addWidget(startButton);

    bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(messageLineEdit);
    bottomLayout->addWidget(sendButton);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(logTextEdit);
    mainLayout->addLayout(bottomLayout);

    Server *s = new Server;
    s->close();

    //connect

    connect(sendButton, &QPushButton::clicked, this, &Widget::ClickedSlot);
    connect(startButton, &QPushButton::toggled, this, &Widget::ToggleSlot);
    connect(this, &Widget::startS, s, &Server::startServer);
    connect(this, &Widget::stopS, s, &Server::stopServer);
    connect(this, &Widget::sendToAll, s, &Server::SendAll);
    connect(s, &Server::fromServerToTE, this, &Widget::textToTE);
    connect(s, &Server::serverIsntStarted, this, &Widget::returnButton);
}

Widget::~Widget()
{

}

void Widget::ClickedSlot()
{
    emit sendToAll(messageLineEdit->text());
    messageLineEdit->clear();
}

void Widget::ToggleSlot(bool checked)
{
    if(checked)
    {
        emit startS(portLineEdit->text().toInt());
    }
    else
    {
        emit stopS();
    }
}

void Widget::returnButton()
{
    startButton->toggle();
}

void Widget::textToTE(QString str)
{
    logTextEdit->append(str);
}

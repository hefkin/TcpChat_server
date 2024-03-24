#include "widget.h"
#include "server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("Chat Server");
    w.resize(600, 300);
    w.show();

    return a.exec();
}

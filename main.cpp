#include <QtGui/QApplication>
#include <QPalette>
#include <QColor>

#include "hello.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Hello main;

    QColor cl;
    QPalette pal;
    cl.setRgb(90,132,201);
    pal.setColor(QPalette::Background,cl);
    main.setPalette(pal);

    main.showFullScreen();
    return a.exec();
}

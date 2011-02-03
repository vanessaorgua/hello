#include "qgpio.h"
#include <QFile>
#include <QString>

QGpio::QGpio(QObject *p): QObject(p)
{
    for(int i=0;i<7;++i) // ініціалізація порта
    {
        QFile ex("/sys/class/gpio/export");
        ex.open(QIODevice::WriteOnly);
        ex.write(QString("%1").arg(160+i).toLatin1());
        ex.close();

        QFile d(QString("/sys/class/gpio/gpio%1/direction").arg(160+i).toLatin1());
        d.open(QIODevice::WriteOnly);
        d.write("out");
        d.close();

        QFile v(QString("/sys/class/gpio/gpio%1/value").arg(160+i).toLatin1());
        v.open(QIODevice::WriteOnly);
        v.write("0");
        v.close();
    }

}

QGpio::~QGpio()
{
    for(int i=0;i<7;++i)
    {
        QFile ex("/sys/class/gpio/unexport");
        ex.open(QIODevice::WriteOnly);
        ex.write(QString("%1").arg(160+i).toLatin1());
        ex.close();
    }
}


void QGpio::setValue(int n ,bool v)
{
    QFile val(QString("/sys/class/gpio/gpio%1/value").arg(160+n).toLatin1());
    val.open(QIODevice::WriteOnly);
    val.write(QString("%1").arg(v?1:0).toLatin1());
    val.close();
}

/*
void QGpio::run()
{

}
*/

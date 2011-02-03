#include "termupdatethread.h"
#include <QLineEdit>
#include <QString>
#include <QFile>
#include <QTimer>
#include <QDebug>

TermUpdateThread::TermUpdateThread(QStringList sensorName) :
        nameS(sensorName)
{

}

void TermUpdateThread::run()
{
    tmr =new QTimer;
    //tmr->setSingleShot(true);
    tmr->setInterval(3000);
    tmr->start();
    connect(tmr,SIGNAL(timeout()),this,SLOT(updateData()));
    qDebug() << "Create event loop";
    exec();    // це запустить цикл обробника подій
}

TermUpdateThread::~TermUpdateThread()
{
    delete tmr;
}

void TermUpdateThread::updateData()
{
    QStringList data;

    data.clear();
    foreach(QString s,nameS)
    {
        QFile f(QString("%1/w1_slave").arg(s)); // /sys/devices/w1 bus master/28-000001ba40af
        f.open(QIODevice::ReadOnly);
        QByteArray b=f.readAll();
        f.close();
        QString s=b.trimmed().split('=').at(2);
        data << s.insert(2,",").left(4);
    }
    emit updateData(data);
    //tmr->start();


}

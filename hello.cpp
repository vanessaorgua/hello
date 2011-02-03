#include "hello.h"
#include "ui_hello.h"
#include "dialog.h"
#include <QTimer>
#include <QFile>
#include <QDateTime>
#include <QByteArray>
#include "termupdatethread.h"
#include <QDebug>
#include <QVector>
#include <QStringList>

#include "myspidev.h"

Hello::Hello(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hello)
{
    ui->setupUi(this);
    connect(ui->bnLink,SIGNAL(clicked()),this,SLOT(checkLink()));

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateData()));

    QStringList name;
    name //<< "/sys/devices/w1 bus master/28-000001ba40af"
            << "/sys/devices/w1 bus master/28-000001ba1fe0";

    qDebug() << "Create thread";

    TermUpdateThread *t1=new TermUpdateThread(name);
    t1->start(QThread::LowPriority);
    connect(t1,SIGNAL(updateData(QStringList&)),this,SLOT(updateTemp(QStringList&)));

    io=new QGpio(this);
    connect(ui->cbOut_1,SIGNAL(toggled(bool)),this,SLOT(updateValue(bool)));
    connect(ui->cbOut_2,SIGNAL(toggled(bool)),this,SLOT(updateValue(bool)));
    connect(ui->cbOut_3,SIGNAL(toggled(bool)),this,SLOT(updateValue(bool)));
    connect(ui->cbOut_4,SIGNAL(toggled(bool)),this,SLOT(updateValue(bool)));
    connect(ui->cbOut_5,SIGNAL(toggled(bool)),this,SLOT(updateValue(bool)));
    connect(ui->cbOut_6,SIGNAL(toggled(bool)),this,SLOT(updateValue(bool)));
    connect(ui->cbOut_7,SIGNAL(toggled(bool)),this,SLOT(updateValue(bool)));

    MySpiDev *spi = new MySpiDev(500);
    spi->start(QThread::LowPriority);
    connect(spi,SIGNAL(valueUpdated(QVector<int>)),this,SLOT(updateAdc(QVector<int>)));
}

Hello::~Hello()
{
    delete ui;
}

void Hello::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Hello::checkLink()
{
    Dialog p(this);
    p.exec();

}

void Hello::updateData()
{
     ui->lineDate->setText(QDateTime::currentDateTime().toString());
}

void Hello::updateTemp(QStringList& d)
{
    ui->lineT_1->setText(d[0]);
    //ui->lineT_2->setText(d[1]);
}

void Hello::updateValue(bool v)
{

    int i=sender()->objectName().right(1).toInt()-1;
    io->setValue(i,v);
}

void Hello::updateAdc(QVector<int> values)
{
    int i=0;
    QVector<QLineEdit*> le;
    le << ui->Ai_0
            << ui->Ai_1
            << ui->Ai_2
            << ui->Ai_3
            << ui->Ai_4
            << ui->Ai_5
            << ui->Ai_6
            << ui->Ai_7;
    foreach(int val,values)
    {
        le[i++]->setText(QString("%1").arg(val));
    }
}

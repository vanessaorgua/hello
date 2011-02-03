#ifndef QGPIO_H
#define QGPIO_H

#include <QObject>
#include <QThread>

class QGpio : public QObject //QThread
{
    Q_OBJECT
public:
    QGpio(QObject *p=0);
    ~QGpio();

    //void run();
public slots:
    void setValue(int ,bool);

};

#endif // QGPIO_H

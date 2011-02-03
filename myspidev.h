#ifndef MYSPIDEV_H
#define MYSPIDEV_H

#include <QThread>
#include <QVector>

class QTimer;

class MySpiDev : public QThread
{
    Q_OBJECT
public:
    MySpiDev(int interval);
    ~MySpiDev();

    void run();

signals:
    void valueUpdated(QVector<int>);
private slots:
    void updateData();

private:
    QTimer *tmr;
    int fd;
    void setCS(bool v);
};

#endif // MYSPIDEV_H

#ifndef HELLO_H
#define HELLO_H

#include <QWidget>
#include <QStringList>
#include <QVector>

#include "qgpio.h"

class TrendChart;

namespace Ui {
    class Hello;
}

class Hello : public QWidget {
    Q_OBJECT
public:
    Hello(QWidget *parent = 0);
    ~Hello();
private slots:
    void checkLink();
    void updateData();
    void updateTemp(QStringList&);
    void updateValue(bool);
    void updateAdc(QVector<int>);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
private:
    Ui::Hello *ui;
    TrendChart *trc;
    QGpio *io;

};

#endif // HELLO_H

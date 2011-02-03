#ifndef TERMUPDATETHREAD_H
#define TERMUPDATETHREAD_H

#include <QVector>
#include <QStringList>

class QLineEdit;
class QString;

class QTimer;

#include <QThread>

class TermUpdateThread : public QThread
{
    Q_OBJECT
public:
    TermUpdateThread(QStringList sensorName);
    ~TermUpdateThread();

    void run();
signals:
    void updateData(QStringList&);

private slots:
        void updateData();
private:
    QStringList nameS;
    QTimer *tmr;
};

#endif // TERMUPDATETHREAD_H

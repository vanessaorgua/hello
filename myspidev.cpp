#include "myspidev.h"
#include <QTimer>
#include <QFile>
#include <QDebug>

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

MySpiDev::MySpiDev(int interval) : fd(-1)
{
    QFile ex("/sys/class/gpio/export");
    ex.open(QIODevice::WriteOnly);
    ex.write("194"); // GPG2
    ex.close();

    QFile d("/sys/class/gpio/gpio194/direction");
    d.open(QIODevice::WriteOnly);
    d.write("out");
    d.close();

    QFile v("/sys/class/gpio/gpio194/value");
    v.open(QIODevice::WriteOnly);
    v.write("1");
    v.close();


    tmr=new QTimer;
    tmr->setInterval(interval);
    connect(tmr,SIGNAL(timeout()),this,SLOT(updateData()));
}

MySpiDev::~MySpiDev()
{
    delete tmr;
    if(fd>-1)
        ::close(fd);

}

void MySpiDev::run()
{

    fd=::open("/dev/spidev0.0",O_RDWR);
    if(fd>-1) // якщо порт відкрили
    {
        // настроїти для роботи
        __u8  mode=0, lsb=0, bits=0;
        __u32 speed=500000;
        if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
            qDebug() << "SPI rd_mode";
            return;
         }
         if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb) < 0) {
            qDebug() << "SPI rd_lsb_fist";
            return;
         }
         if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
            qDebug() << "SPI bits_per_word";
            return;
         }

         if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
            qDebug() << "SPI max_speed_hz";
            return;
         }


        tmr->start();
        exec();
    }


}

void MySpiDev::updateData()
{
    short cmd[8]={0x0800,0x1000,0x1800,0x2000,0x2800,0x3000,0x3800,0x0200};
    QVector<int> res;

    struct spi_ioc_transfer xfer[2];
    short in,out=0;
    int         status;

    memset(xfer, 0, sizeof xfer);

    xfer[0].tx_buf = (__u64) (&out);
    xfer[0].len = 2;

    xfer[1].rx_buf = (__u64) (&in);
    xfer[1].len = 2;

    setCS(false);
    status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
    setCS(true);

    if (status < 0) {
       qDebug() << "SPI_IOC_MESSAGE" << in << out;
    }
    for(int i=0;i<8;++i)
    {
        memset(xfer, 0, sizeof xfer);

        xfer[0].tx_buf = (__u64) cmd+i;
        xfer[0].len = 2;

        xfer[1].rx_buf = (__u64) (&in);
        xfer[1].len = 2;

        setCS(false);
        status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
        setCS(true);
        if (status < 0) {
           qDebug() << "SPI_IOC_MESSAGE" << in << cmd[i];
        }
        res << in;

    }
    emit valueUpdated(res);
}

void   MySpiDev::setCS(bool v)
{
    QFile fv("/sys/class/gpio/gpio194/value");
    fv.open(QIODevice::WriteOnly);
    fv.write(v?"1":"0");
    fv.close();
}

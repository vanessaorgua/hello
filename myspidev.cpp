#include "myspidev.h"
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QtEndian>

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
        __u8  mode=1, lsb=0, bits=0;
        __u32 speed=1000000;
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
    // short cmd[8]={0x0800,0x1000,0x1800,0x2000,0x2800,0x3000,0x3800,0x0200};
    //short    cmd[8]={0x0008,0x0010,0x0018,0x0002};
    short    cmd[8]={0x0010,0x0008,0x0000,0x0002};

    QVector<int> res;

    struct spi_ioc_transfer xfer[1];
    short in,out=0x0018;
    int status;

    memset(xfer, 0, sizeof xfer);

    xfer[0].tx_buf = (__u64) (&out);
    xfer[0].rx_buf = (__u64) (&in);
    xfer[0].len = 2;

    status = ioctl(fd, SPI_IOC_MESSAGE(1), xfer);

    if (status < 0) {
       qDebug() << "SPI_IOC_MESSAGE" << in << out;
    }

    for(int i=0;i<4;++i)
    {
        out=cmd[i];
        status = ioctl(fd, SPI_IOC_MESSAGE(1), xfer);
        if (status < 0) {
           qDebug() << "SPI_IOC_MESSAGE" << in << cmd[i];
        }

        res <<  qToBigEndian(in);

    }
    emit valueUpdated(res);
}


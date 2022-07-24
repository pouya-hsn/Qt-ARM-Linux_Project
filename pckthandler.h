#ifndef PCKTHANDLER_H
#define PCKTHANDLER_H

#include "fifo.h"
#include <QtSerialPort/QSerialPort>

#define Header1 0x53
#define Header2 0x74

#define statusID  0x04
#define powerID 0x05
#define tmpHmdID  0x06
#define levelingID 0xA3
#define mobinID 0xA4
#define mobsatID 0xA5

#define stLen 16
#define powLen 14
#define tmpHmdLen 13
#define lvlLen 5
#define mbnLen 5
#define mbsLen 5

class pckthandler: public QObject
{
    Q_OBJECT

public:
    pckthandler(QObject *parent = Q_NULLPTR);
    void serialSend(QByteArray cmd);
    void idDetect(unsigned char ID,unsigned char len,unsigned char *d);
    unsigned char stBytes[stLen-1] = {} ;
    unsigned char powBytes[powLen-1] = {};
    unsigned char tmpHmdBytes[tmpHmdLen-1] = {} ;
    unsigned char lvlBytes[lvlLen-1] = {};
    unsigned char mbnBytes[mbnLen-1] = {};
    unsigned char mbsBytes[mbsLen-1] = {};
public slots:
    void serialReceived();
signals:
    void mbnChanged(unsigned char *byte);
    void mbsChanged(unsigned char *byte);
    void lvlChanged(unsigned char *byte);
    void sttsChanged(unsigned char *byte) ;
    void tmpHmdChanged(unsigned char *byte) ;
    void pwrChanged(unsigned char *byte) ;

private:
    FIFO *recDataBuff;
    unsigned char buff_size = 80 ;
    QSerialPort *serialPort;
    void updateData(unsigned char *p,unsigned char *inf,unsigned char len);
//    QByteArray *sendPckt;
//    void formUpdate();
};

#endif // SERIAL_H

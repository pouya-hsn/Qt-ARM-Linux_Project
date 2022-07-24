#include "pckthandler.h"
#include "fifo.h"
#include "home.h"
#include "cooler.h"
#include "command.h"
#include "levelingservices.h"
#include "services.h"
#include "levelingstatus.h"
#include "mobinstatus.h"
#include "mobinservices.h"
#include "power.h"
#include "mobsatservices.h"
#include "mobsatstatus.h"
#include "status.h"
#include "temp.h"
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include <QtSerialPort/QSerialPort>
#include <QObject>
#include <QDebug>

extern home *homeObj;
extern cooler *coolObj;
extern pckthandler *handler ;
extern command *cmdObj;
extern status *statusObj ;
extern temp *tmpHmdObj;
extern levelingServices *lvlSrvObj;
extern services *srvObj;
extern levelingstatus *lvlStatusObj ;
extern mobinstatus *mobStatusObj;
extern mobinservices *mobSrvObj;
extern power *pwrObj ;
extern mobsatstatus *msatStatusObj;
extern mobsatservices *msatSrvObj;
extern Mobin *mbnObj;
extern Mobsat *mbsObj ;
extern Leveling *lvlObj ;

pckthandler::pckthandler(QObject *parent):QObject (parent)
{
    recDataBuff = new FIFO(buff_size);
    serialPort = new QSerialPort();
    serialPort->setPortName("/dev/ttyPS1"); // 
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->open(QIODevice::ReadWrite);
    connect(serialPort,SIGNAL(readyRead()), this, SLOT(serialReceived()));
}

void pckthandler::serialSend(QByteArray cmd)
{
    QByteArray pckt ;
    pckt.append(Header1) ;
    pckt.append(Header2) ;
    pckt.append((unsigned char)(cmd.size() & 0x00FF)) ;
    unsigned int ckSum = 0;
    for (int i = 0 ;i < cmd.size(); i++)
    {
            pckt.append((unsigned char)(cmd[i])) ;
            ckSum += (unsigned char)(cmd[i]) ;
    }
    pckt.append((unsigned char)((ckSum >> 8) & 0xFF));
    pckt.append((unsigned char) (ckSum & 0x00FF));
    pckt.append(0x85) ;
    serialPort->write(pckt) ;
}

void pckthandler::serialReceived()
{
    QByteArray ba ; // An Array for Store received Serial Data
    ba = serialPort->readAll();
    recDataBuff = new FIFO(80);
    // store received Data in FIFO buffer
    for(unsigned char i=0; i<= ba.length()-1 ; i++)
    {
        recDataBuff->put(ba[i]);
    }

    //reading buffer
    unsigned char temp_read[buff_size] ; //A buffer for reading and saving the current Buffer Data on it
    recDataBuff->read(temp_read);        //reading buffer

    //finding header index in buffer
    unsigned char rdCntr = 0 ;          // Counter for using in while loop
    while (recDataBuff->Cnt >2 && (temp_read[rdCntr] != Header1 || temp_read[rdCntr+1] != Header2))
    {
            recDataBuff->get();
            rdCntr++ ;
    }

    unsigned char pcktLen = 0 ;
    if(recDataBuff->Cnt>2) pcktLen = temp_read[rdCntr+2] ;

    // cheking if whole packet received
    unsigned char pcktID = 0;
    unsigned char CheckSumMSB,CheckSumLSB;
    int CheckSum , sum = 0 ;
    unsigned char pureData[40] ;
    if(recDataBuff->Cnt >= (pcktLen +5))
    {
        recDataBuff->get() ; // Header1
        recDataBuff->get() ; // Header2
        recDataBuff->get(); // Reading Packet Length
        pcktID = recDataBuff->get() ; //reading Packet ID
        sum += pcktID ;

        //receiving pure data and checking with Two CheckSum Bytes
        for(unsigned char i = 0 ; i < pcktLen-1 ; i++){
            pureData[i] = recDataBuff->get();
            sum += pureData[i] ;
        }

        CheckSumMSB = recDataBuff->get() ;
        CheckSumLSB = recDataBuff->get() ;

        recDataBuff->get() ; //getting footer 0x85

        CheckSum = (CheckSumMSB<<8) + (CheckSumLSB) ; // reading Sended CheckSum
        if (sum == CheckSum)
            idDetect(pcktID,pcktLen,pureData);
    }
}

void pckthandler::updateData(unsigned char *p,unsigned char *inf,unsigned char len)
{
    for(int i = 0 ; i < len ; i++)
        inf[i] = p[i];
}

void pckthandler::idDetect(unsigned char ID,unsigned char len,unsigned char *d)
{
    switch (ID) {
        case statusID:
        {
            if(len == stLen)
            {
                updateData(d,stBytes,stLen-1);
                emit sttsChanged(stBytes) ;
            }
            break ;
        }
        case powerID:
        {
            if(len == powLen)
            {
                updateData(d,powBytes,powLen-1);
                emit pwrChanged(powBytes) ;
            }
            break ;
        }
        case tmpHmdID:
        {
            if(len == tmpHmdLen)
            {
                updateData(d,tmpHmdBytes,tmpHmdLen-1);
                emit tmpHmdChanged(tmpHmdBytes) ;
            }
            break ;
        }

        case levelingID :
        {
            if(len == lvlLen){
                updateData(d , lvlBytes , lvlLen-1);
                emit lvlChanged(lvlBytes) ;
            }
            break ;
        }
        case mobinID:
        {
            if(len == mbnLen)
            {
                updateData(d , mbnBytes , mbnLen-1);
                emit mbnChanged(mbnBytes);
            }
            break ;
        }
        case mobsatID:
        {
            if(len == mbsLen)
            {
                updateData(d , mbsBytes , mbsLen-1);
                emit mbsChanged(mbsBytes);
            }
            break ;
        }
        default: break ;
        }
}



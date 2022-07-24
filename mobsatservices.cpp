#include "mobsatservices.h"
#include "ui_mobsatservices.h"
#include "command.h"
#include "home.h"
#include "services.h"
#include "pckthandler.h"
#include "gpio.h"
#include "genfunc.h"
#include <QTimer>
#include <QThread>
#include <QDebug>

#define mobsatHeader 0x4B

extern genFunc *gen;
extern pckthandler *handler ;
extern services *srvObj;
extern command *cmdObj;

extern QTimer *gpioTim;
extern gpio gp1;
extern gpio gp2;
extern gpio gp3;
extern gpio gp4;
extern gpio gp5;
extern gpio gp6;
extern gpio gp7;
extern gpio gp8;
extern gpio gp9;
extern gpio gp10;

extern bool rearMbsMotor;
extern bool frontMbsMotor;
extern bool pedParkMbsClick;
bool pedParkMbsState = false ;

QTimer *pedParkMbsTim;
int num_sec_mbs = 0 ;

mobsatservices::mobsatservices(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mobsatservices)
{
    ui->setupUi(this);
    gen = new genFunc() ;
    gen->setanIcon(":/stopLvlBtn.png",ui->stopLvlBtn);
    setStatusBytes();
    gpioTim = new QTimer();
    connect(handler, &pckthandler::sttsChanged, this, &mobsatservices::setStatusBytes) ;
    gpioTim->start(50) ;
    pedParkMbsTim = new QTimer() ;
    connect(pedParkMbsTim, &QTimer::timeout, this, &mobsatservices::pedParkMbsCheck) ;
    if(pedParkMbsClick) on_pedParkMbsBtn_clicked();
}

mobsatservices::~mobsatservices()
{
    delete ui;
}

void mobsatservices::on_bckMbstBtn_clicked()
{
    srvObj = new services();
    srvObj->show();
    this->close();
    gpioTim->stop();
    pedParkMbsTim->stop();

}

QByteArray mobsatservices::pcktSend(unsigned char data, unsigned char byteNum)
{
    QByteArray pcktData;
    pcktData.append(mobsatHeader);
    for (unsigned char i = 2; i < 8 ; i++) {
        if(i==byteNum){pcktData.append(data);}
        else { pcktData.append(char(0x00)) ; }
    }
    return  pcktData;
}

//============================================================================
void mobsatservices::on_rearBtn_clicked()
{
    if(!frontMbsMotor)
    {
        handler->serialSend(pcktSend(0xB4,2)); rearMbsMotor = true ;
        gen->setanIcon(":/rearBtn.png", ui->rearBtn);
        QTimer::singleShot(500,this,&mobsatservices::getBackStopRearBtn);
    }
}
void mobsatservices::getBackStopRearBtn()
{
    if(!(handler->stBytes[2] & 0x01))
    {
        gen->setanIcon(":/rearRedBtn.png", ui->rearBtn) ;
        rearMbsMotor = false ;
    }
}

//==========================================================================
void mobsatservices::on_stopLvlBtn_clicked()
{
    handler->serialSend(pcktSend(0xAB,2));
    gen->setanIcon(":/stopRedBtn.png", ui->stopLvlBtn);
    QTimer::singleShot(500,this,&mobsatservices::getBackStopBtn);
}

void mobsatservices::getBackStopBtn()
{
    gen->setanIcon(":/stopLvlBtn.png", ui->stopLvlBtn) ;
}

//==========================================================================
void mobsatservices::on_frontBtn_clicked()
{
    if(!rearMbsMotor)
    {
        handler->serialSend(pcktSend(0xA4,2));
        gen->setanIcon(":/frontBtn.png", ui->frontBtn) ;
        frontMbsMotor = true ;
        QTimer::singleShot(500,this,&mobsatservices::getBackStopFrontBtn);
    }
}
void mobsatservices::getBackStopFrontBtn()
{
    if(!(handler->stBytes[2] & 0x01))
    {
        gen->setanIcon(":/frontRedBtn.png", ui->frontBtn) ;
        frontMbsMotor = false ;
    }
}

//============================ Reading Status Bytes to Determine State of Buttons ============================//
void mobsatservices::setStatusBytes()
{
    if(!(handler->stBytes[2] & 0x01)) // Motor State Arrows
    {
        gen->setanIcon(":/rearRedBtn.png",ui->rearBtn) ; rearMbsMotor = false ;
        gen->setanIcon(":/frontRedBtn.png", ui->frontBtn) ; frontMbsMotor = false ;
    }
    else
    {
        if(rearMbsMotor) gen->setanIcon(":/rearBtn.png",ui->rearBtn) ;
        else if(frontMbsMotor) gen->setanIcon(":/frontBtn.png", ui->frontBtn) ;
    }

    ///////// Swich Buttons Init //////////////
    if(handler->stBytes[3] & 0x40) gen->onButton(ui->frontClmpBtn);
    else gen->offButton(ui->frontClmpBtn) ;

    if(handler->stBytes[3] & 0x20) gen->onButton(ui->rearClmpBtn);
    else gen->offButton(ui->rearClmpBtn);

    if(handler->stBytes[3] & 0x10) gen->onButton(ui->pedClmpBtn);
    else gen->offButton(ui->pedClmpBtn);

    if(0x04 & (handler->stBytes[3])) gen->onButton(ui->pedPwrMbsBtn);
    else gen->offButton(ui->pedPwrMbsBtn);

    if((0x08 & (handler->stBytes[3])) && pedParkMbsClick) gen->setanIcon(":/pedParkMbsGrnBtn.png", ui->pedParkMbsBtn) ;
    else if(!(0x08 & (handler->stBytes[3]))) {gen->setanIcon(":/pedParkMbsBtn.png", ui->pedParkMbsBtn) ; pedParkMbsClick= false;}

    ///////////// Status Display ////////////////
    gen->defState(ui->clpPrxyPedBtn , (0x80 & handler->stBytes[2]),QSize(22,24));
    gen->defState(ui->dirPrxyBtn , (0x80 & handler->stBytes[3]),QSize(22,24));
    gen->defState(ui->dr1MicSwBtn , (0x04 & handler->stBytes[2]),QSize(22,24));
    gen->defState(ui->dr2MicSwBtn , (0x02 & handler->stBytes[2]),QSize(22,24));
    gen->defState(ui->rerClpPrxyBtn , (0x08 & handler->stBytes[2]),QSize(22,24));
    gen->defState(ui->frtClpPrxyBtn , (0x10 & handler->stBytes[2]),QSize(22,24));
    gen->defState(ui->rerMicSwBtn , (0x20 & handler->stBytes[2]),QSize(22,24));
    gen->defState(ui->frtMicSwBtn , (0x40 & handler->stBytes[2]),QSize(22,24));
    gen->defState(ui->pedParkSttsMbsBtn, (0x08 & (handler->stBytes[3])),QSize(22,24)) ;
    gen->defState(ui->pedPwrSttsMbsBtn, (0x04 & (handler->stBytes[3])),QSize(22,24)) ;
}

//============== checking GPIO Pins State and Sending Correspond Command ==============//
void mobsatservices::checkGPIO()
{
    if(!gp1.getState()) cmdObj->on_mbstStupBtn_clicked();
    if(!gp2.getState()) cmdObj->on_mbnStupBtn_clicked();
    if(!gp3.getState()) cmdObj->on_lvlingStupBtn_clicked();
    if(!gp4.getState()) cmdObj->on_nrmlStupModBtn_clicked() ;
    if(!gp5.getState()) cmdObj->on_stopBtn_clicked();
//    if(!gp6.getState()) cmdObj->on_dfctionModBtn_clicked();
    if(!gp7.getState()) cmdObj->on_nrmlRlifModBtn_clicked();
    if(!gp8.getState()) cmdObj->on_lvlingRlifBtn_clicked();
    if(!gp9.getState()) cmdObj->on_mbnRlifBtn_clicked();
    if(!gp10.getState()) cmdObj->on_mbstRlifBtn_clicked() ;
}

//==========================================================================
void mobsatservices::on_pedClmpBtn_clicked()
{
    if(!(handler->stBytes[3] & 0x10)) handler->serialSend(pcktSend(0xA4,3));
    else handler->serialSend(pcktSend(0xB4,3)) ;
}

void mobsatservices::on_rearClmpBtn_clicked()
{
    if(!(handler->stBytes[3] & 0x20)) handler->serialSend(pcktSend(0xA4,5));
    else handler->serialSend(pcktSend(0xB4,5));
}

void mobsatservices::on_frontClmpBtn_clicked()
{
    if(!(handler->stBytes[3] & 0x40)) handler->serialSend(pcktSend(0xA4,4));
    else handler->serialSend(pcktSend(0xB4,4));
}


void mobsatservices::on_pedParkMbsBtn_clicked()
{
    handler->serialSend(pcktSend(0xA4,6));
    pedParkMbsClick = true ;
    pedParkMbsTim->start(1000) ;

}

void mobsatservices::pedParkMbsCheck()
{
    if(0x08 & (handler->stBytes[3]))
    {
        gen->setanIcon(":/pedParkMbsGrnBtn.png", ui->pedParkMbsBtn) ;
        pedParkMbsTim->stop();
    }
    else
    {
        if(num_sec_mbs == 10)
        {
            if(0x08 & (handler->stBytes[3])) gen->setanIcon(":/pedParkMbsGrnBtn.png", ui->pedParkMbsBtn) ;
            else
                {gen->setanIcon(":/pedParkMbsBtn.png", ui->pedParkMbsBtn) ;
                pedParkMbsClick = false ;}

            pedParkMbsTim->stop();
            num_sec_mbs = 0 ;
        }
        else
        {
            num_sec_mbs++ ;
            if(!pedParkMbsState) {gen->setanIcon(":/pedParkMbsGrnBtn.png", ui->pedParkMbsBtn) ; pedParkMbsState = true;}
            else {gen->setanIcon(":/pedParkMbsBtn.png", ui->pedParkMbsBtn) ; pedParkMbsState = false; }
        }
    }
}

void mobsatservices::on_pedPwrMbsBtn_clicked()
{
    if(!(0x04 & (handler->stBytes[3]))) handler->serialSend(pcktSend(0xA4,7));
    else handler->serialSend(pcktSend(0xB4,7));
}

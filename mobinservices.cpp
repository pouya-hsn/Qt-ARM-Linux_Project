#include "mobinservices.h"
#include "ui_mobinservices.h"
#include "home.h"
#include "services.h"
#include "pckthandler.h"
#include "command.h"
#include "gpio.h"
#include <QTimer>
#include "genfunc.h"
//#include <QDebug>

#define mobSrvHead 0x3B
extern genFunc *gen ;
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

extern bool upSunroofClicked ;
extern bool dwnSunroofClicked ;
extern bool upMotorClicked ;
extern bool dwnMotorClicked ;
extern bool pedParkMbnClick ;

QTimer *pedParkTim;
int num_sec = 0 ;
bool pedParkState = false;

mobinservices::mobinservices(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mobinservices)
{
    ui->setupUi(this);
    gen = new genFunc();
    gpioTim = new QTimer();
    connect(gpioTim, &QTimer::timeout, this, &mobinservices::checkGPIO);
    gpioTim->start(50) ;
    setStates() ;
    connect(handler, &pckthandler::sttsChanged, this, &mobinservices::setStates) ;
    pedParkTim = new QTimer() ;
    connect(pedParkTim, &QTimer::timeout, this, &mobinservices::pedParkBtnCheck) ;
    if(pedParkMbnClick) on_pedParkMbnBtn_clicked();
//    if(pedParkMbnClick && (0x04 & (handler->stBytes[1]))) gen->setanIcon(":/pedParkGrnBtn.png", ui->pedParkMbnBtn);
}

mobinservices::~mobinservices()
{
    delete ui;
}

void mobinservices::on_bckMbnBtn_clicked()
{
    srvObj = new services();
    srvObj->show();
    this->close();
    gpioTim->stop();
    pedParkTim->stop();
}

QByteArray mobinservices::pcktSend(unsigned char data , unsigned char byteNum)
{
    QByteArray pckt;
    pckt.append(mobSrvHead);
    for (unsigned char i = 2 ; i < 9 ; i++) {
        if(i==byteNum) pckt.append(data);
        else  pckt.append(char(0x00));
    }
    return pckt;
}

//========================================================================
void mobinservices::on_upMotorBtn_clicked()
{
    if(!dwnMotorClicked)
    {
        handler->serialSend(pcktSend(0xA3,2));
        gen->setanIcon(":/upGrnBtn.png", ui->upMotorBtn) ;
        upMotorClicked = true ;
        QTimer::singleShot(500,this,&mobinservices::getBackStopUpMotor);
    }
}
void mobinservices::getBackStopUpMotor()
{
    if(!(handler->stBytes[1] & 0x40))
    {
        gen->setanIcon(":/upBtn.png", ui->upMotorBtn) ;
        upMotorClicked = false ;
    }
}

//========================================================================
void mobinservices::on_stopMotorBtn_clicked()
{
    handler->serialSend(pcktSend(0xAB,2));
    gen->setanIcon(":/stopRedBtn.png", ui->stopMotorBtn) ;
    QTimer::singleShot(500,this,&mobinservices::getBackStopMotor);
}

void mobinservices::getBackStopMotor()
{
    gen->setanIcon(":/stopLvlBtn.png", ui->stopMotorBtn) ;
}

//========================================================================
void mobinservices::on_dwnMotorBtn_clicked()
{
    if(!upMotorClicked)
    {
        handler->serialSend(pcktSend(0xB3,2));
        gen->setanIcon(":/downGrnBtn.png", ui->dwnMotorBtn) ;
        dwnMotorClicked = true ;
        QTimer::singleShot(500,this,&mobinservices::getBackStopDwnMotor);
    }
}
void mobinservices::getBackStopDwnMotor()
{
    if(!(handler->stBytes[1] & 0x40))
    {
        gen->setanIcon(":/dwnBtn.png", ui->dwnMotorBtn) ;
        dwnMotorClicked = false ;
    }
}

//============================================================================
void mobinservices::on_upSunroofBtn_clicked()
{
    if(!dwnSunroofClicked)
    {
        handler->serialSend(pcktSend(0xA3,6));
        gen->setanIcon(":/upGrnBtn.png", ui->upSunroofBtn) ;
        upSunroofClicked = true ;
        QTimer::singleShot(500,this,&mobinservices::getBackStopUpSunroof);
    }
}
void mobinservices::getBackStopUpSunroof()
{
    if(!(handler->stBytes[1] & 0x80))
    {
        gen->setanIcon(":/upBtn.png", ui->upSunroofBtn) ;
        upSunroofClicked = false ;
    }
}

//============================================================================
void mobinservices::on_stopSunroofBtn_clicked()
{
    handler->serialSend(pcktSend(0xAB,6));
    gen->setanIcon(":/stopRedBtn.png", ui->stopSunroofBtn) ;
    QTimer::singleShot(500,this,&mobinservices::getBackStopSunroof);
}

void mobinservices::getBackStopSunroof()
{
    gen->setanIcon(":/stopLvlBtn.png", ui->stopSunroofBtn) ;

}

//============================================================================
void mobinservices::on_dwnSunroofBtn_clicked()
{
    if(!upSunroofClicked)
    {
        handler->serialSend(pcktSend(0xB3,6));
        gen->setanIcon(":/downGrnBtn.png", ui->dwnSunroofBtn) ;
        dwnSunroofClicked = true ;
        QTimer::singleShot(500,this,&mobinservices::getBackStopDwnSunroof);
    }
}
void mobinservices::getBackStopDwnSunroof()
{
    if(!(handler->stBytes[1] & 0x80))
    {
        gen->setanIcon(":/dwnBtn.png", ui->dwnSunroofBtn) ;
        dwnSunroofClicked = false ;
    }
}

//============================================================================
void mobinservices::on_upClmpBtn_clicked()
{
    if(!(handler->stBytes[1] & 0x20)) handler->serialSend(pcktSend(0xA3,4));
    else handler->serialSend(pcktSend(0xB3,4));
}

void mobinservices::on_downClmpBtn_clicked()
{
    if(!(handler->stBytes[1] & 0x10)) handler->serialSend(pcktSend(0xA3,5));
    else handler->serialSend(pcktSend(0xB3,5));
}

void mobinservices::on_pedClmpBtn_clicked()
{
    if(!(handler->stBytes[1] & 0x08)) handler->serialSend(pcktSend(0xA3,3));
    else handler->serialSend(pcktSend(0xB3,3));

}

void mobinservices::setStates()
{
    //--------- Define States for Mobin Motor ----------//
    if(!(handler->stBytes[1] & 0x40))
    {
        gen->setanIcon(":/upBtn.png", ui->upMotorBtn) ; upMotorClicked = false ;
        gen->setanIcon(":/dwnBtn.png", ui->dwnMotorBtn) ; dwnMotorClicked = false ;
    }
    else
    {
        if(upMotorClicked) gen->setanIcon(":/upGrnBtn.png", ui->upMotorBtn) ;
        else if(dwnMotorClicked) gen->setanIcon(":/downGrnBtn.png", ui->dwnMotorBtn) ;
    }
    //--------- Define States for Sunroof Motor ----------//
    if(!(handler->stBytes[1] & 0x80))
    {
        gen->setanIcon(":/upBtn.png", ui->upSunroofBtn) ; upSunroofClicked = false ;
        gen->setanIcon(":/dwnBtn.png", ui->dwnSunroofBtn) ; dwnSunroofClicked = false ;
    }
    else
    {
        if(upSunroofClicked) gen->setanIcon(":/upGrnBtn.png", ui->upSunroofBtn) ;
        else if(dwnSunroofClicked) gen->setanIcon(":/downGrnBtn.png", ui->dwnSunroofBtn) ;
    }

    //--------- Switch Buttons Init ---------//
    if(handler->stBytes[1] & 0x20) gen->onButton(ui->upClmpBtn);
    else gen->offButton(ui->upClmpBtn);

    if(handler->stBytes[1] & 0x10) gen->onButton(ui->downClmpBtn);
    else gen->offButton(ui->downClmpBtn);

    if(handler->stBytes[1] & 0x08) gen->onButton(ui->pedClmpBtn);
    else gen->offButton(ui->pedClmpBtn);

    if((0x02 & (handler->stBytes[1]))) gen->onButton(ui->pedPwrMbnBtn);
    else gen->offButton(ui->pedPwrMbnBtn);

    if((0x04 & (handler->stBytes[1])) && pedParkMbnClick) gen->setanIcon(":/pedParkGrnBtn.png", ui->pedParkMbnBtn);
    else if(!(0x04 & (handler->stBytes[1]))) {gen->setanIcon(":/pedParkBtn.png", ui->pedParkMbnBtn) ; pedParkMbnClick = false ;}

    //------------- Status Display -----------//
    gen->defState(ui->clpPrxyPedBtn , (0x80 & handler->stBytes[0]),QSize(22,24)) ;
    gen->defState(ui->dwnMicSwBtn , (0x40 & handler->stBytes[0]),QSize(22,24));
    gen->defState(ui->upMicSwBtn , (0x20 & handler->stBytes[0]),QSize(22,24));
    gen->defState(ui->dwnClpPrxyBtn , (0x10 & handler->stBytes[0]),QSize(22,24));
    gen->defState(ui->upClpPrxyBtn , (0x08 & handler->stBytes[0]),QSize(22,24));
    gen->defState(ui->opnMicSwSnrfBtn , (0x04 & handler->stBytes[0]),QSize(22,24));
    gen->defState(ui->clsMicSwSnrfBtn , (0x02 & handler->stBytes[0]),QSize(22,24));
    gen->defState(ui->dirPrxyBtn , (0x01 & handler->stBytes[0]),QSize(22,24));
    gen->defState(ui->pedParkSttsMbnBtn, (0x04 & (handler->stBytes[1])),QSize(22,24)) ; //ped park
    gen->defState(ui->pedPwrSttsMbnBtn, (0x02 & (handler->stBytes[1])),QSize(22,24)) ;  //ped power

}

void mobinservices::checkGPIO()
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


void mobinservices::on_pedParkMbnBtn_clicked()
{

    handler->serialSend(pcktSend(0xA3,7));
    pedParkMbnClick = true ;
    pedParkTim->start(1000) ;

}

void mobinservices::pedParkBtnCheck()
{
    if(0x04 & (handler->stBytes[1]))
    {
        gen->setanIcon(":/pedParkGrnBtn.png", ui->pedParkMbnBtn) ;
        pedParkTim->stop();
    }
    else
    {
        if(num_sec == 10)
        {
            if(0x04 & (handler->stBytes[1])) gen->setanIcon(":/pedParkGrnBtn.png", ui->pedParkMbnBtn) ;
            else
                {gen->setanIcon(":/pedParkBtn.png", ui->pedParkMbnBtn)  ;
                pedParkMbnClick = false ;}

            pedParkTim->stop();
            num_sec = 0 ;
        }
        else
        {
            num_sec++ ;
            if(!pedParkState) {gen->setanIcon(":/pedParkGrnBtn.png", ui->pedParkMbnBtn) ; pedParkState = true;}
            else {gen->setanIcon(":/pedParkBtn.png", ui->pedParkMbnBtn) ; pedParkState = false;}
        }
    }
}

void mobinservices::on_pedPwrMbnBtn_clicked()
{
    if(!(0x02 & (handler->stBytes[1]))) handler->serialSend(pcktSend(0xA3,8));
    else handler->serialSend(pcktSend(0xB3,8));
}

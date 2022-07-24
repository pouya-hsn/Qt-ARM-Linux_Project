#include "levelingservices.h"
#include "ui_levelingservices.h"
#include "home.h"
#include "services.h"
#include "pckthandler.h"
#include "gpio.h"
#include "command.h"
#include "genfunc.h"
#include <QTimer>


#define levelHeader 0x5B

extern genFunc *gen;
extern pckthandler *handler ;
extern home *homeObj;
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


extern bool upJ1; extern bool dwnJ1 ;
extern bool upJ2; extern bool dwnJ2 ;
extern bool upJ3 ; extern bool dwnJ3 ;
extern bool upJ4 ; extern bool dwnJ4 ;
extern bool upAllJack; extern bool dwnAllJack;


levelingServices::levelingServices(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::levelingServices)
{
    ui->setupUi(this);
    gen = new genFunc() ;
    gpioTim = new QTimer();
    connect(gpioTim, &QTimer::timeout, this, &levelingServices::checkGPIO);
    gpioTim->start(50);

    setStates() ;
    XYAnglesShow();
    connect(handler, &pckthandler::sttsChanged, this, &levelingServices::setStates) ;
    connect(handler, &pckthandler::sttsChanged, this, &levelingServices::XYAnglesShow) ;
}

levelingServices::~levelingServices()
{
    delete ui;
}

void levelingServices::on_bckLvlingBtn_clicked()
{
    srvObj = new services();
    srvObj->show();
    this->close();
    gpioTim->stop();
}

QByteArray levelingServices::pcktSend(unsigned char data,unsigned char byteNum)
{
    QByteArray pckt ;
    pckt.append(levelHeader);
    for(unsigned char i = 2 ; i < 10 ; i++)
    {
        if(i==byteNum)
            pckt.append(data);
        else
            pckt.append(char(0x00));
    }
    return pckt;
}

//=================================================================================
void levelingServices::on_upJack1Btn_clicked()
{
    handler->serialSend(pcktSend(0xA5,3));
    gen->setanIcon(":/upGrnBtn.png", ui->upJack1Btn) ;
    upJ1 = true ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopUpJ1);
}
void levelingServices::getBackStopUpJ1()
{
    if(!(handler->stBytes[5] & 0x20)) gen->setanIcon(":/upBtn.png", ui->upJack1Btn) ;
}

//=================================================================================
void levelingServices::on_stopJack1Btn_clicked()
{
    handler->serialSend(pcktSend(0xAB,3));
    gen->setanIcon(":/stopRedBtn.png", ui->stopJack1Btn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopJ1);
}

void levelingServices::getBackStopJ1()
{
    gen->setanIcon(":/stopLvlBtn.png", ui->stopJack1Btn) ;
}

//=================================================================================
void levelingServices::on_dwnJack1Btn_clicked()
{
    handler->serialSend(pcktSend(0xB5,3));
    gen->setanIcon(":/downGrnBtn.png", ui->dwnJack1Btn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopDwnJ1);
    dwnJ1 = true ;
}
void levelingServices::getBackStopDwnJ1()
{
    if(!(handler->stBytes[5] & 0x04)) gen->setanIcon(":/dwnBtn.png", ui->dwnJack1Btn) ;

}

//=================================================================================
void levelingServices::on_upJack2Btn_clicked()
{
    handler->serialSend(pcktSend(0xA5,4));
    gen->setanIcon(":/upGrnBtn.png", ui->upJack2Btn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopUpJ2);
    upJ2 = true ;
}
void levelingServices::getBackStopUpJ2()
{
    if(!(handler->stBytes[5] & 0x20)) gen->setanIcon(":/upBtn.png", ui->upJack2Btn) ;
}

//=================================================================================
void levelingServices::on_stopJack2Btn_clicked()
{
    handler->serialSend(pcktSend(0xAB,4));
    gen->setanIcon(":/stopRedBtn.png", ui->stopJack2Btn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopJ2);
}

void levelingServices::getBackStopJ2()
{
    gen->setanIcon(":/stopLvlBtn.png", ui->stopJack2Btn) ;
}

//=================================================================================
void levelingServices::on_dwnJack2Btn_clicked()
{
    handler->serialSend(pcktSend(0xB5,4));
    gen->setanIcon(":/downGrnBtn.png", ui->dwnJack2Btn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopDwnJ2);
    dwnJ2 = true ;
}
void levelingServices::getBackStopDwnJ2()
{
    if(!(handler->stBytes[5] & 0x02)) gen->setanIcon(":/dwnBtn.png", ui->dwnJack2Btn) ;
}

//=================================================================================
void levelingServices::on_upJack3Btn_clicked()
{
    handler->serialSend(pcktSend(0xA5,5));
    gen->setanIcon(":/upGrnBtn.png", ui->upJack3Btn) ;
    upJ3 = true ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopUpJ3);
}
void levelingServices::getBackStopUpJ3()
{
    if(!(handler->stBytes[5] & 0x20)) gen->setanIcon(":/upBtn.png", ui->upJack3Btn) ;
}

//=================================================================================
void levelingServices::on_stopJack3Btn_clicked()
{
    handler->serialSend(pcktSend(0xAB,5));
    gen->setanIcon(":/stopRedBtn.png", ui->stopJack3Btn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopJ3);
}
void levelingServices::getBackStopJ3()
{
    gen->setanIcon(":/stopLvlBtn.png", ui->stopJack3Btn) ;
}

//=================================================================================
void levelingServices::on_dwnJack3Btn_clicked()
{
    handler->serialSend(pcktSend(0xB5,5));
    gen->setanIcon(":/downGrnBtn.png", ui->dwnJack3Btn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopDwnJ3);
    dwnJ3 = true ;
}
void levelingServices::getBackStopDwnJ3()
{
    if(!(handler->stBytes[5] & 0x01)) gen->setanIcon(":/dwnBtn.png", ui->dwnJack3Btn) ;
}

//=================================================================================
void levelingServices::on_upJack4Btn_clicked()
{
    handler->serialSend(pcktSend(0xA5,6));
    gen->setanIcon(":/upGrnBtn.png", ui->upJack4Btn) ;
    upJ4 = true ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopUpJ4);
}
void levelingServices::getBackStopUpJ4()
{
    if(!(handler->stBytes[5] & 0x20)) gen->setanIcon(":/upBtn.png", ui->upJack4Btn) ;
}

//=================================================================================
void levelingServices::on_stopJack4Btn_clicked()
{
    handler->serialSend(pcktSend(0xAB,6));
    gen->setanIcon(":/stopRedBtn.png", ui->stopJack4Btn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopJ4);
}
void levelingServices::getBackStopJ4()
{
    gen->setanIcon(":/stopLvlBtn.png", ui->stopJack4Btn) ;
}

//=================================================================================
void levelingServices::on_dwnJack4Btn_clicked()
{
    handler->serialSend(pcktSend(0xB5,6));
    gen->setanIcon(":/downGrnBtn.png", ui->dwnJack4Btn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopDwnJ4);
    dwnJ4 = true ;
}
void levelingServices::getBackStopDwnJ4()
{
    if(!(handler->stBytes[5] & 0x80)) gen->setanIcon(":/dwnBtn.png", ui->dwnJack4Btn) ;
}

//=================================================================================
void levelingServices::on_upAllJackBtn_clicked()
{
    handler->serialSend(pcktSend(0xA5,7));
    gen->setanIcon(":/upGrnBtn.png", ui->upAllJackBtn) ;
    upAllJack = true ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopUpAllJ);
}
void levelingServices::getBackStopUpAllJ()
{
    if(!(handler->stBytes[5] & 0x20)) gen->setanIcon(":/upBtn.png", ui->upAllJackBtn) ;
}

//=================================================================================
void levelingServices::on_stopAllJackBtn_clicked()
{
    handler->serialSend(pcktSend(0xAB,7));
    gen->setanIcon(":/stopRedBtn.png", ui->stopAllJackBtn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopAllJ);
}
void levelingServices::getBackStopAllJ()
{
    gen->setanIcon(":/stopLvlBtn.png", ui->stopAllJackBtn) ;
}

//=================================================================================
void levelingServices::on_dwnAllJackBtn_clicked()
{
    handler->serialSend(pcktSend(0xB5,7));
    gen->setanIcon(":/downGrnBtn.png", ui->dwnAllJackBtn) ;
    QTimer::singleShot(500,this,&levelingServices::getBackStopDwnAllJ);
    dwnAllJack = true ;
}
void levelingServices::getBackStopDwnAllJ()
{
    if(!(handler->stBytes[5] & 0x40)) gen->setanIcon(":/dwnBtn.png", ui->dwnAllJackBtn) ;
}

//=================================================================================
void levelingServices::on_pwrMotorBtn_clicked()
{
    if(!(handler->stBytes[5] & 0x40)) handler->serialSend(pcktSend(0xA5,2));
    else handler->serialSend(pcktSend(0xB5,2));
}

void levelingServices::on_highSpeedValBtn_clicked()
{
    if(!(handler->stBytes[5] & 0x10)) handler->serialSend(pcktSend(0xA5,8));
    else handler->serialSend(pcktSend(0xB5,8));
}

void levelingServices::on_lowSpeedValBtn_clicked()
{
    if(!(handler->stBytes[5] & 0x08)) handler->serialSend(pcktSend(0xA5,9));
    else handler->serialSend(pcktSend(0xB5,9));

}

void levelingServices::setStates()
{
    //---------- Define Color of the Arrows commands -----------//
    if(!(handler->stBytes[5] & 0x04)) {gen->setanIcon(":/dwnBtn.png", ui->dwnJack1Btn); dwnJ1 =false;}
    else if(dwnJ1) gen->setanIcon(":/downGrnBtn.png", ui->dwnJack1Btn);

    if(!(handler->stBytes[5] & 0x02)) {gen->setanIcon(":/dwnBtn.png", ui->dwnJack2Btn); dwnJ2 = false ;}
    else if(dwnJ2) gen->setanIcon(":/downGrnBtn.png", ui->dwnJack2Btn);

    if(!(handler->stBytes[5] & 0x01)) {gen->setanIcon(":/dwnBtn.png", ui->dwnJack3Btn); dwnJ3 = false ;}
    else if(dwnJ3) gen->setanIcon(":/downGrnBtn.png", ui->dwnJack3Btn);

    if(!(handler->stBytes[6] & 0x80)) {gen->setanIcon(":/dwnBtn.png", ui->dwnJack4Btn); dwnJ4 = false ;}
    else if(dwnJ4) gen->setanIcon(":/downGrnBtn.png", ui->dwnJack4Btn);

    if(!(handler->stBytes[6] & 0x40)) {gen->setanIcon(":/dwnBtn.png", ui->dwnAllJackBtn); dwnAllJack = false ;}
    else if(dwnAllJack) gen->setanIcon(":/downGrnBtn.png", ui->dwnAllJackBtn);

    if(!(handler->stBytes[5] & 0x20))
    {
        gen->setanIcon(":/upBtn.png", ui->upAllJackBtn); upAllJack = false;
        gen->setanIcon(":/upBtn.png", ui->upJack4Btn); upJ1 = false;
        gen->setanIcon(":/upBtn.png", ui->upJack3Btn); upJ2 = false;
        gen->setanIcon(":/upBtn.png", ui->upJack2Btn); upJ3 = false;
        gen->setanIcon(":/upBtn.png", ui->upJack1Btn); upJ4 = false;
    }
    else
    {
//        qDebug() << upAllJack ;
        if(upAllJack) gen->setanIcon(":/upGrnBtn.png", ui->upAllJackBtn);
        if(upJ4) gen->setanIcon(":/upGrnBtn.png", ui->upJack4Btn);
        if(upJ3) gen->setanIcon(":/upGrnBtn.png", ui->upJack3Btn);
        if(upJ2) gen->setanIcon(":/upGrnBtn.png", ui->upJack2Btn);
        if(upJ1) gen->setanIcon(":/upGrnBtn.png", ui->upJack1Btn);
    }

    //------------ Define Green or Red for All Switches -----------///
    if(!(handler->stBytes[5] & 0x40)) gen->offButton(ui->pwrMotorBtn);
    else gen->onButton(ui->pwrMotorBtn);
    if(!(handler->stBytes[5] & 0x10)) gen->offButton(ui->highSpeedValBtn);
    else gen->onButton(ui->highSpeedValBtn);
    if(!(handler->stBytes[5] & 0x08)) gen->offButton(ui->lowSpeedValBtn);
    else gen->onButton(ui->lowSpeedValBtn);

    //------------ Define Green or Red for All status Options -----------///
    gen->defState(ui->jck1PrxyBtn , (0x80 & handler->stBytes[4]),QSize(22,24));
    gen->defState(ui->jck2PrxyBtn , (0x40 & handler->stBytes[4]),QSize(22,24));
    gen->defState(ui->jck3PrxyBtn , (0x20 & handler->stBytes[4]),QSize(22,24));
    gen->defState(ui->jck4PrxyBtn , (0x10 & handler->stBytes[4]),QSize(22,24));
    gen->defState(ui->disVal , (0x20 & handler->stBytes[5]),QSize(22,24));
    gen->defState(ui->prsSw1Btn , (0x08 & handler->stBytes[4]),QSize(22,24));
    gen->defState(ui->prsSw2Btn , (0x04 & handler->stBytes[4]),QSize(22,24));
    gen->defState(ui->prsSw3Btn , (0x02 & handler->stBytes[4]),QSize(22,24));
    gen->defState(ui->prsSw4Btn , (0x01 & handler->stBytes[4]),QSize(22,24));
    gen->defState(ui->mnPrsSw , (0x80 & handler->stBytes[5]),QSize(22,24));
}

void levelingServices::XYAnglesShow()
{
    // showing the X,Y Angles on Leveling Icon in Home Page
    long int xBytes = ((handler->stBytes[7])& 0x000000FF) | ((handler->stBytes[8] << 8)& 0x0000FF00) |
            ((handler->stBytes[9] << 16)& 0x00FF0000) | ((handler->stBytes[10] << 24)&0xFF000000);
    float x = float(xBytes)/1000 ;

    long int yBytes = ((handler->stBytes[11])& 0x000000FF) | ((handler->stBytes[12] << 8)& 0x0000FF00) |
            ((handler->stBytes[13] << 16)& 0x00FF0000) | ((handler->stBytes[14] <<24) & 0xFF000000);
    float y = float(yBytes)/1000 ;
    ui->xLblVal->setText(QString("%1").arg(x)) ;
    ui->yLblVal->setText(QString("%1").arg(y)) ;
}

void levelingServices::checkGPIO()
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

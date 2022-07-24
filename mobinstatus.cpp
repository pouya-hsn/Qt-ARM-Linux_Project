#include "mobinstatus.h"
#include "ui_mobinstatus.h"
#include "home.h"
#include "command.h"
#include "power.h"
#include "cooler.h"
#include "temp.h"
#include "status.h"
#include "mobsatstatus.h"
#include "levelingstatus.h"
#include "pckthandler.h"
#include "power.h"
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include "gpio.h"
#include "genfunc.h"
#include <QTimer>

extern genFunc *gen;
extern pckthandler *handler;
extern levelingstatus *lvlStatusObj;
extern mobsatstatus *msatStatusObj ;
extern command *cmdObj ;
extern power *pwrObj ;
extern cooler *coolObj ;
extern temp *tmpHmdObj ;
extern status *statusObj;
extern Leveling *lvlObj ;
extern Mobin *mbnObj ;
extern Mobsat *mbsObj ;

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

mobinstatus::mobinstatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mobinstatus)
{
    ui->setupUi(this);
    gen = new genFunc() ;
    gpioTim = new QTimer();
    connect(gpioTim, &QTimer::timeout, this, &mobinstatus::checkGPIO);
    gpioTim->start(50);

    setStatusBytes();
    connect(handler, &pckthandler::sttsChanged, this, &mobinstatus::setStatusBytes);
}

mobinstatus::~mobinstatus()
{
    delete ui;
}

void mobinstatus::on_cmmndBtn_clicked()
{
    cmdObj = new command();
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void mobinstatus::on_pwrBtn_clicked()
{
    pwrObj = new power();
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void mobinstatus::on_clrBtn_clicked()
{
    coolObj = new cooler();
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void mobinstatus::on_tempClrBtn_clicked()
{
    tmpHmdObj = new temp();
    tmpHmdObj->show();
    this->close();
    gpioTim->stop();
}

void mobinstatus::on_bckBtn_clicked()
{
    statusObj = new status();
    statusObj->show();
    this->close();
    gpioTim->stop();
}

void mobinstatus::on_mbstBtn_clicked()
{
    mbsObj = new Mobsat() ;
    mbsObj->show();
    this->close();
    gpioTim->stop();
}

void mobinstatus::on_lvlingBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    this->close();
    gpioTim->stop();
}

void mobinstatus::on_mbnBtn_clicked()
{
    mbnObj = new Mobin() ;
    mbnObj->show();
    this->close();
    gpioTim->stop();
}


void mobinstatus::setStatusBytes()
{
    gen->defState(ui->clpPrxyPed , (0x80 & (handler->stBytes[0])),QSize(18,18)); // clamp proximeter of ped
    gen->defState(ui->dwnMicSw , (0x40 & (handler->stBytes[0])),QSize(18,18)); // down micro switch
    gen->defState(ui->upMicSw , (0x20 & (handler->stBytes[0])),QSize(18,18)); // up micro switch
    gen->defState(ui->dwnClpPrxy , (0x10 & (handler->stBytes[0])),QSize(18,18)); //down clamp proximeter
    gen->defState(ui->upClpPrxy , (0x08 & (handler->stBytes[0])),QSize(18,18)); // up clamp proximeter
    gen->defState(ui->opnMicSwSnrf , (0x04 & (handler->stBytes[0])),QSize(18,18)); // open micro switch sunroof
    gen->defState(ui->clsMicSwSnrf , (0x02 & (handler->stBytes[0])),QSize(18,18)); // close micro switch sunroof
    gen->defState(ui->dirPrxy , (0x01 & (handler->stBytes[0])),QSize(18,18)); // direction proximeter
    gen->defState(ui->snrfMot , (0x80 & (handler->stBytes[1])),QSize(18,18)); // sunroof motor
    gen->defState(ui->mbnMot , (0x40 & (handler->stBytes[1])),QSize(18,18)); // mobin motor
    gen->defState(ui->pedClp , (0x08 & (handler->stBytes[1])),QSize(18,18)); // ped clamp
    gen->defState(ui->upClp , (0x20 & (handler->stBytes[1])),QSize(18,18)); // up clamp
    gen->defState(ui->dwnClp , (0x10 & (handler->stBytes[1])),QSize(18,18)); // down clamp
    gen->defState(ui->pedParkSttsMbnBtn, (0x04 & (handler->stBytes[1])),QSize(18,18)) ; //ped park
    gen->defState(ui->pedPwrSttsMbnBtn, (0x02 & (handler->stBytes[1])),QSize(18,18)) ;  //ped power
}

void mobinstatus::checkGPIO()
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
